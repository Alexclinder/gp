#pragma once

void Screen_Init(void);
void Screen_Exit(void);
void Next_level_Init(void);

// Screens Sequence
void Main_Menu_Update(void);
void Pause_Menu_Update(void);
void Win_Screen(void);
void Lose_Screen(void);
void Leaderboard_Screen(void);
void Completed_Game_Screen(void);
void Level_Select_Screen(void);

// Dynamic button function to handle hover and click events
void dynamicBtn(const char* text, float rect_y, void *init(void), void *update(void), void *exit(void));
void Exit_dynamicBtn(const char* text, float rect_y);

// Elements for the menu buttons
void drawBtn(float y, float w, float h, const char* text, float scalar);
void drawTextCenter(const char* text, float x, float y, float scalar);