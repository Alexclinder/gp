#include <stdio.h>
#include <math.h>

#include "cprocessing.h"
#include "game_mechanics.h"
#include "boss.h"

#include "level.h"
#include "game.h"
#include "screen_seq.h"
#include "npc_mechanics.h"
#include "colors.h"

#define PLAYER_WALK_SPEED 350.0f
#define DEPTH_MOVE_FACTOR 0.6f

#define BOSS_SPEED     140.0f
#define BOSS_W         220.0f
#define BOSS_H         220.0f
#define BOSS_STOP_DIST 60.0f

/* Wall ends at 225 of 540 in the background image */
#define FLOOR_TOP_FRAC (225.0f / 540.0f)

typedef struct
{
    float x, y, w, h;
} FloorRect;

/* Desk and flower pot collision areas */
static const FloorRect boss_obstacles[] =
{
    { 345.0f / 960.0f, 440.0f / 540.0f, 230.0f / 960.0f, 65.0f / 540.0f },
    {  28.0f / 960.0f, 250.0f / 540.0f,  44.0f / 960.0f, 27.0f / 540.0f },
    { 542.0f / 960.0f, 228.0f / 540.0f,  44.0f / 960.0f, 24.0f / 540.0f },
    { 904.0f / 960.0f, 250.0f / 540.0f,  44.0f / 960.0f, 26.0f / 540.0f }
};

#define OBSTACLE_COUNT ((int)(sizeof(boss_obstacles) / sizeof(boss_obstacles[0])))

CP_Image boss;
CP_Image bossBackground;

/* Boss position represents its feet (bottom centre) */
static float bossX, bossY;


// Check Player Collision with obstacles (flower pots, desk)
static int Player_IsOnObstacle(float fx, float fy)
{
    float W = (float)CP_System_GetWindowWidth();
    float H = (float)CP_System_GetWindowHeight();

    for (int i = 0; i < OBSTACLE_COUNT; i++)
    {
        float ox = boss_obstacles[i].x * W;
        float oy = boss_obstacles[i].y * H;
        float ow = boss_obstacles[i].w * W;
        float oh = boss_obstacles[i].h * H;

        if (fx > ox && fx < ox + ow && fy > oy && fy < oy + oh){
            return 1;
        }
    }

    return 0;
}



// Player Movement Just for Boss level
static void Boss_PlayerMovement(void)
{
    float dt = CP_System_GetDt();

    float W = (float)CP_System_GetWindowWidth();
    float H = (float)CP_System_GetWindowHeight();

    float dx = 0.0f;
    float dy = 0.0f;

    if (CP_Input_KeyDown(KEY_A)) dx -= 1.0f;
    if (CP_Input_KeyDown(KEY_D)) dx += 1.0f;
    if (CP_Input_KeyDown(KEY_W)) dy -= 1.0f;
    if (CP_Input_KeyDown(KEY_S)) dy += 1.0f;

    float nx = position.x + dx * PLAYER_WALK_SPEED * dt;
    float ny = position.y + dy * PLAYER_WALK_SPEED * DEPTH_MOVE_FACTOR * dt;

    /* Keep player inside the floor boundaries */
    nx = CP_Math_ClampFloat(nx, 0.0f, W - PLAYER_SIZE);
    ny = CP_Math_ClampFloat(ny, FLOOR_TOP_FRAC * H - PLAYER_SIZE, H - PLAYER_SIZE);

    /* Check horizontal movement */
    if (!Player_IsOnObstacle(nx + PLAYER_SIZE / 2.0f, position.y + PLAYER_SIZE))
    {
        position.x = nx;
    }

    /* Check vertical movement */
    if (!Player_IsOnObstacle(position.x + PLAYER_SIZE / 2.0f, ny + PLAYER_SIZE))
    {
        position.y = ny;
    }
}


// Boss Movement 
static void BOSS_Movement(void)
{
    float dt = CP_System_GetDt();

    /* Get player's feet position */
    float targetX = position.x + PLAYER_SIZE / 2.0f;
    float targetY = position.y + PLAYER_SIZE;

    /* Calculate direction towards player */
    float dx = targetX - bossX;
    float dy = targetY - bossY;

    /* Calculate distance */
    float distance = sqrtf(dx * dx + dy * dy);

    /* Stop when close enough */
    if (distance <= BOSS_STOP_DIST)
    {
        CP_Engine_SetNextGameState(Screen_Init, Win_Screen, Screen_Exit);
    }

    /* Normalize direction */
    dx /= distance;
    dy /= distance;

    /* Move boss towards player */
    bossX += dx * BOSS_SPEED * dt;
    bossY += dy * BOSS_SPEED * dt;
}


// Draw Boss
static void BOSS_Draw(void)
{
    CP_Settings_ImageMode(CP_POSITION_CORNER);
    CP_Image_Draw(boss,bossX - BOSS_W / 2.0f,bossY - BOSS_H,BOSS_W,BOSS_H,255);
}



// Load in boss and BG
void Boss_Init(void)
{
    boss = CP_Image_Load("Assets/BOSS.png");
    bossBackground = CP_Image_Load("Assets/bossbackground.png");

    float W = (float)CP_System_GetWindowWidth();
    float H = (float)CP_System_GetWindowHeight();

    /* Initialize player */
    position.x = W * 0.1f;
    position.y = H * 0.85f - PLAYER_SIZE;

    velocity.x = 0.0f;
    velocity.y = 0.0f;

    isCarrying = 0;
    pickingUp = 0;
    dropping = 0;
    player_action = 1;

    /* Initialize boss */
    bossX = W * 0.75f;
    bossY = H * 0.6f;
}



void Boss_Update(void)
{
    /* Draw background */
    CP_Graphics_ClearBackground(CP_Color_Create(0, 100, 0, 100));
    CP_Settings_ImageMode(CP_POSITION_CORNER);
    CP_Image_Draw(bossBackground, 0.0f, 0.0f, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight(), 255);

    /* Update movement */
    Boss_PlayerMovement();
    BOSS_Movement();

    /* Draw characters according to their depth */
    if (position.y + PLAYER_SIZE < bossY)
    {
        Player_Animation();
        BOSS_Draw();
    }
    else
    {
        BOSS_Draw();
        Player_Animation();
    }

    win_condition level3_win = { 1490.0f, 530.0f, 50.0f, 100.0f, 0.0f, 10.0f };

    // Added by Sin Wee for testing game state transition to leaderboard screen
    DrawExit(level3_win.x, level3_win.y, level3_win.w, level3_win.h, level3_win.degrees, level3_win.cornerRadius);

    if (CheckWinCondition(level3_win.x, level3_win.y, level3_win.w, level3_win.h, level3_win.degrees, level3_win.cornerRadius) == 1) {
        NPC_UpdateProjectiles(0);
        CP_Engine_SetNextGameState(Screen_Init, Leaderboard_Screen, Screen_Exit);
    }
}


void Boss_Exit(void)
{
    CP_Image_Free(&boss);
    CP_Image_Free(&bossBackground);
}
