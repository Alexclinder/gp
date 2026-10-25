#include "cprocessing.h"
#include "game.h"
#include "game_mechanics.h"
#include <math.h>


// NPC 
#define NPC_SIZE 100.0f
#define GRAVITY        1500.0f
#define MAX_FALL_SPEED 900.0f
#define MOVE_SPEED     150.0f

CP_Vector NPCposition = { 100, 200 };
CP_Vector NPCvelocity = { 0, 0 };
CP_Image npc1;
int NPConGround = 0;
int LeftOrRightState = 0;

//NPC Projectile 
#define PROJECTILE_SPEED 400.0f
#define PROJECTILE_SIZE  15.0f
#define SHOOT_INTERVAL   3.0f
#define MAX_PROJECTILES  20

typedef struct {
	CP_Vector Projposition;
	CP_Vector Projvelocity;
	float size;
	int active;
} Projectile;

Projectile shots[MAX_PROJECTILES] = { 0 };
float shootTimer = 0.0f;

void NPC_MovementDefault(void) {

	float dt = CP_System_GetDt();
	float groundY = (float)CP_System_GetWindowHeight();
	float screenW = (float)CP_System_GetWindowWidth();


	/* --- Gravity: always applied --- */
	NPCvelocity.y += GRAVITY * dt;
	if (NPCvelocity.y > MAX_FALL_SPEED) NPCvelocity.y = MAX_FALL_SPEED;

	/* --- Move --- */
	NPCposition = CP_Vector_Add(NPCposition, CP_Vector_Scale(NPCvelocity, dt));

	/* --- Ground check: snap to ground and stop falling --- */
	if (NPCposition.y + NPC_SIZE >= groundY) {
		NPCposition.y = groundY - NPC_SIZE;
		NPCvelocity.y = 0.0f;
		NPConGround = 1;
	}
	else {
		NPConGround = 0;
	}

	/* --- NPC CONSTANT MOVEMENT From Left to right --- */
	NPCvelocity.x = 0.0f;
	if (LeftOrRightState == 0 && NPConGround == 1) {
		NPCvelocity.x -= MOVE_SPEED;
	}
	if (LeftOrRightState == 1 && NPConGround == 1) {
		NPCvelocity.x += MOVE_SPEED;
	}

	if (NPCposition.x < 0.0f) LeftOrRightState = 1;
	if (NPCposition.x + NPC_SIZE > screenW)  LeftOrRightState = 0;

	NPCposition = CP_Vector_Add(NPCposition, CP_Vector_Scale(NPCvelocity, dt));

	/* --- Draw --- */

	CP_Settings_ImageMode(CP_POSITION_CORNER);
	CP_Image_Draw(npc1, NPCposition.x, NPCposition.y, NPC_SIZE, NPC_SIZE, 255);

}

void NPC_Attack(void)
{
	float dt = CP_System_GetDt();
	shootTimer += dt;

	if (shootTimer >= SHOOT_INTERVAL)
	{
		for (int i = 0; i < MAX_PROJECTILES; i++)
		{
			if (!shots[i].active)
			{
				// Calculate the centre of the NPC and player
				CP_Vector npcCenter = {
					NPCposition.x + NPC_SIZE / 2.0f,
					NPCposition.y + NPC_SIZE / 2.0f
				};

				CP_Vector playerCenter = {
					position.x + PLAYER_SIZE / 2.0f,
					position.y + PLAYER_SIZE / 2.0f
				};

				// Spawn the projectile at the NPC's centre
				shots[i].Projposition = npcCenter;

				// Aim towards the actual player's centre
				CP_Vector direction = CP_Vector_Normalize(
					CP_Vector_Subtract(playerCenter, npcCenter)
				);

				// Set projectile velocity
				shots[i].Projvelocity = CP_Vector_Scale(
					direction, PROJECTILE_SPEED
				);

				shots[i].active = 1;
				shootTimer = 0.0f;
				break;
			}
		}
	}
}

void NPC_UpdateProjectiles(void) {
	float dt = CP_System_GetDt();

	for (int i = 0; i < MAX_PROJECTILES; i++) {
		if (shots[i].active) {
			shots[i].Projposition = CP_Vector_Add(shots[i].Projposition, CP_Vector_Scale(shots[i].Projvelocity, dt));

			//Remove projecile when it leaves the screen 
			if (shots[i].Projposition.x < 0 || shots[i].Projposition.x > CP_System_GetDisplayWidth() || shots[i].Projposition.y < 0 || shots[i].Projposition.y > CP_System_GetWindowHeight()) {
				shots[i].active = 0;

			}
		}

		
	}
}

void NPC_DrawProjectiles(void) {
	CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));

	for (int i = 0; i < MAX_PROJECTILES; i++) {
		if (shots[i].active == 1) {
			CP_Graphics_DrawCircle(shots[i].Projposition.x, shots[i].Projposition.y, PROJECTILE_SIZE);
		}
	}
}

int NPC_CheckPlayerCollision(void) {
	for (int i = 0; i < MAX_PROJECTILES; i++) {
		if (shots[i].active) {
			float dx = shots[i].Projposition.x - (position.x + PLAYER_SIZE / 2.0f);
			float dy = shots[i].Projposition.y - (position.y + PLAYER_SIZE / 2.0f);

			float distance = sqrt(dx * dx + dy * dy);

			if (distance < PLAYER_SIZE / 2.0f + PROJECTILE_SIZE / 2.0f) {
				return 1;
			}
		}
	}
	return 0;
}