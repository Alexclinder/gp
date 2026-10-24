#include "cprocessing.h"
#include "game.h"
#include "game_mechanics.h"
#include "screen_seq.h"
#include "Physics.h"
#include "PlayerMechanics.h"
#include "Player's_Elements.h"

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
int onGround_status = 0;
int toggleGrab = 0;
float animationTimer = 0.0f;

/*Wall Climb Variable*/
float cling_wall_x = -1.0f;
float cling_wall_y = -1.0f;
float wall_timer = 0.0f;

void Character_MovementDefault(void) {
	float dt = CP_System_GetDt();
	float groundY = (float)CP_System_GetWindowHeight();
	float screenW = (float)CP_System_GetWindowWidth();
	/*
	if (CP_Input_KeyDown(KEY_SPACE)) {
		carry_timer += dt;
		if (carry_timer <= 3.0 && carry_timer <= 2.0){
			CP_Image_Draw(carry, position.x, position.y, PLAYER_SIZE, PLAYER_SIZE, 255);
		}
		else if (carry_timer < 2.0 && carry_timer <= 1.0) {
			CP_Image_Draw(g2, position.x, position.y, PLAYER_SIZE, PLAYER_SIZE, 255);
		}
		else if (carry_timer < 1.0 && carry_timer <= 0.0) {
			CP_Image_Draw(g1, position.x, position.y, PLAYER_SIZE, PLAYER_SIZE, 255);
		}
	}
	*/
	/* --- Horizontal: works on the ground AND in mid-air --- */
	velocity.x = 0.0f;
	if (CP_Input_KeyDown(KEY_A)) velocity.x -= MOVE_SPEED;
	if (CP_Input_KeyDown(KEY_D)) velocity.x += MOVE_SPEED;

	/* --- Jump: only when standing on something --- */
	if ((CP_Input_KeyTriggered(KEY_W)) && onGround_status) {
		velocity.y = -JUMP_SPEED;
		onGround_status = 0;
	}

	/* --- Gravity: always applied, S makes you fall faster --- */
	velocity.y += GRAVITY * dt;
	if (CP_Input_KeyDown(KEY_S) && !onGround_status) {
		velocity.y += GRAVITY * dt;
	}
	if (velocity.y > MAX_FALL_SPEED) velocity.y = MAX_FALL_SPEED;

	/* --- Move --- */
	position = CP_Vector_Add(position, CP_Vector_Scale(velocity, dt));

	/* --- Ground check: snap to ground and stop falling --- */
	if (position.y + PLAYER_SIZE >= groundY) {
		position.y = groundY - PLAYER_SIZE;
		velocity.y = 0.0f;
		onGround_status = 1;
	}
	else {
		onGround_status = 0;
	}

	/* --- Keep player inside the screen horizontally --- */
	if (position.x < 0.0f) position.x = 0.0f;
	if (position.x + PLAYER_SIZE > screenW) position.x = screenW - PLAYER_SIZE;

}

void Character_MovementCarry(void)
{

}

// dt is declared in default player movement
void Player_Animation_Checks(void) {

	float dt = CP_System_GetDt();
	// SPACE : Start the grabbing animation
	if (onGround_status && CP_Input_KeyTriggered(KEY_SPACE))
	{
		if (player_action == STANDING)
		{
			toggleGrab = 1;
			player_action = GRABBING_1;
			animationTimer = 0.0f;
		}
	}
	// G: Start reversing the animation
	if (onGround_status && CP_Input_KeyTriggered(KEY_G))
	{
		if (player_action == CARRY)
		{
			player_action = GRABBING_2;
			toggleGrab = 0;
			animationTimer = 0.0f;
		}
	}


	if (!onGround_status)
	{
		player_action = JUMPING;
		animationTimer = 0.0f;
	}

	// if player touches the ground and the previous action is jumping
	if (onGround_status == 0 && player_action == JUMPING) {
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

float Wall_Climb(float area_center_x, float area_center_y, float area_width, float area_height, int Wall_Climb_Status) {
	float dt = CP_System_GetDt();
	if (onGround_status) wall_timer = 0.0f;
	// --- WALL CLIMB (only reached when touching a side) ---
	if (Wall_Climb_Status == 1)
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
			;

		}
	}
}

