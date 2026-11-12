
#include "cprocessing.h"
#include "game.h"
#include "game_mechanics.h"
#include "level.h"
#include "colors.h"
#include <math.h>
#include "scoring.h"


// NPC
#define NPC_SIZE       100.0f
#define GRAVITY        1500.0f
#define MAX_FALL_SPEED 900.0f
#define MOVE_SPEED    150.0f

CP_Vector NPCposition = { 100, 200 };
CP_Vector NPCvelocity = { 0, 0 };
//NPC
CP_Image npc1;

int NPConGround = 0;
int LeftOrRightState = 0;
int NPCalive = 1;            // ADDED: 0 = NPC has been hit and is gone


// NPC Projectile
#define PROJECTILE_SPEED 400.0f
#define PROJECTILE_SIZE  15.0f
#define SHOOT_INTERVAL   3.0f

CP_Vector Projposition = { 0, 0 };
CP_Vector Projvelocity = { 0, 0 };

int ProjectileActive = 0;
int ProjReflected = 0;       // ADDED: 1 = projectile was parried, can't hurt player
float shootTimer = 0.0f;

// score is locked to prevent inf loop
int NPC_ded = 0;
int NPC_reflected = 0;

// NPC Movement
void NPC_MovementDefault(void)
{
    if (NPCalive == 0) return;      // ADDED: no movement, no drawing

    float dt = CP_System_GetDt();
    float groundY = (float)CP_System_GetWindowHeight();
    float screenW = (float)CP_System_GetWindowWidth();

    // Gravity
    NPCvelocity.y += GRAVITY * dt;

    if (NPCvelocity.y > MAX_FALL_SPEED)
    {
        NPCvelocity.y = MAX_FALL_SPEED;
    }

    // Vertical movement
    NPCposition = CP_Vector_Add(
        NPCposition,
        CP_Vector_Scale(NPCvelocity, dt)
    );

    // Ground check
    if (NPCposition.y + NPC_SIZE >= groundY)
    {
        NPCposition.y = groundY - NPC_SIZE;
        NPCvelocity.y = 0.0f;
        NPConGround = 1;
    }
    else
    {
        NPConGround = 0;
    }

    // Horizontal movement
    NPCvelocity.x = 0.0f;

    if (LeftOrRightState == 0 && NPConGround == 1)
    {
        NPCvelocity.x -= MOVE_SPEED;
    }

    if (LeftOrRightState == 1 && NPConGround == 1)
    {
        NPCvelocity.x += MOVE_SPEED;
    }

    // Change direction at screen edges
    if (NPCposition.x < 0.0f)
    {
        LeftOrRightState = 1;
    }

    if (NPCposition.x + NPC_SIZE > screenW)
    {
        LeftOrRightState = 0;
    }

    // Horizontal position update
    NPCposition = CP_Vector_Add(
        NPCposition,
        CP_Vector_Scale(NPCvelocity, dt)
    );

    // Draw NPC
    CP_Settings_ImageMode(CP_POSITION_CORNER);
    CP_Image_Draw(npc1, NPCposition.x, NPCposition.y, NPC_SIZE, NPC_SIZE, 255);
}


// NPC Attack
void NPC_Attack(void)
{
    if (NPCalive == 0) return;      // ADDED: dead NPCs don't shoot

    float dt = CP_System_GetDt();
    shootTimer += dt;

    if (shootTimer >= SHOOT_INTERVAL && ProjectileActive == 0)
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

        // Spawn projectile at NPC centre
        Projposition = npcCenter;

        // Calculate direction towards player
        CP_Vector direction = CP_Vector_Normalize(
            CP_Vector_Subtract(playerCenter, npcCenter)
        );

        // Set projectile velocity
        Projvelocity = CP_Vector_Scale(direction, PROJECTILE_SPEED);

        ProjectileActive = 1;
        ProjReflected = 0;          // ADDED: new shot is dangerous again
        shootTimer = 0.0f;
    }
}


// Update NPC Projectiles
void NPC_UpdateProjectiles(int check)
{
    float dt = CP_System_GetDt();

    if (check == 1 && ProjectileActive == 1)
    {
        // Move projectile
        Projposition = CP_Vector_Add(
            Projposition,
            CP_Vector_Scale(Projvelocity, dt)
        );

        // Remove projectile when it leaves the screen
        if (Projposition.x < 0 ||
            Projposition.x > CP_System_GetWindowWidth() ||
            Projposition.y < 0 ||
            Projposition.y > CP_System_GetWindowHeight())
        {
            ProjectileActive = 0;
        }
    }
    else if (check == 0)
    {
        // Remove projectile
        ProjectileActive = 0;

        // Reset shooting timer
        shootTimer = 0.0f;
    }
}


// Draw NPC Projectiles
void NPC_DrawProjectiles(void)
{
    if (ProjectileActive == 1)
    {
        CP_Settings_Fill(red);

        CP_Graphics_DrawCircle(
            Projposition.x,
            Projposition.y,
            PROJECTILE_SIZE
        );
    }
}


// Check Player Collision
int NPC_CheckPlayerCollision(void)
{
    if (ProjectileActive == 1 && ProjReflected == 0)   // ADDED: && ProjReflected == 0
    {
        float dx = Projposition.x - (position.x + PLAYER_SIZE / 2.0f);
        float dy = Projposition.y - (position.y + PLAYER_SIZE / 2.0f);

        float distance = sqrt(dx * dx + dy * dy);

        if (distance < PLAYER_SIZE / 2.0f + PROJECTILE_SIZE / 2.0f)
        {
            ProjectileActive = 0;
            return 1;
        }
    }

    return 0;
}


// ADDED: Check Shield Collision
// Returns 1 = reflected, 2 = blocked, 0 = no hit
int NPC_CheckShieldCollision(void)
{
    if (ProjectileActive == 0 || shieldActive == 0 || ProjReflected == 1)
        return 0;

    CP_Vector shieldCenter = {
        position.x + PLAYER_SIZE / 2.0f,
        position.y + PLAYER_SIZE / 2.0f
    };

    float distance = CP_Vector_Distance(Projposition, shieldCenter);

    if (distance < SHIELD_SIZE / 2.0f + PROJECTILE_SIZE / 2.0f)
    {
        if (shieldTimer <= SHIELD_PARRY_TIME)
        {
            // Within the 1 second window: send it back the way it came
            Projvelocity = CP_Vector_Negate(Projvelocity);
            ProjReflected = 1;
            NPC_reflected = (ProjReflected) ? 1 : 0;
            // if reflected, unlock reflected scoring to get 100 points
            reflect_score = 0;
            return 1;
        }
        else
        {
            // Too late: shield absorbs it
            ProjectileActive = 0;
            return 2;
        }
    }

    return 0;
}


// ADDED: Reflected projectile hits the NPC -> NPC disappears
int NPC_CheckReflectedHit(void)
{
    if (NPCalive == 0 || ProjectileActive == 0 || ProjReflected == 0)
        return 0;

    CP_Vector npcCenter = {
        NPCposition.x + NPC_SIZE / 2.0f,
        NPCposition.y + NPC_SIZE / 2.0f
    };

    float distance = CP_Vector_Distance(Projposition, npcCenter);

    if (distance < NPC_SIZE / 2.0f + PROJECTILE_SIZE / 2.0f)
    {
        NPCalive = 0;           // NPC disappears
        NPC_ded = (!NPCalive) ? 1 : 0;
        // unlock npc_score on subsequent kills
        npc_score = 0;
        ProjectileActive = 0;   // projectile disappears too
        return 1;
    }

   
    return 0;
}


// p will be assigned the address of position, to avoid pass by value
CP_Vector* npc_p = &NPCposition;

void NPC_Starting_Pos(spawn arr[], int level_selected) {
    // When the address of p is updated, value stored within position is updated.
    *npc_p = arr[level_selected].pos;
    NPCalive = 1;               // ADDED: NPC is alive again on level start
}