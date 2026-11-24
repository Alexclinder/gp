/*!
@file     demonBoss.c
@author   Paul David Sheeba Christobel Kezin David (david.p)
@module   csd1401f26 Software Engineering Project 1
@section  B
@date     27/10/26
@brief    Implements the demon boss state machine, movement, collision, drawing, and lifecycle functions.
*//*______________________________________________________________________*/

#include <stdio.h>      
#include <math.h>
#include "cprocessing.h"

#include "game.h"
#include "game_mechanics.h"
#include "elevator.h"
#include "screen_seq.h"
#include "Images.h"


/* Boss Settings */

#define BOSS_SIZE              150.0f

#define BOSS_HOVER_SPEED       200.0f
#define BOSS_ATTACK_SPEED      600.0f
#define BOSS_RETURN_SPEED      250.0f

#define BOSS_ATTACK_TIME       5.0f
#define BOSS_STOP_DISTANCE     70.0f

#define BOSS_DAMAGE_DISTANCE 100.0f // If player within this distance from boss damage taken

// Boss safe zone (top portion of screen)
#define BOSS_ZONE_LEFT         100.0f
#define BOSS_ZONE_TOP          50.0f
#define BOSS_ZONE_BOTTOM       155.0f

/* Boss States */

typedef enum
{
    BOSS_HOVER,
    BOSS_ATTACK,
    BOSS_RETURN

} BossState;

/* Boss Variables */

static float bossX;
static float bossY;

static float bossTimer = 0.0f;

static int bossHasHitPlayer = 0;

static float attackTargetX;
static float attackTargetY;

static float returnTargetX;
static float returnTargetY;

static BossState bossState = BOSS_HOVER;


/*
BOSS_MoveTowards

GET frame time
WORK OUT distance from boss to target

IF boss is not already at the target
    SET move amount = speed * frame time
    IF move amount is more than the distance left
        SET move amount to the distance left (don't overshoot)
    MOVE boss towards the target by move amount
*/
static void BOSS_MoveTowards(float targetX, float targetY, float speed)
{
    float dt = CP_System_GetDt();

    float dx = targetX - bossX;
    float dy = targetY - bossY;

    float distance = sqrtf(dx * dx + dy * dy);

    if (distance > 0.0f)
    {
        float move = speed * dt;

        // Prevent overshooting
        if (move > distance)
            move = distance;

        bossX += (dx / distance) * move;
        bossY += (dy / distance) * move;
    }
}


/*
BOSS_SetRandomPosition

GET screen width

WORK OUT the area the boss can move in
    (inside the boss zone)

PICK a random x and y inside that area
SET that as the boss's next target position
*/
static void BOSS_SetRandomPosition(void)
{
    float screenW = (float)CP_System_GetWindowWidth();

    int minX = (int)(BOSS_ZONE_LEFT + BOSS_SIZE / 2.0f);
    int maxX = (int)(screenW - BOSS_ZONE_LEFT - BOSS_SIZE / 2.0f);

    int minY = (int)(BOSS_ZONE_TOP + BOSS_SIZE / 2.0f);
    int maxY = (int)BOSS_ZONE_BOTTOM;

    returnTargetX = (float)CP_Random_RangeInt(minX, maxX);
    returnTargetY = (float)CP_Random_RangeInt(minY, maxY);
}


/*
BOSS_CheckCollision

Returns: 1 = player is within damage distance, 0 = not

WORK OUT distance from boss to player's top-left corner

IF distance is less than boss damage distance
    RETURN 1
RETURN 0
*/
static int BOSS_CheckCollision(void)
{
    float dx = bossX - position.x;
    float dy = bossY - position.y;

    float distance = sqrtf(dx * dx + dy * dy);

    if (distance < BOSS_DAMAGE_DISTANCE)
        return 1;

    return 0;
}


/*
BOSS_Init

SET boss state to hover
RESET boss timer

PICK a random position in the boss zone
PUT boss at that position
*/
void BOSS_Init(void)
{
    bossState = BOSS_HOVER;
    bossTimer = 0.0f;

    // Choose a random starting position
    BOSS_SetRandomPosition();

    bossX = returnTargetX;
    bossY = returnTargetY;
}



/*
BOSS_Movement

CHECK boss state
    HOVER
        WAIT for attack time
        LOCK ON to player's position, SET state to attack

    ATTACK
        MOVE boss towards locked position
        IF boss hits player AND shield is not active
            TAKE 25 off player health
            IF player health reaches 0
                GO to lose screen
        WHEN boss reaches stopping distance
            PICK a new random position, SET state to return

    RETURN
        MOVE boss towards new position
        WHEN boss reaches it
            SET state to hover
*/
void BOSS_Movement(void)
{
    float dt = CP_System_GetDt();

    // Player target point (player's left edge, half a player above the player's top)
    float playerX = position.x;
    float playerY = position.y - PLAYER_SIZE / 2.0f;

    float dx;
    float dy;
    float distance;

    switch (bossState)
    {

    case BOSS_HOVER: // Hovering state
    {
        // Wait for BOSS_ATTACK_TIME seconds
        bossTimer += dt;

        if (bossTimer >= BOSS_ATTACK_TIME)
        {
            // Lock onto player's current position
            attackTargetX = playerX;
            attackTargetY = playerY;

            bossHasHitPlayer = 0; // Reset hit detection for new attack

            bossTimer = 0.0f;
            bossState = BOSS_ATTACK;
        }

        break;
    }
    case BOSS_ATTACK: // Attacking state
    {
        dx = attackTargetX - bossX;
        dy = attackTargetY - bossY;

        distance = sqrtf(dx * dx + dy * dy);

        // Stop before reaching the player
        if (distance <= BOSS_STOP_DISTANCE + 1.0f)
        {
            BOSS_SetRandomPosition();

            bossState = BOSS_RETURN;
            break;
        }

        // Calculate movement for this frame
        float move = BOSS_ATTACK_SPEED * dt;

        // Prevent boss from getting too close
        float allowedMove = distance - BOSS_STOP_DISTANCE;

        if (move > allowedMove)
            move = allowedMove;

        // Move towards the locked player position
        bossX += (dx / distance) * move;
        bossY += (dy / distance) * move;

        // Check if boss hits player
        if (!bossHasHitPlayer && BOSS_CheckCollision())
        {
            bossHasHitPlayer = 1;

            // Only take damage if shield is not active
            if (!shieldActive)
            {
                playerHealth -= 25.0f;

                if (playerHealth <= 0.0f)
                {
                    playerHealth = 0.0f;
                    CP_Engine_SetNextGameState(Screen_Init, Lose_Screen, Screen_Exit);
                    return;
                }
            }
        }

        // Check whether boss has reached stopping distance
        if (distance - move <= BOSS_STOP_DISTANCE + 1.0f)
        {
            BOSS_SetRandomPosition();

            bossState = BOSS_RETURN;
        }

        break;
    }
    case BOSS_RETURN: // Returning state
    {
        // Move towards the new random position
        BOSS_MoveTowards(
            returnTargetX,
            returnTargetY,
            BOSS_RETURN_SPEED
        );

        dx = returnTargetX - bossX;
        dy = returnTargetY - bossY;

        distance = sqrtf(dx * dx + dy * dy);

        // Boss has reached its destination
        if (distance < 1.0f)
        {
            bossX = returnTargetX;
            bossY = returnTargetY;

            bossTimer = 0.0f;
            bossState = BOSS_HOVER;
        }

        break;
    }
    }
}

/*
BOSS_Draw

DRAW boss image at boss's position and size
*/
void BOSS_Draw(void)
{
    CP_Image_Draw(
        demonBoss,
        bossX,
        bossY,
        BOSS_SIZE,
        BOSS_SIZE,
        255
    );
}

/*
BOSS_Update

UPDATE boss movement
DRAW boss
*/
void BOSS_Update(void)
{
    BOSS_Movement();
    BOSS_Draw();
}

/*
BOSS_Exit

FREE boss image
FREE level images
*/
void BOSS_Exit(void)
{
    CP_Image_Free(demonBoss);
    FreeLevelImages();
}