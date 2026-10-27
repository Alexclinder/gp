#pragma once

extern level_selected;

struct spawn {
	CP_Vector player_pos;
};
// 'player_spawn': 'float [4][2]' differs in levels of indirection from 'int'
// This error occurs becoz i excluded [4][2] from player_spawn
extern struct spawn player_spawns[4][2];

extern const float first_level_objects[2][5];
extern float first_level_objects_ingame[2][5];

extern const float second_level_objects[1][5];
extern float second_level_objects_ingame[1][5];

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

void level_selection(int level_no);

void DrawExit(float x, float y, float w, float h, float degrees, float cornerRadius);

void DrawBG(CP_Image BG);

