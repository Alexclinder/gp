#pragma once
// level.h - dont move out
/***************LEVEL****************/
extern float killZoneX;
// Needed for Game_Reset() reassign back to 0

//Win Condition structfor levels
typedef struct {
	float x;
	float y;
	float w;
	float h;
	float degrees;
	float cornerRadius;
} win_condition;

//Level selection enum
typedef enum {
	main_menu,	// 0
	level_1,	// 1 
	level_2,	// 2
	level_3,	// 3
	level_4,	// 4
	boss_level,	// 5
} levels;

extern level_selected;
void level_selection(int level_no);
/*****************BACKGROUND DRAWING FUNCTION****************/
void DrawBG(CP_Image BG);
/*****************EXIT DRAWING FUNCTION****************/
void DrawExit(float x, float y, float w, float h, float degrees, float cornerRadius);

void level_1st(void);

void level_2nd(void);

void level_3rd(void);

void level_4th(void);


/*******************Level Timer Variables************/
extern int TimeSet;
extern int PauseTimeStatus;
extern int ResetTime;
extern float WinScreenTime;
extern float Seconds;

extern struct LevelTimer {
	float level1_time;
	float level2_time;
	float level3_time;
}Level_Timer;

void SaveTime(float time);

void Objective_Text(void);



/*****************Spawn Locations for Player********************/
typedef struct{
	CP_Vector pos;
} spawn;
extern spawn player_spawns[6];

/*****************Spawn Locations for NPCs********************/
extern spawn NPC_1_spawns[6];

// objects.h
/*********************Object Related*****************/

extern const float first_level_objects[2][5];
extern float first_level_objects_ingame[2][5];

extern const float second_level_objects[2][5];
extern float second_level_objects_ingame[2][5];

extern const float third_level_objects[2][5];
extern float third_level_objects_ingame[2][5];

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

int Object_HitCheck(float cx, float cy, float radius, int self);




