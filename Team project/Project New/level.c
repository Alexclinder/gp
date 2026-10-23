/* Find sin wee for any concerns or questions about the code.
@brief This files contains the coods for the platform
		Any interactables will be inside this file as well

*/

#include "cprocessing.h"
#include "level.h"
#include "game_mechanics.h"


typedef enum levels {
	level_1 = 1,
	level_2,
	level_3,
	level_4
} levels;

int level_selected = level_1;

// 4 rows & 6 columns
// first number in rows represent the number of platforms
// second number is the parameter numbers - do not change from 6!!!
float first_level[4][6] = {
						  {200, 800, 300, 30, 0, 10},
						  {700, 600, 30, 200, 0, 10},
						  {950, 600, 200, 30, 0, 10},
						  {1200, 500, 200, 30, 0, 10} };

float second_level[5][6] = {
						  { 200, 500, 300, 30, 0, 10 },
						  { 500, 400, 30, 200, 0, 10 },
						  { 750, 800, 200, 30, 0, 10 },
						  { 1500, 400, 200, 30, 0, 10 },
						  { 1100, 600, 200, 40, 0, 10 } };

float third_level[6][6] = {
						  { 200, 800, 300, 30, 0, 10 },
						  { 800, 600, 30, 200, 0, 10 },
						  { 450, 400, 200, 30, 0, 10 },
						  { 1000, 900, 200, 30, 0, 10 },
						  { 450, 400, 200, 30, 0, 10 }, 
						  { 550, 200, 400, 30, 0, 10 }	};

float fourth_level[4][6] = {
						  { 200, 800, 300, 30, 0, 10 },
						  { 700, 600, 40, 200, 0, 10 },
						  { 800, 500, 200, 30, 0, 10 },
						  { 1000, 400, 200, 30, 0, 10 } };


void level_1st(void) {
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
	DrawExit(1300, 300, 100, 100, 0, 10);
}

void level_2nd(void) {
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
	DrawExit(1300, 300, 100, 100, 0, 10);
}

void level_3rd(void) {
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
	DrawExit(1300, 300, 100, 100, 0, 10);
}

void level_4th(void) {
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
	DrawExit(1300, 300, 100, 100, 0, 10);
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

void DrawExit(float x, float y, float w, float h, float degrees, float cornerRadius)
{
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Settings_Fill(CP_Color_Create(100, 200, 100, 255));
	CP_Graphics_DrawRectAdvanced(x + w / 2.0f, y + h / 2.0f, w, h,degrees,cornerRadius);
}