#pragma once

extern level_selected;

// 'player_spawn': 'float [4][2]' differs in levels of indirection from 'int'
// This error occurs becoz i excluded [4][2] from player_spawn
extern player_spawn[4][2];


void level_1st(void);

void level_2nd(void);

void level_3rd(void);

void level_4th(void);

void level_selection(int level_no);

void DrawExit(float x, float y, float w, float h, float degrees, float cornerRadius);

void DrawBG(CP_Image BG);