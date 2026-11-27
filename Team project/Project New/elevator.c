/*!
@file     elevator.c
@author   Paul David Sheeba Christobel Kezin David (david.p)
@module   csd1401f26 Software Engineering Project 1
@section  B
@date     20/10/26
@brief    Implements the boss-level elevator section, including platforms, camera scrolling,
          lasers, the vent, and player health.
*//*______________________________________________________________________*/
#include <stdio.h>
#include <math.h>
#include "cprocessing.h"
#include "game.h"
#include "game_mechanics.h"
#include "level.h"
#include "colors.h"
#include "npc_mechanics.h"
#include "screen_seq.h"
#include "Images.h"
#include "demonBoss.h"


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
static int platformDirection = 1;  /* Sets whether the next platform will be on the right or left*/

// LASER SETTINGS
#define LASER_DELAY       10.0f
#define LASER_SPEED       30.0f
#define LASER_THICKNESS   5.0f
#define LASER_SCROLL_RATIO 0.3f /*controls how much the lasers move down when the camera scrolls up.*/

static float laserTimer = 0.0f;
static float laserY = 0.0f;
static int laserActive = 0;



// CLIMB TIMER
#define CLIMB_DURATION 30.0f

static float climbTimer = 0.0f;
static int platformsStopped = 0;

// VENT
static int ventVisible = 0;

#define VENT_WIDTH  150.0f
#define VENT_HEIGHT 150.0f

static float ventX = 0.0f;
static float ventY = 0.0f;

/*
Plats_Generate

WHILE highest platform is less than one screen above the camera
      AND there is room for more platforms
    MOVE up by the gap to get the new platform's height
    PICK a random width
    PUT platform on the opposite side from the last one
    KEEP platform inside the screen
    SAVE platform
*/
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

/*
Plats_Cull

FOR each platform
    IF platform is not far below the bottom of the screen
        KEEP it
REMOVE every other platform
*/
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


/*
Lasers_Update

IF laser is not active
    ADD frame time to laser timer
    IF laser timer reaches laser delay
        SET laser as active
        START laser at bottom of screen

IF laser is active
    MOVE laser up slowly
    MOVE laser down a bit when the camera scrolls up
*/
static void Lasers_Update(float cameraScroll)
{
    float dt = CP_System_GetDt();

    // Wait LASER_DELAY seconds before spawning
    if (!laserActive)
    {
        laserTimer += dt;

        if (laserTimer >= LASER_DELAY)
        {
            laserActive = 1;

            // Start at the bottom of the screen
            laserY = (float)CP_System_GetWindowHeight();
        }
    }

    if (laserActive)
    {
        // Slowly rise upwards
        laserY -= LASER_SPEED * dt;

        // Camera moves upwards, so laser moves slightly down
        laserY += cameraScroll * LASER_SCROLL_RATIO;
    }
}

/*
Lasers_Draw

IF laser is not active
    RETURN

SET colour to red
DRAW a dashed line across the screen at the laser's height
*/
static void Lasers_Draw(void)
{
    if (!laserActive)
        return;

    float screenW = (float)CP_System_GetWindowWidth();
    CP_Settings_Fill(red);

    // Horizontal laser lines
    for (int i = 0; i < screenW; i += 40)
    {
        CP_Graphics_DrawRect((float)i + 10.0f, laserY, 20.0f, 5.0f);
    }
}

/*
Laser_CheckCollision

Returns: 1 = player touches the laser, 0 = not touching

IF laser is not active
    RETURN 0

GET player's top and bottom
GET laser's top and bottom

IF player overlaps the laser
    RETURN 1
RETURN 0
*/
static int Laser_CheckCollision(void)
{
    if (!laserActive)
        return 0;

    float playerTop = position.y - PLAYER_SIZE / 2.0f;
    float playerBottom = position.y + PLAYER_SIZE / 2.0f;

    float laserTop = laserY - LASER_THICKNESS / 2.0f;
    float laserBottom = laserY + LASER_THICKNESS / 2.0f;

    // Check if player overlaps the horizontal laser
    if (playerBottom >= laserTop && playerTop <= laserBottom)
        return 1;

    return 0;
}


/*
DrawPlayerHealth

SET bar position centered above the player
WORK OUT how much health is left (0 to 1)

DRAW grey background bar
DRAW green bar, width based on health left
*/
void DrawPlayerHealth(void)
{
    float x = position.x + PLAYER_SIZE / 2.0f - 80.0f / 2.0f;   // centred over the player (bar width 80)
    float y = position.y - 18.0f;                               // 18 px above the player's head

    float frac = CP_Math_ClampFloat(playerHealth / PLAYER_MAX_HEALTH, 0.0f, 1.0f);

    CP_Settings_RectMode(CP_POSITION_CORNER);

    // empty/background part
    CP_Settings_Fill(CP_Color_Create(60, 60, 60, 255));
    CP_Graphics_DrawRect(x, y, 80.0f, 10.0f);

    // filled part
    CP_Settings_Fill(CP_Color_Create(50, 205, 50, 255));
    CP_Graphics_DrawRect(x, y, 80.0f * frac, 10.0f);
}

/*
Vent_CheckCollision

Returns: 1 = player touches the vent, 0 = not touching

IF vent is not visible
    RETURN 0

GET vent's position on screen

IF player overlaps the vent
    RETURN 1
RETURN 0
*/
static int Vent_CheckCollision(void)
{
    if (!ventVisible)
        return 0;

    float ventScreenY = ventY - camY;

    if (position.x + PLAYER_SIZE > ventX &&
        position.x < ventX + VENT_WIDTH &&
        position.y + PLAYER_SIZE > ventScreenY &&
        position.y < ventScreenY + VENT_HEIGHT)
    {
        return 1;
    }

    return 0;
}

/*
Vent_Draw

IF vent is not visible
    RETURN

DRAW vent image at its position on screen
*/
static void Vent_Draw(void)
{
    if (!ventVisible)
        return;
    CP_Settings_ImageMode(CP_POSITION_CORNER);
    CP_Image_Draw(
        vent,
        ventX,
        ventY - camY,
        VENT_WIDTH,
        VENT_HEIGHT,
        255
    );
}

/*
Vent_Init

PUT vent in the middle of the screen horizontally
PUT vent above the highest platform
*/
static void Vent_Init(void)
{
    float screenW = (float)CP_System_GetWindowWidth();

    // Centre the vent horizontally
    ventX = screenW / 2.0f - VENT_WIDTH / 2.0f;

    // Place vent above the highest generated platform
    ventY = topY - VENT_HEIGHT - 75.0f;
}



/*
Elevator_Reset

MOVE NPC to its spawn point
LOAD level images

RESET camera and platforms
PUT player at bottom left, STOP player moving
CLEAR objects and projectile
SET player health to full

RESET laser
RESET climb timer, platforms and vent
SET UP boss
*/
void Elevator_Reset(void)
{

    NPC_Starting_Pos(NPC_1_spawns, level_selected);
    LoadLevelImages();

    float screenH = (float)CP_System_GetWindowHeight();

    camY = 0.0f;
    topY = screenH;
    plat_count = 0;
    lastX = 700.0f;
    platformDirection = 1;

    position = CP_Vector_Set(100.0f, screenH - PLAYER_SIZE);
    velocity = CP_Vector_Set(0.0f, 0.0f);
    onGround = 0;
    current_obj_count = 0;
    ProjectileActive = 0;

    playerHealth = PLAYER_MAX_HEALTH;

    laserTimer = 0.0f;
    laserActive = 0;
    laserY = 0.0f;

    climbTimer = 0.0f;
    platformsStopped = 0;
    ventVisible = 0;
    ventX = 0.0f;
    ventY = 0.0f;

    BOSS_Init();
}

/*
Elevator_Init

MOVE NPC to its spawn point
RESET boss level
RESET timers (teleport charges set to unlimited)
*/
void Elevator_Init(void)
{
    NPC_Starting_Pos(NPC_1_spawns, level_selected);
    Elevator_Reset();
    ResetALLTimers_StatusBossLvl();

}

/*
Elevator_Update

DRAW background

IF platforms are still being made
    ADD frame time to climb timer
    IF climb timer reaches climb duration
        STOP making platforms
        SHOW vent above the highest platform

IF player goes above the scroll line
    PUSH player back down to it
    MOVE camera up by the same amount
UPDATE laser

REMOVE platforms far below the screen
IF platforms are still being made
    MAKE new platforms above

FOR each platform
    DRAW it, plus a copy 600px to the left and right
DRAW vent
DRAW player health bar
UPDATE and DRAW boss

IF NPC projectile or laser hits the player
    CLEAR projectile
    GO to lose screen
    RETURN

IF player reaches the vent
    GO to leaderboard screen
    RETURN

DRAW objects, player, wall cling bar and shield
DRAW laser
*/
void Elevator_Update(void)
{
    // Initialization for PosX, PosY & alpha can be found in level.c, declarations in level.h
    DrawBG(bossBG, PosX, PosY,
        (float)CP_System_GetWindowWidth(),
        (float)CP_System_GetWindowHeight(),
        alpha);
    // CLIMB TIMER
    if (!platformsStopped)
    {
        climbTimer += CP_System_GetDt();

        if (climbTimer >= CLIMB_DURATION)
        {
            climbTimer = CLIMB_DURATION;

            // Stop generating platforms
            platformsStopped = 1;

            // Make vent appear
            Vent_Init();
            ventVisible = 1;
        }
    }

    /* scroll: shove player down, camera up */
    float cameraScroll = 0.0f;

    if (position.y < SCROLL_Y)
    {
        float d = SCROLL_Y - position.y;

        position.y += d;
        camY -= d;

        cameraScroll = d;
    }
    Lasers_Update(cameraScroll);

    /* remove old platforms, make new ones above */
    Plats_Cull();

    if (!platformsStopped)
    {
        Plats_Generate();
    }

    /* draw platforms */
    for (int i = 0; i < plat_count; i++) {
        DrawPlatform(plats[i].x + plats[i].w / 2.0f, plats[i].y - camY + PLAT_H / 2.0f, plats[i].w, PLAT_H, 0.0f, 0.0f);
        DrawPlatform(plats[i].x + plats[i].w / 2.0f + 600, plats[i].y - camY + PLAT_H / 2.0f, plats[i].w, PLAT_H, 0.0f, 0.0f);
        DrawPlatform(plats[i].x + plats[i].w / 2.0f - 600, plats[i].y - camY + PLAT_H / 2.0f, plats[i].w, PLAT_H, 0.0f, 0.0f);
    }

    // Draw vent when visible
    Vent_Draw();

    //Draw Player health Bar
    DrawPlayerHealth();

    // Boss movement and drawing
    BOSS_Update();


    // Check for collision then end game if hit
    if (NPC_CheckPlayerCollision() == 1 || Laser_CheckCollision())
    {
        NPC_UpdateProjectiles(0);
        CP_Engine_SetNextGameState(Screen_Init, Lose_Screen, Screen_Exit);
        return;
    }

    // Check if player reaches the vent
    if (Vent_CheckCollision())
    {
        // Level 4 & Boss Level are integrated, so whatever score in level 4 is carried over to boss level.
        score_complete_level();
        CP_Engine_SetNextGameState(Screen_Init, Leaderboard_Screen, Screen_Exit);
        return;
    }

    Objects_Draw();
    Player_Animation();
    Draw_ClingBar();
    Shield_Draw();

    Lasers_Draw();
}

/*
Elevator_Exit

CLEAN UP boss
FREE boss image
FREE level images
*/
void Elevator_Exit(void)
{
    BOSS_Exit();
    CP_Image_Free(demonBoss);
    FreeLevelImages();
}