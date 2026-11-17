#pragma once

void Screen_Init(void);
void Screen_Exit(void);
void Next_level_Init(void);
void Exit_Main_Menu_Init(void);

// Screens Sequence
void Main_Menu_Update(void);
void Pause_Menu_Update(void);
void Win_Screen(void);
void Lose_Screen(void);
void Leaderboard_Screen(void);
void Level_Select_Screen(void);
void Controls_Screen(void);

// Elements for the menu buttons
int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y);
void drawBtn(float x, float y, float w, float h, const char* text, float scalar, CP_Color color);
void drawTextCenter(const char* text, float x, float y, float scalar, CP_Color color);

// Dynamic button function to handle hover and click events
void dynamicBtn(const char* text, float rect_x, float rect_y, CP_Color color, void *init(void), void *update(void), void *exit(void));
void Exit_dynamicBtn(const char* text, float rect_x, float rect_y, CP_Color color);

// For Objective and score text
void drawTextTopLeft(const char* text, float x, float y, float scalar, CP_Color color);


