#pragma once

extern level_selected;
// Needed for Game_Reset() reassign back to 0
typedef struct{
	CP_Vector pos;
} spawn;

typedef enum {
	level_1,	// 0 
	level_2,	// 1
	level_3,	// 2
	level_4		// 3
} levels;

extern spawn player_spawns[4];
extern spawn NPC_1_spawns[4];

extern const float first_level_objects[2][5];
extern float first_level_objects_ingame[2][5];

extern const float second_level_objects[1][5];
extern float second_level_objects_ingame[1][5];

/* columns of an object row: { x, y, w, h, type } */
enum { OBJ_X, OBJ_Y, OBJ_W, OBJ_H, OBJ_TYPE };
/* columns of a platform row: { x, y, w, h, degrees, cornerRadius } */
enum { PLAT_X, PLAT_Y, PLAT_W, PLAT_H };
/* rows of level 1's objects, same order as first_level_objects */
enum { DESK, PRINTER };

extern float (*current_objects)[5];
extern int current_obj_count;
extern int heldObject;

void Objects_Reset(void);

int Object_Check(void);

void Objects_Draw(void);

void level_1st(void);

void level_2nd(void);

void level_3rd(void);

void level_4th(void);

int Object_HitCheck(float cx, float cy, float radius, int self);

void level_selection(int level_no);

void DrawExit(float x, float y, float w, float h, float degrees, float cornerRadius);

void DrawBG(CP_Image BG);

extern int TimeSet;
extern int PauseTimeCount;
extern int ResetTime;
