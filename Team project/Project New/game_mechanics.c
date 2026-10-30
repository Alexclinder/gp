#include "cprocessing.h"
#include "game.h"
#include "game_mechanics.h"
#include "screen_seq.h"
#include "level.h"
#include <stdio.h>
#include <math.h>

#define MOVE_SPEED     300.0f
#define JUMP_SPEED     500.0f
#define WALL_CLING_TIME 2.0f
#define CLIMB_SPEED     250.0f
#define GRAVITY        1500.0f
#define MAX_FALL_SPEED 900.0f
#define GRAB_FRAME_TIME    0.15f
#define KICK_DIST      100.0f
#define KICK_TIME      0.2f
#define KICK_DIST 100.0f   /* how far a kick moves the object */
#define KICK_TIME 0.2f     /* how long the slide takes, in seconds */

float collision_radius = 50.0f;
float degrees;
float cornerRadius;
float animationTimer = 0.0f;
int win = 0;
int wall_climb = 1;
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
CP_Vector position = { 100,100 };
CP_Vector velocity = { 0, 0 };


/* Player states */
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

/* Player variables */
int player_action = STANDING;
int onGround = 0;
int toggleGrab = 0;



int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y)
{
	// check if the click is within 
	// x bound of the area(check the x-axis radius from the centre of the rectangle(x coordinate - (width/2) && x coordinate + (width/2)))
	// y bound of the area(check the y-axis radius from the centre of the rectangle(y coordinate - (length/2) && x coordinate + (length/2)))
	if ((click_x >= area_center_x - area_width / 2 && click_x <= area_center_x + area_width / 2) &&
		(click_y >= area_center_y - area_height / 2 && click_y <= area_center_y + area_height / 2))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int IsCircleClicked(float circle_center_x, float circle_center_y, float diameter, float click_x, float click_y)
{
	// check if the click is within 
	// x bound of the area(check the x-axis radius from the centre of the circle(x coordinate - (diameter/2) && x coordinate + (diameter/2)))
	// y bound of the area(check the y-axis radius from the centre of the circle(y coordinate - (diameter/2) && y coordinate + (diameter/2)))
	if ((click_x >= circle_center_x - diameter / 2 && click_x <= circle_center_x + diameter / 2) &&
		(click_y >= circle_center_y - diameter / 2 && click_y <= circle_center_y + diameter / 2))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}



// t = 0 at start spot, 1 at end spot
void HeldObject_MoveTo(float t)
{
	current_objects[heldObject][0] = fromX + (toX - fromX) * t;
	current_objects[heldObject][1] = fromY + (toY - fromY) * t;
}

void Character_MovementDefault(void) {
	float dt = CP_System_GetDt();
	float groundY = (float)CP_System_GetWindowHeight();
	float screenW = (float)CP_System_GetWindowWidth();

	/* --- Space: pick up (grounded + touching an object, not mid-kick) --- */
	if (CP_Input_KeyTriggered(KEY_SPACE) && !pickingUp && kickedObject == -1 && onGround && (heldObject = Object_Check()) != -1) {
		pickingUp = 1;
		carry_timer = 0.0f;
		fromX = current_objects[heldObject][OBJ_X];
		fromY = current_objects[heldObject][OBJ_Y];
		toX = position.x + (PLAYER_SIZE - current_objects[heldObject][OBJ_W]) / 2.0f;
		toY = position.y - current_objects[heldObject][OBJ_H];
	}

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
	if (CP_Input_KeyTriggered(KEY_W) && onGround && !pickingUp) {
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

void Character_MovementCarry(void)
{
	float dt = CP_System_GetDt();
	float groundY = (float)CP_System_GetWindowHeight();
	float screenW = (float)CP_System_GetWindowWidth();

	/* --- G: drop (grounded only) --- */
	if (CP_Input_KeyTriggered(KEY_G) && !dropping && onGround) {
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
	if (CP_Input_KeyTriggered(KEY_W) && onGround && !dropping) {
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

int collision_check(float area_center_x, float area_center_y, float area_width, float area_height)
{
	float dt = CP_System_GetDt();

	// Standing on the floor or a platform -> cling time refills
	if (onGround) wall_timer = 0.0f;

	float area_left = area_center_x - area_width / 2;
	float area_right = area_center_x + area_width / 2;
	float area_top = area_center_y - area_height / 2;
	float area_bottom = area_center_y + area_height / 2;

	// Player edges (position is the player's top-left corner)
	float player_left = position.x;
	float player_right = position.x + PLAYER_SIZE;
	float player_top = position.y;
	float player_bottom = position.y + PLAYER_SIZE;

	// How far the player has gone into the platform from each side
	float overlap_top = player_bottom - area_top;
	float overlap_bottom = area_bottom - player_top;
	float overlap_left = player_right - area_left;
	float overlap_right = area_right - player_left;
	int side;

	// Not touching the platform
	if (player_right <= area_left || player_left >= area_right || player_bottom < area_top || player_top >= area_bottom)
	{
		return 0;
	}


	// TOP: stop falling through, platform becomes the new ground
	if (overlap_top <= overlap_bottom && overlap_top <= overlap_left && overlap_top <= overlap_right)
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
		// Touching a new wall -> fresh 3 seconds
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

			// Climb up / down the wall
			if (CP_Input_KeyDown(KEY_W)) position.y -= CLIMB_SPEED * dt;
			if (CP_Input_KeyDown(KEY_S)) position.y += CLIMB_SPEED * dt;
		}
	}

	return side;
}

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

void DrawPlatform(float x, float y, float w, float h,
	float degrees, float cornerRadius)
{
	CP_Settings_Fill(CP_Color_Create(100, 180, 255, 255));
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Graphics_DrawRectAdvanced(x, y, w, h, degrees, cornerRadius);

	CP_Settings_ImageMode(CP_POSITION_CENTER);
	CP_Image_Draw(platform, x, y, w, h, 255);
	CP_Settings_ImageMode(CP_POSITION_CORNER);

	collision_check(x, y, w, h);
}

// dt is declared in default player movement
void Player_Animation_Checks(void) {

	float dt = CP_System_GetDt();
	// SPACE : Start the grabbing animation
	if (onGround && CP_Input_KeyTriggered(KEY_SPACE))
	{
		if (player_action == STANDING)
		{
			toggleGrab = 1;
			player_action = GRABBING_1;
			animationTimer = 0.0f;
		}
	}

	// G: Start reversing the animation
	if (onGround && CP_Input_KeyTriggered(KEY_G))
	{
		if (player_action == CARRY)
		{
			player_action = GRABBING_2;
			toggleGrab = 0;
			animationTimer = 0.0f;
		}
	}

	
	if (!onGround)
	{
		player_action = JUMPING;
		animationTimer = 0.0f;
	}

	// if player touches the ground and the previous action is jumping
	if (onGround==0 && player_action == JUMPING) {
		player_action = STANDING;
	}

	// Animate grabbing or dropping
	// Both conditions is needed for player action
	if (player_action == GRABBING_1 || player_action == GRABBING_2)
	{
		animationTimer += dt;

		if (animationTimer >= GRAB_FRAME_TIME)
		{
			animationTimer = 0.0f;

			if (toggleGrab == 1)
			{
				/* Forward: grab */
				if (player_action == GRABBING_1)
				{
					player_action = GRABBING_2;
				}
				else if (player_action == GRABBING_2)
				{
					player_action = CARRY;
				}
			}
			else
			{
				/* Reverse: drop */
				if (player_action == GRABBING_2)
				{
					player_action = GRABBING_1;
				}
				else if (player_action == GRABBING_1)
				{
					player_action = STANDING;
					animationTimer = 0.0f;
				}
			}
		}
	}
}

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
		CP_Image_Draw(jump, position.x, position.y,PLAYER_SIZE, PLAYER_SIZE, 255);
		break;

	case GRABBING_1:
		CP_Image_Draw(g1, position.x, position.y,PLAYER_SIZE, PLAYER_SIZE, 255);
		break;

	case GRABBING_2:
		CP_Image_Draw(g2, position.x, position.y, PLAYER_SIZE, PLAYER_SIZE, 255);
		break;

	case CARRY:
		CP_Image_Draw(carry, position.x, position.y, PLAYER_SIZE, PLAYER_SIZE, 255);
		break;
	}
}

int CheckWinCondition(float x, float y, float w, float h, float degrees, float cornerRadius)
{
	return (position.x + PLAYER_SIZE > x && position.x < x + w && position.y + PLAYER_SIZE > y && position.y < y + h);
}

// Ask Sin Wee if dont understand

// p will be assigned the address of position, to avoid pass by value
CP_Vector *p = &position;

void Player_Starting_Pos(spawn arr[], int level_selected) {
	// When the address of p is updated, value stored within position is updated.
	*p = arr[level_selected].pos;
}


