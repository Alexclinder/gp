/*!
@file     npc_mechanics.c
@author   Paul David Sheeba Christobel Kezin David (david.p)
@module   csd1401f26 Software Engineering Project 1
@section  B
@date     23/09/26
@brief    Implements the NPC: movement, projectile attack, and the collision
          checks for the player, the shield and reflected projectiles.
*//*______________________________________________________________________*/
#include "cprocessing.h"
#include "game.h"
#include "game_mechanics.h"
#include "level.h"
#include "colors.h"
#include <math.h>
#include "scoring.h"


// NPC Stats
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
/*
NPC_MovementDefault

IF NPC is dead
    RETURN

GET frame time, ground height and screen width

APPLY gravity
LIMIT fall speed
MOVE NPC down by velocity

IF NPC is at or below ground
    SNAP to ground, STOP falling, SET on ground
ELSE
    SET not on ground

IF on ground
    MOVE left or right depending on direction

IF NPC reaches left edge of screen
    SET direction to right
IF NPC reaches right edge of screen
    SET direction to left

MOVE NPC sideways by velocity
DRAW NPC
*/
void NPC_MovementDefault(void)
{
    if (NPCalive == 0) return;      // dead NPC: no movement, no drawing

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


/*
NPC_Attack

IF NPC is dead
    RETURN

ADD frame time to shoot timer

IF shoot timer reaches shoot interval AND no projectile is out
    GET center of NPC and center of player
    SPAWN projectile at NPC's center
    AIM projectile at the player
    SET projectile as active and not reflected
    RESET shoot timer
*/
void NPC_Attack(void)
{
    if (NPCalive == 0) return;      // dead NPCs don't shoot

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
        ProjReflected = 0;          // new shot can hit the player again
        shootTimer = 0.0f;
    }
}


/*
NPC_UpdateProjectiles

check: 1 = update projectile, 0 = clear projectile

IF check is 1 AND projectile is active
    MOVE projectile by its velocity
    IF projectile leaves the screen
        REMOVE projectile
ELSE IF check is 0
    REMOVE projectile
    RESET shoot timer
*/
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


/*
NPC_DrawProjectiles

IF projectile is active
    DRAW red circle at projectile's position
*/
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


/*
NPC_CheckPlayerCollision

Returns: 1 = projectile hit the player, 0 = no hit

IF projectile is active AND not reflected
    GET distance from projectile to player's center
    IF distance is less than player radius + projectile radius
        REMOVE projectile
        RETURN 1

RETURN 0
*/
int NPC_CheckPlayerCollision(void)
{
    if (ProjectileActive == 1 && ProjReflected == 0)
    {
        float dx = Projposition.x - (position.x + PLAYER_SIZE / 2.0f);
        float dy = Projposition.y - (position.y + PLAYER_SIZE / 2.0f);

        float distance = sqrtf(dx * dx + dy * dy);

        if (distance < PLAYER_SIZE / 2.0f + PROJECTILE_SIZE / 2.0f)
        {
            ProjectileActive = 0;
            return 1;
        }
    }

    return 0;
}


/*
NPC_CheckShieldCollision

Returns: 1 = reflected, 2 = blocked, 0 = no hit

IF no projectile OR shield is off OR projectile already reflected
    RETURN 0

GET distance from projectile to shield's center

IF projectile touches the shield
    IF shield is still in parry window
        SEND projectile back the way it came
        SET projectile as reflected
        UNLOCK reflect score
        RETURN 1
    ELSE
        REMOVE projectile (shield blocks it)
        RETURN 2

RETURN 0
*/
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
            // Within the parry window: send it back the way it came
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


/*
NPC_CheckReflectedHit

Returns: 1 = reflected projectile hit the NPC, 0 = no hit

IF NPC is dead OR no projectile OR projectile not reflected
    RETURN 0

GET distance from projectile to NPC's center

IF projectile touches the NPC
    SET NPC as dead
    UNLOCK NPC kill score
    REMOVE projectile
    RETURN 1

RETURN 0
*/
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
        // unlock npc_score so the kill gives 300 points
        npc_score = 0;
        ProjectileActive = 0;   // projectile disappears too
        return 1;
    }


    return 0;
}


// npc_p points to NPCposition, so the spawn function can change it directly
CP_Vector* npc_p = &NPCposition;

/*
NPC_Starting_Pos

SET NPC's position to this level's spawn point
SET NPC as alive
*/
void NPC_Starting_Pos(spawn arr[], int level_selected) {
    // Updating *npc_p updates NPCposition
    *npc_p = arr[level_selected].pos;
    NPCalive = 1;               // NPC is alive again on level start
}