/* Find sin wee for any concerns or questions about the code.
@brief This files contains the coods for the platform
		Any interactables will be inside this file as well

struct initialization: https://www.geeksforgeeks.org/c/how-to-initialize-structures-in-c/
*/

#include "cprocessing.h"
#include "level.h"
#include "game_mechanics.h"
#include "game.h"
#include "screen_seq.h"

#define PARAMETERS 6
#define NO_ROTATION 0
#define CORNER_RADIUS 0
#define RECT_HORIZ_H 40

typedef struct {
	float x;
	float y;
	float w;
	float h;
	float degrees;
	float cornerRadius;
} win_condition ;

typedef enum{
	level_1 = 1,
	level_2,
	level_3,
	level_4
} levels;


int level_selected = level_1;

// Parameters is for drawing and checking win_con in level 1
win_condition level1_win = {1300, 300, 100, 100, NO_ROTATION, CORNER_RADIUS};

// LEVEL 1: Outside the building
float first_level[5][PARAMETERS] = {
	{ 180.0f, 800.0f, 360.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 500.0f, 740.0f, 180.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 760.0f, 670.0f, 180.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 1020.0f, 600.0f, 180.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 1370.0f, 530.0f, 300.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS }
};

// Parameters is for drawing and checking win_con in level 2
win_condition level2_win = { 1528.0f, 150.0f, 50.0f, 146.0f, NO_ROTATION, CORNER_RADIUS };

// LEVEL 2: Entering the building
float second_level[5][PARAMETERS] = {
	{ 0.0f, 250.0f, 630.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 570.0f, 530.0f, 240.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 940.0f, 460.0f, 240.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 1240.0f, 350.0f, 240.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 1500.0f, 260.0f, 220.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS }
};

// Parameters is for drawing and checking win_con in level 3
win_condition level3_win = { 1300, 300, 100, 100, NO_ROTATION, CORNER_RADIUS };

// LEVEL 3: Final Level
float third_level[6][PARAMETERS] = {
	{ 180.0f, 800.0f, 300.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 440.0f, 730.0f, 150.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 650.0f, 650.0f, 140.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 850.0f, 710.0f, 150.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 1050.0f, 620.0f, 150.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 1370.0f, 510.0f, 250.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS }
};

// Parameters is for drawing and checking win_con in level 4
win_condition level4_win = { 1300, 300, 100, 100, NO_ROTATION, CORNER_RADIUS };

// LEVEL 4: Infinite level??? lol
float fourth_level[6][PARAMETERS] = {
	{ 160.0f, 800.0f, 300.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 390.0f, 720.0f, 140.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 580.0f, 640.0f, 130.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 780.0f, 560.0f, 130.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 1000.0f, 470.0f, 150.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 1370.0f, 350.0f, 300.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS }
};


void level_1st(void) {
	DrawBG(background);

	if (isCarrying) Character_MovementCarry();
	else Character_MovementDefault();
	//Player_Animation_Checks();
	Player_Animation();
	// iterates thru the 4 rows and 6 col
	// Nested loop not needed as first level columns are iterated through.
	for (int i = 0; i < 4; i++) {
		DrawPlatform(first_level[i][0], 
					 first_level[i][1], 
					 first_level[i][2], 
			         first_level[i][3], 
			         first_level[i][4], 
			         first_level[i][5]);
	}
	DrawExit(level1_win.x, level1_win.y, level1_win.w, level1_win.h, level1_win.degrees, level1_win.cornerRadius);

	if (CheckWinCondition(level1_win.x, level1_win.y, level1_win.w, level1_win.h, level1_win.degrees, level1_win.cornerRadius) == 1) 
		CP_Engine_SetNextGameState(Screen_Init, Win_Screen, Screen_Exit);
}

void level_2nd(void) {
	DrawBG(backgroundtwo);

	if (isCarrying) Character_MovementCarry();
	else Character_MovementDefault();
	//Player_Animation_Checks();
	Player_Animation();

	// iterates thru the 4 rows and 6 col
	// Nested loop not needed as first level columns are iterated through.
	for (int i = 0; i < 5; i++) {
		DrawPlatform(second_level[i][0],
			second_level[i][1],
			second_level[i][2],
			second_level[i][3],
			second_level[i][4],
			second_level[i][5]);
	}
	DrawExit(level2_win.x, level2_win.y, level2_win.w, level2_win.h, level2_win.degrees, level2_win.cornerRadius);

	if (CheckWinCondition(level2_win.x, level2_win.y, level2_win.w, level2_win.h, level2_win.degrees, level2_win.cornerRadius) == 1)
		CP_Engine_SetNextGameState(Screen_Init, Win_Screen, Screen_Exit);
}

void level_3rd(void) {
	DrawBG(backgroundthree);

	if (isCarrying) Character_MovementCarry();
	else Character_MovementDefault();
	//Player_Animation_Checks();
	Player_Animation();
	// iterates thru the 4 rows and 6 col
	// Nested loop not needed as first level columns are iterated through.
	for (int i = 0; i < 6; i++) {
		DrawPlatform(third_level[i][0],
			third_level[i][1],
			third_level[i][2],
			third_level[i][3],
			third_level[i][4],
			third_level[i][5]);
	}
	DrawExit(level3_win.x, level3_win.y, level3_win.w, level3_win.h, level3_win.degrees, level3_win.cornerRadius);

	if (CheckWinCondition(level3_win.x, level3_win.y, level3_win.w, level3_win.h, level3_win.degrees, level3_win.cornerRadius) == 1)
		CP_Engine_SetNextGameState(Screen_Init, Win_Screen, Screen_Exit);
}


void level_4th(void) {
	DrawBG(backgroundthree); // Use background 3 as placeholder for level 4

	// iterates thru the 4 rows and 6 col
	// Nested loop not needed as first level columns are iterated through.
	for (int i = 0; i < 4; i++) {
		DrawPlatform(fourth_level[i][0],
			fourth_level[i][1],
			fourth_level[i][2],
			fourth_level[i][3],
			fourth_level[i][4],
			fourth_level[i][5]);
	}
	DrawExit(level4_win.x, level4_win.y, level4_win.w, level4_win.h, level4_win.degrees, level4_win.cornerRadius);

	if (CheckWinCondition(level4_win.x, level4_win.y, level4_win.w, level4_win.h, level4_win.degrees, level4_win.cornerRadius) == 1)
		CP_Engine_SetNextGameState(Screen_Init, Win_Screen, Screen_Exit);
}



void level_selection(int level_no) {
	switch (level_no) {
		case level_1: level_1st();
			break;
		case level_2: level_2nd();
			break;
		case level_3: level_3rd();
			break;
		case level_4: level_4th();
			break;
	}
}

void DrawExit(float x, float y, float w, float h, float degrees, float cornerRadius){
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Settings_Fill(CP_Color_Create(100, 200, 100, 255));
	CP_Graphics_DrawRectAdvanced(x, y, w, h, degrees, cornerRadius);
}

void DrawBG(CP_Image BG) {
	/* --- Draw --- */
	CP_Graphics_ClearBackground(CP_Color_Create(0, 100, 0, 100));
	CP_Settings_ImageMode(CP_POSITION_CORNER);
	CP_Image_Draw(BG, 0, 0, CP_System_GetWindowWidth(), CP_System_GetWindowHeight(), 255);
}