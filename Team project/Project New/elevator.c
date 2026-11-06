#include <stdio.h>
#include <math.h>
#include "cprocessing.h"

#define TILE_SIZE 64
#define MAP_ROWS  15
#define MAP_COLS  26

/* '#' = solid tile, '.' = empty */
static const char map[MAP_ROWS][MAP_COLS + 1] = {
    "..........#...............",
    "..........................",
    "..........................",
    "..........................",
    "..........#####...........",
    "..........................",
    "....#####.................",
    "..........................",
    "...............####.......",
    "..........###.............",
    "........#.................",
    "........#.........@.......",
    "........#.................",
    "........#.................",
    "##########################"
};

void Platformer_Draw(void) {
    for (int r = 0; r < MAP_ROWS; r++) {
        for (int c = 0; c < MAP_COLS; c++) {
            if (map[r][c] == '#') {
                CP_Settings_Fill(CP_Color_Create(80, 80, 80, 255));
                CP_Graphics_DrawRect((float)(c * TILE_SIZE), (float)(r * TILE_SIZE), (float)TILE_SIZE, (float)TILE_SIZE);
            }
            if (map[r][c] == '@') {
                CP_Settings_Fill(CP_Color_Create(0, 80, 80, 255));
                CP_Graphics_DrawRect((float)(c * TILE_SIZE), (float)(r * TILE_SIZE), (float)TILE_SIZE, (float)TILE_SIZE);
            }
        }
    }
}

#define PLAYER_SIZE    48.0f
#define MOVE_SPEED     300.0f
#define JUMP_SPEED     500.0f
#define GRAVITY        1500.0f
#define MAX_FALL_SPEED 1200.0f

typedef struct Body {
    CP_Vector pos, vel;
    float size;
    int onGround;
} Body;

static Body player;

/* Outside the map: solid on the left, right and bottom, empty above */
static int Tile_IsSolid(int c, int r) {
    if (c < 0 || c >= MAP_COLS) return 1;
    if (r < 0) return 0;
    if (r >= MAP_ROWS) return 1;
    return map[r][c] == '#';
}

/* Does the body's box overlap any solid tile? */
static int Body_HitsSolid(const Body* b) {
    int left = (int)floorf(b->pos.x / TILE_SIZE);
    int right = (int)floorf((b->pos.x + b->size - 0.01f) / TILE_SIZE);
    int top = (int)floorf(b->pos.y / TILE_SIZE);
    int bottom = (int)floorf((b->pos.y + b->size - 0.01f) / TILE_SIZE);
    for (int r = top; r <= bottom; r++)
        for (int c = left; c <= right; c++)
            if (Tile_IsSolid(c, r)) return 1;
    return 0;
}

static void Body_Step(Body* b, float moveDir, int wantJump, float dt) {
    b->vel.x = moveDir * MOVE_SPEED;
    if (wantJump && b->onGround) { b->vel.y = -JUMP_SPEED; b->onGround = 0; }
    b->vel.y += GRAVITY * dt;
    if (b->vel.y > MAX_FALL_SPEED) b->vel.y = MAX_FALL_SPEED;

    /* X axis */
    b->pos.x += b->vel.x * dt;
    if (Body_HitsSolid(b)) {
        b->pos.x -= b->vel.x * dt;
        b->vel.x = 0.0f;
    }

    /* Y axis */
    b->pos.y += b->vel.y * dt;
    b->onGround = 0;
    if (Body_HitsSolid(b)) {
        if (b->vel.y > 0.0f) b->onGround = 1;
        b->pos.y -= b->vel.y * dt;
        b->vel.y = 0.0f;
    }
}

void Elevator_Init(void)
{
    player.pos = CP_Vector_Set(100.0f, 500.0f);
    player.vel = CP_Vector_Set(0.0f, 0.0f);
    player.size = PLAYER_SIZE;
    player.onGround = 0;
}

void Elevator_Update(void)
{
    float dt = CP_System_GetDt();

    float moveDir = 0.0f;
    if (CP_Input_KeyDown(KEY_A)) moveDir -= 1.0f;
    if (CP_Input_KeyDown(KEY_D)) moveDir += 1.0f;
    int wantJump = CP_Input_KeyTriggered(KEY_W);

    Body_Step(&player, moveDir, wantJump, dt);

    CP_Graphics_ClearBackground(CP_Color_Create(30, 30, 40, 255));
    Platformer_Draw();

    CP_Settings_Fill(CP_Color_Create(220, 80, 80, 255));
    CP_Graphics_DrawRect(player.pos.x, player.pos.y, player.size, player.size);
}


void Elevator_Exit(void)
{

}
