#include <stdio.h>
#include <math.h>

#include "cprocessing.h"
#include "game.h"
#include "game_mechanics.h"
#include "boss.h"
#include <Windows.h>

// =====================================
// BOSS SETTINGS
// =====================================

#define BOSS_SIZE              150.0f
#define BOSS_DISTANCE          300.0f
#define BOSS_HOVER_SPEED       200.0f
#define BOSS_ATTACK_SPEED      600.0f
#define BOSS_ATTACK_TIME       5.0f
#define BOSS_STOP_DISTANCE    70.0f

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

static float debugTimer = 0.0f;


static float bossX;
static float bossY;

static float bossTimer = 0.0f;

static float attackTargetX;
static float attackTargetY;

static BossState bossState = BOSS_HOVER;


// =====================================
// MOVE TOWARDS TARGET
// =====================================

static void BOSS_Debug(void)
{
    debugTimer += CP_System_GetDt();

    if (debugTimer >= 1.0f)
    {
        debugTimer = 0.0f;

        char message[256];

        sprintf_s(
            message,
            sizeof(message),
            "Boss State: %d | Timer: %.2f | DT: %.4f | X: %.2f | Y: %.2f\n",
            bossState,
            bossTimer,
            CP_System_GetDt(),
            bossX,
            bossY
        );

        OutputDebugStringA(message);
    }
}

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
// INITIALISE BOSS
// =====================================

void BOSS_Init(void)
{
    bossState = BOSS_HOVER;
    bossTimer = 0.0f;

    // Start 700 pixels away from player
    bossX = position.x + BOSS_DISTANCE;
    bossY = position.y - PLAYER_SIZE / 2.0f;
}


// =====================================
// UPDATE BOSS MOVEMENT
// =====================================


void BOSS_Movement(void)
{
    float dt = CP_System_GetDt();

    float playerX = position.x;
    float playerY = position.y - PLAYER_SIZE / 2.0f;

    float dx;
    float dy;
    float distance;

    switch (bossState)
    {
    case BOSS_HOVER:
    {
        bossTimer += dt;

        // Wait for 5 seconds before attacking
        if (bossTimer >= BOSS_ATTACK_TIME)
        {
            attackTargetX = playerX;
            attackTargetY = playerY;

            bossTimer = 0.0f;
            bossState = BOSS_ATTACK;
        }

        break;
    }


    case BOSS_ATTACK:
    {
        dx = attackTargetX - bossX;
        dy = attackTargetY - bossY;

        distance = sqrtf(dx * dx + dy * dy);

        // Stop when close enough to the player
        if (distance <= BOSS_STOP_DISTANCE + 1.0f)
        {
            bossState = BOSS_RETURN;
            break;
        }

        // Calculate how far the boss can move this frame
        float move = BOSS_ATTACK_SPEED * dt;

        // Calculate the maximum distance it can move
        // without getting closer than the stop distance
        float allowedMove = distance - BOSS_STOP_DISTANCE;

        if (move > allowedMove)
            move = allowedMove;

        // Move directly towards the player's locked position
        bossX += (dx / distance) * move;
        bossY += (dy / distance) * move;

        // Check whether the boss has reached its stopping distance
        if (distance - move <= BOSS_STOP_DISTANCE + 1.0f)
        {
            bossState = BOSS_RETURN;
        }

        break;
    }


    case BOSS_RETURN:
    {
        // Direction from player to boss
        dx = bossX - playerX;
        dy = bossY - playerY;

        distance = sqrtf(dx * dx + dy * dy);

        if (distance < 0.001f)
        {
            dx = 1.0f;
            dy = 0.0f;
            distance = 1.0f;
        }

        // Find a position 300 pixels away
        float targetX = playerX +
            (dx / distance) * BOSS_DISTANCE;

        float targetY = playerY +
            (dy / distance) * BOSS_DISTANCE;

        // Move away from player
        BOSS_MoveTowards(
            targetX,
            targetY,
            BOSS_HOVER_SPEED
        );

        // Only start the next 5-second timer
        // once the boss has returned
        dx = targetX - bossX;
        dy = targetY - bossY;

        distance = sqrtf(dx * dx + dy * dy);

        if (distance < 5.0f)
        {
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
    BOSS_Debug();
    BOSS_Draw();
}


// =====================================
// EXIT BOSS
// =====================================

void BOSS_Exit(void)
{
    CP_Image_Free(&demonBoss);
}
