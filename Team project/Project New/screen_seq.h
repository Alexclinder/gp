#pragma once
//****************************Main Menu Functions*************************//
void Screen_Init(void);
void Main_Menu_Update(void);
void Screen_Exit(void);



//****************************Menu Button Functions*************************//

//Checks if mouse pointer in within square/rectangle parameters
int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y);

//Checks if mouse pointer in within circle parameters
int IsCircleClicked(float circle_center_x, float circle_center_y, float diameter, float click_x, float click_y);

// Dynamic button function to handle hover and click events
void dynamicBtn(const char* text, float y, void* init(void), void* update(void), void* exit(void));
void Exit_dynamicBtn(const char* text);

// Elements for the menu buttons
void drawBtn(float x, float y, float w, float h, const char* text, float scalar);
void drawTextCenter(const char* text, float x, float y, float scalar);



//****************************Special Menus & Win/Lose Screens*************************//
// Pause Menu
void Pause_Menu_Update(void);

// Win Screen & Next Level Sequence
void Win_Screen(void);
void Next_level_Init(void);

// Lose Screen
void Lose_Screen(void);



//****************************Not in Use Yet*************************//
void Leaderboard_Screen(void);
void Completed_Game_Screen(void);
void Level_Select_Screen(void);

