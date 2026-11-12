#include <stdio.h>
#include <math.h>
#include "cprocessing.h"
#include "game.h"
#include "game_mechanics.h"
#include "level.h"
#include "colors.h"
#include "npc_mechanics.h"
#include "screen_seq.h"
#include "boss.h"

#define PLAT_H      24.0f
#define MAX_PLATS   32
#define SCROLL_Y    350.0f   /* player never goes above this on screen */
#define GAP_MIN     75
#define GAP_MAX     75       /* must stay under jump height (~83px) */
#define SHIFT_MAX   250      /* max sideways shift between platforms */

//Player Health
#define PLAYER_MAX_HEALTH 100.0f
#define BOSS_DAMAGETOPLAYER 100.0f
float playerHealth = PLAYER_MAX_HEALTH;

typedef struct {
    float x, y, w;           /* world coords, top-left */
} Plat;

static Plat  plats[MAX_PLATS];
static int   plat_count = 0;
static float camY = 0.0f;   /* gets more negative as you climb */
static float topY = 0.0f;   /* world y of the highest platform so far */
static float lastX = 700.0f;


static int platformDirection = 1;
static void Plats_Generate(void)
{
    float screenW = (float)CP_System_GetWindowWidth();
    float screenH = (float)CP_System_GetWindowHeight();

    while (topY > camY - screenH && plat_count < MAX_PLATS) {
        float w, x;

        topY -= (float)CP_Random_RangeInt(GAP_MIN, GAP_MAX);
        w = (float)(CP_Random_RangeInt(2, 5) * 64);

        /* Generate platform on the opposite side */
        if (platformDirection == 1) {
            x = lastX + SHIFT_MAX;
            platformDirection = -1;
        }
        else {
            x = lastX - SHIFT_MAX - w;
            platformDirection = 1;
        }

        /* Keep platform inside the screen */
        if (x < 0.0f)
            x = 0.0f;

        if (x + w > screenW)
            x = screenW - w;

        lastX = x + w / 2.0f;

        plats[plat_count].x = x;
        plats[plat_count].y = topY;
        plats[plat_count].w = w;

        plat_count++;
    }
}

static void Plats_Cull(void)
{
    float screenH = (float)CP_System_GetWindowHeight();
    int n = 0;

    for (int i = 0; i < plat_count; i++) {
        if (plats[i].y - camY < screenH + 100.0f)
            plats[n++] = plats[i];
    }
    plat_count = n;
}

void DrawPlayerHealth(void)
{
    float x = position.x + PLAYER_SIZE / 2.0f - 80.0f / 2.0f;   // centred over the player (bar width 80)
    float y = position.y - 18.0f;                               // 12 px above the player's head

    float frac = CP_Math_ClampFloat(playerHealth / PLAYER_MAX_HEALTH, 0.0f, 1.0f);

    CP_Settings_RectMode(CP_POSITION_CORNER);

    // empty/background part
    CP_Settings_Fill(CP_Color_Create(60, 60, 60, 255));
    CP_Graphics_DrawRect(x, y, 80.0f, 10.0f);

    // filled part
    CP_Settings_Fill(CP_Color_Create(50, 205, 50, 255));
    CP_Graphics_DrawRect(x, y, 80.0f * frac, 10.0f);
}

void Elevator_Init(void)
{
    float screenH = (float)CP_System_GetWindowHeight();

    /* reset generation state */
    camY = 0.0f;
    topY = screenH;      /* first platform sits just above the floor */
    // lastX = 100.0f;
    plat_count = 0;

    position = CP_Vector_Set(100.0f, screenH - PLAYER_SIZE);
    velocity = CP_Vector_Set(0.0f, 0.0f);
    onGround = 0;
    current_obj_count = 0;       /* no pickup objects in this level */
    ProjectileActive = 0;


    // Initialise boss AFTER player position
    BOSS_Init();
}

void Elevator_Update(void)
{
    DrawBG(bossBG);

    /* 2. scroll: shove player down, camera up */
    if (position.y < SCROLL_Y) {
        float d = SCROLL_Y - position.y;
        position.y += d;
        camY -= d;
    }

    /* 3. remove old platforms, make new ones above */
    Plats_Cull();
    Plats_Generate();

    /* 4. draw platforms */
    for (int i = 0; i < plat_count; i++) {
        DrawPlatform(plats[i].x + plats[i].w / 2.0f, plats[i].y - camY + PLAT_H / 2.0f, plats[i].w, PLAT_H, 0.0f, 0.0f);
        DrawPlatform(plats[i].x + plats[i].w / 2.0f + 600, plats[i].y - camY + PLAT_H / 2.0f, plats[i].w, PLAT_H, 0.0f, 0.0f);
        DrawPlatform(plats[i].x + plats[i].w / 2.0f - 600, plats[i].y - camY + PLAT_H / 2.0f, plats[i].w, PLAT_H, 0.0f, 0.0f);
    }

    //Draw Player health Bar
    DrawPlayerHealth();


    Objective_Text();
    ingameTimer();
    TeleportTimer();
    Teleport_Mechannic();

    if (isCarrying) Character_MovementCarry();
    else Character_MovementDefault();

    // 6. Boss movement and drawing
    BOSS_Update();

    Shield_Update();

    // Spawn in Enemy
    //NPC_MovementDefault();
    //NPC_Attack();
    //NPC_UpdateProjectiles(1);
    //NPC_CheckShieldCollision();
    //NPC_CheckReflectedHit();
    //NPC_DrawProjectiles();

    // Check for collision then end game if hit
    if (NPC_CheckPlayerCollision() == 1)
    {
        NPC_UpdateProjectiles(0);
        CP_Engine_SetNextGameState(Screen_Init, Lose_Screen, Screen_Exit);
        return;
    }

    Objects_Draw();
    //Player_Animation_Checks();
    Player_Animation();
    Draw_ClingBar();
    Shield_Draw();
}

void Elevator_Exit(void)
{
    BOSS_Exit();
}