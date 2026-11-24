/*!
@file     game_mechanics.c
@author   Lee Jun Kai (l.junkai)- Player movement, wall climb, shield, collision, carrying object, kicking object, platform collisions, cling bar
@co-author  Sin Wee - Plaforms(static, moving, alternating), Win Condition, Killzone, Pressure Plate
			Lewin Yeo - Teleporting
@module   csd1401f26 Software Engineering Project 1
@section  B
@date     19/09/26
@brief	Implements the core gameplay mechanics: player movement, wall cling,
		teleport, shield, collision, pressure plates, kill zones and moving platforms.
*//*______________________________________________________________________*/

#include "cprocessing.h"
#include "game.h"
#include "game_mechanics.h"
#include "screen_seq.h"
#include "level.h"
#include "colors.h"
#include <stdio.h>
#include <math.h>
#include "npc_mechanics.h"
#include "sounds.h"
#include "scoring.h"
#include "Images.h"

#define MOVE_SPEED     300.0f
#define JUMP_SPEED     500.0f
#define WALL_CLING_TIME 2.0f
#define CLIMB_SPEED     250.0f
#define GRAVITY        1500.0f
#define MAX_FALL_SPEED 900.0f
#define GRAB_FRAME_TIME    0.15f
#define KICK_DIST 100.0f   /* how far a kick moves the object */
#define KICK_TIME 0.2f     /* how long the slide takes, in seconds */

float collision_radius = 50.0f;
float degrees;
float cornerRadius;
float animationTimer = 0.0f;
int win = 0;
int wall_climb = 1;
int is_clinging = 0;
int cling_side = 0;
float carry_timer = 0.0f;
float cling_wall_x = -1.0f;
float cling_wall_y = -1.0f;
float wall_timer = 0.0f;
int pickingUp = 0;
int dropping = 0;
int isCarrying = 0;
float fromX, fromY, toX, toY;
int kickedObject = -1;
float kick_dir = 0.0f;
float kick_moved = 0.0f;
int   shieldActive = 0;
float shieldTimer = 0.0f;

int Teleport_count = 3; // Number of Teleportes available
int Teleport_Status = 1;
float TeleportWindowTiming = 0.1f;
#define TeleportDistance 85.0f // how far the player Teleportes
#define TeleportCooldown 2.0f

/********************Player & Platform**********************/
float area_left;
float area_right;
float area_top;
float area_bottom;


/* Player variables */
CP_Vector position = { 100,100 };
CP_Vector velocity = { 0, 0 };

// Player edges (position is the player's top-left corner)
float player_left;
float player_right;
float player_top;
float player_bottom;

/************************ Player states ***********************/
typedef enum player_states
{
	STANDING = 1, // 1
	JUMPING,	  // 2
	RUNNING,      // 3
	GRABBING_1,   // 4
	GRABBING_2,   // 5
	CARRY,		  // 6
	SLIDING       // 7
} PlayerState;

int player_action = STANDING;
int onGround = 0;
int toggleGrab = 0;



struct TeleportCountTextCoordinates {
	float XLeft;
	float XMiddle;
	float Xright;
	float y;
} TeleportTextCoordinates;

/**********************Teleport Ability******************/
//Teleport Timer//
void TeleportTimer(void) {
	float dt = CP_System_GetDt();
	//Cooldown before pressing to Teleport again
	TeleportWindowTiming += dt; // Contuously increase the Teleport window timing based on the delta time
	TeleportWindowTiming = CP_Math_ClampFloat(TeleportWindowTiming, 0.0, TeleportCooldown);
	if (TeleportWindowTiming >= TeleportCooldown)
		Teleport_Status = 1; // reset the Teleport status once the Teleport cooldown is reset
}
//Things to do once Teleport is activated//
void Teleport_Activated(void) {
	CP_Sound_Play(teleportSound);
	TeleportWindowTiming = 0;
	Teleport_Status = 0;
	if (Teleport_count <= 3)
		Teleport_count--;
	score_teleport_used();
}

//Teleport Mechanic//
/*
Teleport_Mechannic

IF teleport is on cooldown
	DRAW cooldown bar above player

DRAW teleport charges HUD based on Teleport_count

IF player has charges
	IF moving right AND Left Shift pressed AND teleport ready
		MOVE player right, use teleport, RETURN 1
	IF moving left AND Left Shift pressed AND teleport ready
		MOVE player left, use teleport, RETURN 1

RETURN 0
*/
int Teleport_Mechannic(void) {
	TeleportTextCoordinates.XLeft = 1520.0;
	TeleportTextCoordinates.XMiddle = 1550.0;
	TeleportTextCoordinates.Xright = 1580.0;
	TeleportTextCoordinates.y = 40.0;
	float TeleportCooldownBarScale = 25.0f;

	CP_Settings_RectMode(CP_POSITION_CORNER);
	if (TeleportWindowTiming < TeleportCooldown) {// A "Meter" above the player's head indicating the teleport cooldown
		CP_Settings_Fill(yellow);
		CP_Graphics_DrawRect(position.x, position.y - 20.0f, TeleportWindowTiming * TeleportCooldownBarScale, 10.0f);
	}

	CP_Settings_Fill(medium_light_green);
	CP_Font_Set(Font);
	CP_Settings_TextSize(25);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_RIGHT, CP_TEXT_ALIGN_V_TOP);

	//show how many teleports the player has left
	switch (Teleport_count) {
	case 3:
		CP_Graphics_DrawRectAdvanced(TeleportTextCoordinates.XLeft, TeleportTextCoordinates.y, 20.0, 20.0, 45.0, 30.0);
	case 2:
		CP_Graphics_DrawRectAdvanced(TeleportTextCoordinates.XMiddle, TeleportTextCoordinates.y, 20.0, 20.0, 45.0, 30.0);
	case 1:
		CP_Font_DrawText("Teleport Charges", 1600.0f, 0.0f);
		CP_Graphics_DrawRectAdvanced(TeleportTextCoordinates.Xright, TeleportTextCoordinates.y, 20.0, 20.0, 45.0, 30.0);

		//CP_Graphics_DrawRect(TeleportTextCoordinates.XLeft-15.0, TeleportTextCoordinates.y+25.0, TeleportWindowTiming* TeleportCooldownBarScale, 20.0);
		break;
	case 4:
		CP_Font_DrawText("Teleport Charges", 1600.0f, 0.0f);
		CP_Font_DrawText("Unlimted Charges", 1600.0f, 30.0f);
		break;
	}

	//Activate Teleport when movement key & LeftShift is pressed
	if (Teleport_count > 0) {
		if (CP_Input_KeyDown(KEY_D) && velocity.x > 0 && Teleport_Status == 1 && CP_Input_KeyTriggered(KEY_LEFT_SHIFT)) {// If the player is moving to the right and presses the LeftShift key,
			position.x += TeleportDistance;
			Teleport_Activated();
			return 1;
		}
		if (CP_Input_KeyDown(KEY_A) && velocity.x < 0 && Teleport_Status == 1 && CP_Input_KeyTriggered(KEY_LEFT_SHIFT)) {// If the player is moving to the left and presses the LeftShift key,
			//*Note*: since CP_Input_KeyTriggered checks on the exact frame the key is pressed, the function returns true only once when the key is pressed for that frame.
			//Held Down it returns false, so the player will have to release the key and press it again to trigger the Teleport mechanic.
			position.x -= TeleportDistance;
			Teleport_Activated();
			return 1;
		}
	}
	return 0;

}

/************************Object Related Functions**********************/
/*
HeldObject_MoveTo

t = 0 means start position, t = 1 means end position

SET held object's x = start x + (end x - start x) * t
SET held object's y = start y + (end y - start y) * t
*/
void HeldObject_MoveTo(float t)
{
	current_objects[heldObject][0] = fromX + (toX - fromX) * t;
	current_objects[heldObject][1] = fromY + (toY - fromY) * t;
}

/*
Shield_Draw

IF shield is not active
	RETURN

IF shield is still in parry window
	SET colour to gold (will reflect)
ELSE
	SET colour to blue (block only)

DRAW shield circle centered on player
*/
void Shield_Draw(void) {
	if (!shieldActive) return;

	if (shieldTimer <= SHIELD_PARRY_TIME)
		CP_Settings_Fill(CP_Color_Create(255, 215, 0, 120));   // gold = will reflect
	else
		CP_Settings_Fill(CP_Color_Create(100, 180, 255, 120)); // blue = block only

	CP_Graphics_DrawCircle(position.x + PLAYER_SIZE / 2.0f,
		position.y + PLAYER_SIZE / 2.0f,
		SHIELD_SIZE);
}

/*
Shield_Update

GET frame time

IF P is pressed AND shield is not active
	TURN shield on
	RESET shield timer

IF shield is active
	ADD frame time to shield timer
	IF shield timer reaches shield duration
		TURN shield off
		RESET shield timer
*/
void Shield_Update(void) {
	float dt = CP_System_GetDt();

	/* --- P : summon shield (can't recast while one is up) --- */
	if (CP_Input_KeyTriggered(KEY_P) && !shieldActive) {
		shieldActive = 1;
		shieldTimer = 0.0f;
	}

	/* --- Shield timer: disappears after SHIELD_DURATION --- */
	if (shieldActive) {
		shieldTimer += dt;
		if (shieldTimer >= SHIELD_DURATION) {
			shieldActive = 0;
			shieldTimer = 0.0f;
		}
	}
}

/*
Character_MovementDefault

GET frame time, ground height and screen width

IF P is pressed AND shield is not active
	TURN shield on, RESET shield timer
IF shield is active
	ADD frame time to shield timer
	IF timer reaches shield duration
		TURN shield off, RESET shield timer

IF E is pressed AND not picking up AND not kicking AND on ground AND touching an object
	START pick up
	SAVE object's start position and position above player's head

IF picking up
	ADD frame time to carry timer
	PLAY grab animation and move object up in steps
	WHEN animation is done
		STOP picking up, SET player to carrying

IF F is pressed AND not picking up AND not kicking AND touching an object
	START kick
	SET kick direction away from player

IF kicking
	MOVE object a bit in kick direction
	KEEP object inside screen
	IF object has moved kick distance
		STOP kick

IF not picking up
	A moves left, D moves right

IF W or Space is pressed AND on ground AND not picking up
	JUMP

APPLY gravity
IF S is held AND in the air
	APPLY extra gravity
LIMIT fall speed

MOVE player by velocity

IF player is at or below ground
	SNAP to ground, STOP falling, SET on ground
ELSE
	SET not on ground

KEEP player inside screen horizontally
*/
void Character_MovementDefault(void) {

	int   shieldActive = 0;
	float shieldTimer = 0.0f;
	float dt = CP_System_GetDt();
	float groundY = (float)CP_System_GetWindowHeight();
	float screenW = (float)CP_System_GetWindowWidth();

	/* --- P : summon shield (can't recast while one is up) --- */
	if (CP_Input_KeyTriggered(KEY_P) && !shieldActive) {
		shieldActive = 1;
		shieldTimer = 0.0f;
	}

	/* --- Shield timer: disappears after SHIELD_DURATION --- */
	if (shieldActive) {
		shieldTimer += dt;
		if (shieldTimer >= SHIELD_DURATION) {
			shieldActive = 0;
			shieldTimer = 0.0f;
		}
	}

	/* --- E: pick up (grounded + touching an object, not mid-kick) --- */
	if (CP_Input_KeyTriggered(KEY_E) && !pickingUp && kickedObject == -1 && onGround && (heldObject = Object_Check()) != -1) {
		pickingUp = 1;
		carry_timer = 0.0f;
		fromX = current_objects[heldObject][OBJ_X];
		fromY = current_objects[heldObject][OBJ_Y];
		toX = position.x + (PLAYER_SIZE - current_objects[heldObject][OBJ_W]) / 2.0f;
		toY = position.y - current_objects[heldObject][OBJ_H];
	}
	/* --- Animation of carrying --- */
	if (pickingUp) {
		carry_timer += dt;
		if (carry_timer <= 0.15) { player_action = GRABBING_1; HeldObject_MoveTo(0.0f); }
		else if (carry_timer <= 0.3) { player_action = GRABBING_2; HeldObject_MoveTo(0.5f); }
		else if (carry_timer <= 0.5) { player_action = CARRY;      HeldObject_MoveTo(1.0f); }
		else {
			pickingUp = 0;
			carry_timer = 0.0f;
			isCarrying = 1;
			player_action = CARRY;
		}
	}

	/* --- F: kick the touched object away from the player --- */
	if (CP_Input_KeyTriggered(KEY_F) && !pickingUp && kickedObject == -1) {
		int i = Object_Check();
		if (i != -1) {
			float objMid = current_objects[i][OBJ_X] + current_objects[i][OBJ_W] / 2.0f;
			float playerMid = position.x + PLAYER_SIZE / 2.0f;
			kickedObject = i;
			kick_moved = 0.0f;
			kick_dir = (playerMid < objMid) ? 1.0f : -1.0f;   /* kicked from left = goes right */
		}
	}

	/* --- Kick slide: move a bit each frame until it has gone KICK_DIST --- */
	if (kickedObject != -1) {
		float step = KICK_DIST / KICK_TIME * dt;
		if (kick_moved + step > KICK_DIST) step = KICK_DIST - kick_moved;
		kick_moved += step;

		current_objects[kickedObject][OBJ_X] += kick_dir * step;

		/* keep it on screen */
		if (current_objects[kickedObject][OBJ_X] < 0.0f)
			current_objects[kickedObject][OBJ_X] = 0.0f;
		if (current_objects[kickedObject][OBJ_X] + current_objects[kickedObject][OBJ_W] > screenW)
			current_objects[kickedObject][OBJ_X] = screenW - current_objects[kickedObject][OBJ_W];

		if (kick_moved >= KICK_DIST) kickedObject = -1;   /* kick finished */
	}

	/* --- Horizontal: locked during the animation --- */
	velocity.x = 0.0f;
	if (!pickingUp) {
		if (CP_Input_KeyDown(KEY_A)) velocity.x -= MOVE_SPEED;
		if (CP_Input_KeyDown(KEY_D)) velocity.x += MOVE_SPEED;
	}

	/* --- Jump: only when standing on something, not during animation --- */
	if ((CP_Input_KeyTriggered(KEY_W) || CP_Input_KeyTriggered(KEY_SPACE)) && onGround && !pickingUp) {
		velocity.y = -JUMP_SPEED;
		onGround = 0;
	}

	/* --- Gravity: always applied, S makes you fall faster --- */
	velocity.y += GRAVITY * dt;
	if (CP_Input_KeyDown(KEY_S) && !onGround) {
		velocity.y += GRAVITY * dt;
	}
	if (velocity.y > MAX_FALL_SPEED) velocity.y = MAX_FALL_SPEED;

	/* --- Move --- */
	position = CP_Vector_Add(position, CP_Vector_Scale(velocity, dt));

	/* --- Ground check: snap to ground and stop falling --- */
	if (position.y + PLAYER_SIZE >= groundY) {
		position.y = groundY - PLAYER_SIZE;
		velocity.y = 0.0f;
		onGround = 1;
	}
	else {
		onGround = 0;
	}

	/* --- Keep player inside the screen horizontally --- */
	if (position.x < 0.0f) position.x = 0.0f;
	if (position.x + PLAYER_SIZE > screenW) position.x = screenW - PLAYER_SIZE;
}

/* Handles player movement, jumping, gravity, and drop animation while carrying an object. */
/*
Character_MovementCarry

GET frame time, ground height and screen width

IF E is pressed AND not dropping AND on ground
	START drop
	SAVE object's start position
	SET drop position to the right of player
	IF that is off screen
		SET drop position to the left of player

IF dropping
	ADD frame time to carry timer
	PLAY drop animation and move object down in steps
	WHEN animation is done
		STOP dropping, STOP carrying
		SET player to standing, CLEAR held object

IF not dropping
	A moves left, D moves right

IF W or Space is pressed AND on ground AND not dropping
	JUMP

APPLY gravity
IF S is held AND in the air
	APPLY extra gravity
LIMIT fall speed

MOVE player by velocity

IF player is at or below ground
	SNAP to ground, STOP falling, SET on ground
ELSE
	SET not on ground

KEEP player inside screen horizontally

IF not dropping
	KEEP held object on top of player's head
*/
void Character_MovementCarry(void)
{
	float dt = CP_System_GetDt();
	float groundY = (float)CP_System_GetWindowHeight();
	float screenW = (float)CP_System_GetWindowWidth();

	/* --- E: drop (grounded only) --- */
	if (CP_Input_KeyTriggered(KEY_E) && !dropping && onGround) {
		float w = current_objects[heldObject][OBJ_W];
		float h = current_objects[heldObject][OBJ_H];
		dropping = 1;
		carry_timer = 0.0f;
		fromX = current_objects[heldObject][OBJ_X];
		fromY = current_objects[heldObject][OBJ_Y];
		toX = position.x + PLAYER_SIZE;
		if (toX + w > screenW) toX = position.x - w;
		toY = position.y + PLAYER_SIZE - h;
	}
	/* --- animation of dropping --- */
	if (dropping) {
		carry_timer += dt;
		if (carry_timer <= 0.15) { player_action = CARRY;      HeldObject_MoveTo(0.0f); }
		else if (carry_timer <= 0.3) { player_action = GRABBING_2; HeldObject_MoveTo(0.5f); }
		else if (carry_timer <= 0.5) { player_action = GRABBING_1; HeldObject_MoveTo(1.0f); }
		else {
			dropping = 0;
			carry_timer = 0.0f;
			isCarrying = 0;
			player_action = STANDING;
			heldObject = -1;
		}
	}

	/* --- Horizontal: locked during the animation --- */
	velocity.x = 0.0f;
	if (!dropping) {
		if (CP_Input_KeyDown(KEY_A)) velocity.x -= MOVE_SPEED;
		if (CP_Input_KeyDown(KEY_D)) velocity.x += MOVE_SPEED;
	}

	/* --- Jump: only when standing on something, not during animation --- */
	if ((CP_Input_KeyTriggered(KEY_W) || CP_Input_KeyTriggered(KEY_SPACE)) && onGround && !dropping) {
		velocity.y = -JUMP_SPEED;
		onGround = 0;
	}

	/* --- Gravity: always applied, S makes you fall faster --- */
	velocity.y += GRAVITY * dt;
	if (CP_Input_KeyDown(KEY_S) && !onGround) {
		velocity.y += GRAVITY * dt;
	}
	if (velocity.y > MAX_FALL_SPEED) velocity.y = MAX_FALL_SPEED;

	/* --- Move --- */
	position = CP_Vector_Add(position, CP_Vector_Scale(velocity, dt));

	/* --- Ground check: snap to ground and stop falling --- */
	if (position.y + PLAYER_SIZE >= groundY) {
		position.y = groundY - PLAYER_SIZE;
		velocity.y = 0.0f;
		onGround = 1;
	}
	else {
		onGround = 0;
	}

	/* --- Keep player inside the screen horizontally --- */
	if (position.x < 0.0f) position.x = 0.0f;
	if (position.x + PLAYER_SIZE > screenW) position.x = screenW - PLAYER_SIZE;

	/* --- Held object follows the head while walking/jumping --- */
	if (!dropping) {
		current_objects[heldObject][OBJ_X] = position.x + (PLAYER_SIZE - current_objects[heldObject][OBJ_W]) / 2.0f;
		current_objects[heldObject][OBJ_Y] = position.y - current_objects[heldObject][OBJ_H];
	}
}

/* Pushes the player out of a platform, handles wall clinging, and returns the side hit (0-4). */
/*
collision_check

Returns: 0 = no hit, 1 = top, 2 = bottom, 3 = left side, 4 = right side

IF player is on ground
	RESET wall cling timer

GET platform edges from its center, width and height
GET player edges from its position
WORK OUT how far player is inside the platform from each side

IF player is not inside the platform
	IF wall climb is off
		RETURN 0
	IF player is not right next to a side of the platform
		RETURN 0
	SNAP player flush against that side
	SET side to left or right

ELSE IF player hit the top
	PUT player on top of platform, STOP falling
	SET on ground, RESET wall cling timer
	RETURN 1

ELSE IF player hit the bottom
	PUT player under platform, STOP moving up
	RETURN 2

ELSE IF player hit the left side
	PUSH player out to the left
	SET side to left

ELSE
	PUSH player out to the right
	SET side to right

IF wall climb is on
	IF this is a new wall
		RESET wall cling timer, SAVE this wall
	IF cling time is not used up
		ADD frame time to wall cling timer
		CANCEL gravity so player sticks to wall
		W or Space climbs up, S climbs down
		SET clinging and which side the wall is on

RETURN side
*/
int collision_check(float area_center_x, float area_center_y, float area_width, float area_height)
{
	float dt = CP_System_GetDt();
	const float WALL_STICK_DIST = 2.0f;   // how close (px) counts as "touching" a wall's side
	int side;

	// Standing on the floor or a platform -> cling time refills
	if (onGround) wall_timer = 0.0f;

	area_left = area_center_x - area_width / 2;
	area_right = area_center_x + area_width / 2;
	area_top = area_center_y - area_height / 2;
	area_bottom = area_center_y + area_height / 2;

	// Player edges (position is the player's top-left corner)
	player_left = position.x;
	player_right = position.x + PLAYER_SIZE;
	player_top = position.y;
	player_bottom = position.y + PLAYER_SIZE;

	// How far the player has gone into the platform from each side
	float overlap_top = player_bottom - area_top;
	float overlap_bottom = area_bottom - player_top;
	float overlap_left = player_right - area_left;
	float overlap_right = area_right - player_left;

	int overlapping = !(player_right <= area_left || player_left >= area_right ||
		player_bottom < area_top || player_top >= area_bottom);

	if (!overlapping)
	{
		// Not inside the platform. With wall climb on, still count it as
		// touching if the player is right next to a side (no need to hold A/D)
		if (wall_climb != 1) return 0;

		int vertical_overlap = (player_bottom > area_top && player_top < area_bottom);
		int near_left = (player_right <= area_left && player_right >= area_left - WALL_STICK_DIST);
		int near_right = (player_left >= area_right && player_left <= area_right + WALL_STICK_DIST);

		if (!vertical_overlap || !(near_left || near_right)) return 0;

		if (near_left)
		{
			position.x = area_left - PLAYER_SIZE;   // stay flush against the wall
			side = 3;
		}
		else
		{
			position.x = area_right;
			side = 4;
		}
	}
	// TOP: stop falling through, platform becomes the new ground
	else if (overlap_top <= overlap_bottom && overlap_top <= overlap_left && overlap_top <= overlap_right)
	{
		position.y = area_top - PLAYER_SIZE;
		if (velocity.y > 0) velocity.y = 0.0f;
		onGround = 1;
		wall_timer = 0.0f;
		return 1;
	}
	// BOTTOM: stop phasing up through the platform
	else if (overlap_bottom <= overlap_left && overlap_bottom <= overlap_right)
	{
		position.y = area_bottom;
		if (velocity.y < 0) velocity.y = 0.0f;
		return 2;
	}
	// LEFT SIDE: stop moving right into the platform
	else if (overlap_left <= overlap_right)
	{
		position.x = area_left - PLAYER_SIZE;
		side = 3;
	}
	// RIGHT SIDE: stop moving left into the platform
	else
	{
		position.x = area_right;
		side = 4;
	}

	// --- WALL CLIMB (only reached when touching a side) ---
	if (wall_climb == 1)
	{
		// Touching a new wall -> cling timer starts again
		if (area_center_x != cling_wall_x || area_center_y != cling_wall_y)
		{
			wall_timer = 0.0f;
			cling_wall_x = area_center_x;
			cling_wall_y = area_center_y;
		}

		if (wall_timer < WALL_CLING_TIME)
		{
			wall_timer += dt;

			// Undo this frame's gravity so the player sticks
			position.y -= velocity.y * dt;
			velocity.y = 0.0f;

			// Climb up / down the wall (no A/D needed)
			if (CP_Input_KeyDown(KEY_W) || CP_Input_KeyTriggered(KEY_SPACE)) position.y -= CLIMB_SPEED * dt;
			if (CP_Input_KeyDown(KEY_S)) position.y += CLIMB_SPEED * dt;

			is_clinging = 1;      // tell Draw_ClingBar to draw the bar
			cling_side = side;    // which side the wall is on
		}
	}

	return side;
}


#define CLING_BAR_W    8.0f   // bar thickness
#define CLING_BAR_GAP  5.0f   // space between player and bar

/* Draws the draining wall-cling endurance bar beside the player while clinging. */
/*
Draw_ClingBar

IF player is not clinging
	RETURN

WORK OUT how much cling time is left (1 = full, 0 = empty)
SET bar height based on time left

IF wall is on player's right
	PUT bar on player's left
ELSE
	PUT bar on player's right

DRAW grey background bar, full player height

IF more than 25% cling time left
	SET colour to green
ELSE
	SET colour to red
DRAW filled bar, shrinking from top to bottom

RESET clinging (collision_check sets it again next frame if still clinging)
*/
void Draw_ClingBar(void)
{
	if (!is_clinging) return;

	float remaining = 1.0f - (wall_timer / WALL_CLING_TIME);   // 1 = full, 0 = empty
	if (remaining < 0.0f) remaining = 0.0f;
	float barH = PLAYER_SIZE * remaining;

	// Bar goes on the side away from the wall
	// side 3 = wall is on the player's right -> bar on the left
	float barX = (cling_side == 3) ? position.x - CLING_BAR_GAP - CLING_BAR_W
		: position.x + PLAYER_SIZE + CLING_BAR_GAP;

	CP_Settings_Save();
	CP_Settings_RectMode(CP_POSITION_CORNER);
	CP_Settings_NoStroke();

	// Empty background, full player height
	CP_Settings_Fill(CP_Color_Create(60, 60, 60, 180));
	CP_Graphics_DrawRect(barX, position.y, CLING_BAR_W, PLAYER_SIZE);

	// Remaining endurance: drains from top to bottom
	if (remaining > 0.25f)
		CP_Settings_Fill(CP_Color_Create(80, 220, 100, 255));  // green
	else
		CP_Settings_Fill(CP_Color_Create(230, 60, 60, 255));   // red when almost out
	CP_Graphics_DrawRect(barX, position.y + (PLAYER_SIZE - barH), CLING_BAR_W, barH);

	CP_Settings_Restore();

	is_clinging = 0;   // collision_check sets it again next frame if still clinging
}

/* Returns 1 if a circle overlaps a rectangle (top-left x, y), 0 otherwise. */
/*
sphere_aabb_intersection

Returns: 1 = circle touches or overlaps box, 0 = no hit

FIND closest point on the box to the circle's center
	CLAMP circle's x between box's left and right
	CLAMP circle's y between box's top and bottom

GET distance from closest point to circle's center (squared)

IF distance is less than or equal to radius (squared)
	RETURN 1
RETURN 0
*/
int sphere_aabb_intersection(float cx, float cy, float radius, float x, float y, float w, float h)
{
	// Find the closest point on the platform to the sphere centre
	float closestX = cx;
	float closestY = cy;

	if (closestX < x)
		closestX = x;
	else if (closestX > x + w)
		closestX = x + w;

	if (closestY < y)
		closestY = y;
	else if (closestY > y + h)
		closestY = y + h;

	// Vector from closest point to sphere centre
	CP_Vector offset = CP_Vector_Subtract(
		CP_Vector_Set(cx, cy),
		CP_Vector_Set(closestX, closestY)
	);

	// Compare squared distances
	float distanceSquared =
		offset.x * offset.x + offset.y * offset.y;

	float radiusSquared = radius * radius;

	// Touching counts as intersection
	if (distanceSquared <= radiusSquared) {
		return 1;
	}
	return 0;
}

/* Draws a grey platform centered at (x, y) and checks player collision against it. */
/*
DrawPlatform

SET colour to grey
SET rectangle to draw from its center
DRAW platform with given size, rotation and rounded corners

CHECK player collision with this platform
*/
void DrawPlatform(float x, float y, float w, float h,
	float degrees, float cornerRadius)
{
	CP_Settings_Fill(grey);
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Graphics_DrawRectAdvanced(x, y, w, h, degrees, cornerRadius);

	//CP_Settings_ImageMode(CP_POSITION_CENTER);
	//CP_Image_Draw(platform, x, y, w, h, 255);
	//CP_Settings_ImageMode(CP_POSITION_CORNER);

	collision_check(x, y, w, h);

}

/* Draws the player sprite for the current player_action. */
/*
Player_Animation

CHECK player's current action
	STANDING   -> DRAW standing sprite
	RUNNING    -> DRAW standing sprite
	JUMPING    -> DRAW jump sprite
	GRABBING_1 -> DRAW grab sprite 1
	GRABBING_2 -> DRAW grab sprite 2
	CARRY      -> DRAW carry sprite

All sprites are drawn at the player's position and size
*/
void Player_Animation(void)
{
	switch (player_action)
	{
	case STANDING:
		CP_Image_Draw(Zilong, position.x, position.y, PLAYER_SIZE, PLAYER_SIZE, 255);
		break;

	case RUNNING:
		CP_Image_Draw(Zilong, position.x, position.y, PLAYER_SIZE, PLAYER_SIZE, 255);
		break;

	case JUMPING:
		CP_Image_Draw(jump, position.x, position.y, PLAYER_SIZE, PLAYER_SIZE, 255);
		break;

	case GRABBING_1:
		CP_Image_Draw(g1, position.x, position.y, PLAYER_SIZE, PLAYER_SIZE, 255);
		break;

	case GRABBING_2:
		CP_Image_Draw(g2, position.x, position.y, PLAYER_SIZE, PLAYER_SIZE, 255);
		break;

	case CARRY:
		CP_Image_Draw(carry, position.x, position.y, PLAYER_SIZE, PLAYER_SIZE, 255);
		break;
	}
}

/* Returns 1 if the player overlaps the goal rectangle (top-left x, y), 0 otherwise. */
int CheckWinCondition(float x, float y, float w, float h, float degrees, float cornerRadius)
{
	return (position.x + PLAYER_SIZE > x && position.x < x + w && position.y + PLAYER_SIZE > y && position.y < y + h);
}

// p will be assigned the address of position, to avoid pass by value
CP_Vector* p = &position;

/* Sets the player's position to the spawn point of the selected level. */
void Player_Starting_Pos(spawn arr[], int level_selected) {
	// When the address of p is updated, value stored within position is updated.
	*p = arr[level_selected].pos;
}

int unlocked_gate_score = 0;
int pressed;

/* Draws a pressure plate and its door; the door opens while the player or an object is on the plate. */
/*
Pressure_Plate_Door

GET pressure plate edges from its center, width and height
SET plate as not pressed

IF player overlaps the plate
	SET plate as pressed
FOR each object (until plate is pressed)
	IF object overlaps the plate
		SET plate as pressed

IF plate is pressed
	DRAW plate in dark green
ELSE
	DRAW plate in light green
	DRAW door (closed)
	CHECK player collision with door
*/
void Pressure_Plate_Door(float pres_x, float pres_y, float pres_width, float pres_height,
	float door_x, float door_y, float door_width, float door_height) {

	// if character or object intersects the pressure plate, the door will open.

		{
			pressed = 0;

			// Pressure plate edges
			float plate_left = pres_x - pres_width / 2.0f;
			float plate_right = pres_x + pres_width / 2.0f;
			float plate_top = pres_y - pres_height / 2.0f;
			float plate_bottom = pres_y + pres_height / 2.0f;

			// Player edges
			float player_left = position.x;
			float player_right = position.x + PLAYER_SIZE;
			float player_top = position.y;
			float player_bottom = position.y + PLAYER_SIZE;

			if (player_right > plate_left &&
				player_left < plate_right &&
				player_bottom > plate_top &&
				player_top < plate_bottom)
			{
				pressed = 1;
			}

			// check if any object is on the pressure plate
			for (int i = 0; i < current_obj_count && !pressed; i++)
			{
				float obj_left = current_objects[i][OBJ_X];
				float obj_right = obj_left + current_objects[i][OBJ_W];
				float obj_top = current_objects[i][OBJ_Y];
				float obj_bottom = obj_top + current_objects[i][OBJ_H];

				if (obj_right > plate_left &&
					obj_left < plate_right &&
					obj_bottom > plate_top &&
					obj_top < plate_bottom)
				{
					pressed = 1;
				}
			}

			if (pressed) {
				CP_Settings_Fill(medium_light_green);
			}
			else {

				CP_Settings_Fill(light_green);
			}
			CP_Graphics_DrawRect(pres_x, pres_y, pres_width, pres_height);


			if (!pressed)
			{

				CP_Settings_Fill(purple);
				CP_Graphics_DrawRect(door_x, door_y, door_width, door_height);

				collision_check(door_x, door_y, door_width, door_height);
			}
		}
}

/* Pressure plate that toggles two doors: pressing it opens door 1 and closes door 2. */
/*
Pressure_Plate_2Door

GET pressure plate edges from its center, width and height
SET plate as not pressed

IF player overlaps the plate
	SET plate as pressed
FOR each object (until plate is pressed)
	IF object overlaps the plate
		SET plate as pressed

IF plate is pressed
	DRAW door 2 (closed)
	CHECK player collision with door 2
	DRAW plate
ELSE
	DRAW plate in light green
	DRAW door 1 (closed)
	CHECK player collision with door 1
*/
void Pressure_Plate_2Door(float pres_x, float pres_y, float pres_width, float pres_height,
	float door1_x, float door1_y, float door1_width, float door1_height,
	float door2_x, float door2_y, float door2_width, float door2_height) {

	// if character or object intersects the pressure plate, the door will open.

		{
			pressed = 0;

			// Pressure plate edges
			float plate_left = pres_x - pres_width / 2.0f;
			float plate_right = pres_x + pres_width / 2.0f;
			float plate_top = pres_y - pres_height / 2.0f;
			float plate_bottom = pres_y + pres_height / 2.0f;

			// Player edges
			float player_left = position.x;
			float player_right = position.x + PLAYER_SIZE;
			float player_top = position.y;
			float player_bottom = position.y + PLAYER_SIZE;

			if (player_right > plate_left &&
				player_left < plate_right &&
				player_bottom > plate_top &&
				player_top < plate_bottom)
			{
				pressed = 1;
			}

			// check if any object is on the pressure plate
			for (int i = 0; i < current_obj_count && !pressed; i++)
			{
				float obj_left = current_objects[i][OBJ_X];
				float obj_right = obj_left + current_objects[i][OBJ_W];
				float obj_top = current_objects[i][OBJ_Y];
				float obj_bottom = obj_top + current_objects[i][OBJ_H];

				if (obj_right > plate_left &&
					obj_left < plate_right &&
					obj_bottom > plate_top &&
					obj_top < plate_bottom)
				{
					pressed = 1;
				}
			}

			if (pressed) {
				CP_Settings_Fill(medium_light_green);

				CP_Settings_Fill(purple);
				CP_Graphics_DrawRect(door2_x, door2_y, door2_width, door2_height);

				collision_check(door2_x, door2_y, door2_width, door2_height);
			}
			else {

				CP_Settings_Fill(light_green);
			}
			CP_Graphics_DrawRect(pres_x, pres_y, pres_width, pres_height);


			if (!pressed)
			{

				CP_Settings_Fill(purple);
				CP_Graphics_DrawRect(door1_x, door1_y, door1_width, door1_height);

				collision_check(door1_x, door1_y, door1_width, door1_height);
			}
		}
}

/*
kill_zone

SET rectangle to draw from its center
SET colour to red
DRAW kill zone

GET kill zone edges from its center, width and height
GET player edges from its position

IF player overlaps the kill zone
	GO to lose screen
*/
void kill_zone(float x, float y, float w, float h)
{
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Settings_Fill(red);
	CP_Graphics_DrawRect(x, y, w, h);

	// Kill zone edges
	float zone_left = x - w / 2.0f;
	float zone_right = x + w / 2.0f;
	float zone_top = y - h / 2.0f;
	float zone_bottom = y + h / 2.0f;

	// Player edges
	float player_left = position.x;
	float player_right = position.x + PLAYER_SIZE;
	float player_top = position.y;
	float player_bottom = position.y + PLAYER_SIZE;

	if (player_right > zone_left &&
		player_left < zone_right &&
		player_bottom > zone_top &&
		player_top < zone_bottom)
	{
		CP_Engine_SetNextGameState(Screen_Init, Lose_Screen, Screen_Exit);
		return;
	}
}

float platform_velocity = -100.0f;
float platform_move = 0.0f;

/*
platform_alternating_movement

GET frame time
ADD platform speed * frame time to platform's offset
SET platform's new x = start x + offset

IF platform reaches left boundary
	LOCK platform to left boundary
	FIX offset to match
	MOVE platform right
ELSE IF platform reaches right boundary
	LOCK platform to right boundary
	FIX offset to match
	MOVE platform left

DRAW platform at new x (also checks player collision)
*/
void platform_alternating_movement(float x, float y, float w, float h,
	float degrees, float cornerRadius,
	float left_boundary, float right_boundary) {
	float dt = CP_System_GetDt();

	// This code makes stuff move 
	platform_move += platform_velocity * dt;

	// new x with offset 
	float updated_x = x + platform_move;

	// Reverse direction at the left boundary
	if (updated_x <= left_boundary)
	{
		// lock the x to the left limit
		updated_x = left_boundary;
		// update offset with the remaining distance
		platform_move = updated_x - x;
		// makes thing go right
		platform_velocity = 100.0f;
	}
	// Reverse direction at the right boundary
	else if (updated_x >= right_boundary)
	{
		// lock the x to the right limit
		updated_x = right_boundary;
		// update offset with the remaining distance
		platform_move = updated_x - x;
		// makes thing go left
		platform_velocity = -100.0f;
	}
	// draw stuff
	DrawPlatform(updated_x, y, w, h, degrees, cornerRadius);
}


float kill_velocity = -100.0f;
float kill_move = 0.0f;

/*
kill_zone_alternating_movement

GET frame time
ADD kill zone speed * frame time to kill zone's offset
SET kill zone's new x = start x + offset

IF kill zone reaches left boundary
	LOCK kill zone to left boundary
	FIX offset to match
	MOVE kill zone right
ELSE IF kill zone reaches right boundary
	LOCK kill zone to right boundary
	FIX offset to match
	MOVE kill zone left

DRAW kill zone at new x (also checks if player touches it)
*/
void kill_zone_alternating_movement(float x, float y, float w, float h, float left_boundary, float right_boundary)
{
	float dt = CP_System_GetDt();

	// Move the kill zone
	kill_move += kill_velocity * dt;

	float updated_x = x + kill_move;

	// Reverse at left boundary
	if (updated_x <= left_boundary)
	{
		updated_x = left_boundary;
		kill_move = updated_x - x;
		kill_velocity = 100.0f;
	}

	// Reverse at right boundary
	else if (updated_x >= right_boundary)
	{
		updated_x = right_boundary;
		kill_move = updated_x - x;
		kill_velocity = -100.0f;
	}

	// Draw and check collision at updated position
	kill_zone(updated_x, y, w, h);
}