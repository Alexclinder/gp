#include <stdio.h>
#include <math.h>
#include "cprocessing.h"

#include "game.h"
#include "game_mechanics.h"
#include "elevator.h"
#include "screen_seq.h"
#include "Images.h"



// =====================================
// BOSS SETTINGS
// =====================================

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

// =====================================
// BOSS STATES
// =====================================

typedef enum
{
    BOSS_HOVER,
    BOSS_ATTACK,
    BOSS_RETURN

} BossState;

// =====================================
// BOSS VARIABLES
// =====================================

static float bossX;
static float bossY;

static float bossTimer = 0.0f;

static int bossHasHitPlayer = 0;

static float attackTargetX;
static float attackTargetY;

static float returnTargetX;
static float returnTargetY;

static BossState bossState = BOSS_HOVER;


// =====================================
// MOVE TOWARDS TARGET
// =====================================

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


// =====================================
// SELECT RANDOM SAFE-ZONE POSITION
// =====================================

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

// =====================================
// CHECK COLLISION BETWEEN PLAYER AND BOSS
// =====================================

static int BOSS_CheckCollision(void)
{
    float dx = bossX - position.x;
    float dy = bossY - position.y;

    float distance = sqrtf(dx * dx + dy * dy);

    if (distance < BOSS_DAMAGE_DISTANCE)
        return 1;

    return 0;
}


// =====================================
// INITIALISE BOSS
// =====================================

void BOSS_Init(void)
{
    bossState = BOSS_HOVER;
    bossTimer = 0.0f;

    // Choose a random starting position
    BOSS_SetRandomPosition();

    bossX = returnTargetX;
    bossY = returnTargetY;
}


// =====================================
// UPDATE BOSS MOVEMENT
// =====================================

void BOSS_Movement(void)
{
    float dt = CP_System_GetDt();

    // Player centre
    float playerX = position.x;
    float playerY = position.y - PLAYER_SIZE / 2.0f;

    float dx;
    float dy;
    float distance;

    switch (bossState)
    {
        // =================================
        // HOVER STATE
        // =================================

    case BOSS_HOVER:
    {
        // Wait for 5 seconds
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

    // =================================
    // ATTACK STATE
    // =================================

    case BOSS_ATTACK:
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

    // =================================
    // RETURN STATE
    // =================================

    case BOSS_RETURN:
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


// =====================================
// DRAW BOSS
// =====================================

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


// =====================================
// UPDATE BOSS
// =====================================

void BOSS_Update(void)
{
    BOSS_Movement();
    BOSS_Draw();
}


// =====================================
// EXIT BOSS
// =====================================

void BOSS_Exit(void)
{
    CP_Image_Free(&demonBoss);
}
