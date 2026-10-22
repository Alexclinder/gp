#include "Levels.h"
#include "cprocessing.h"
#include "game_mechanics.h"


CP_Vector Win_con_Box;
float Win_con_Height = 50.0f;
float Win_con_Width = 50.0f;



int level_1(float Player_XPos, float Player_YPos) {

	//Draws Win Condition Box
	CP_Settings_Fill(CP_Color_Create(0, 128, 0, 150));
	Win_con_Box = CP_Vector_Set(1500.0f, 700.0f);
	CP_Settings_RectMode(CP_POSITION_CORNER);
	CP_Graphics_DrawRect(Win_con_Box.x, Win_con_Box.y, Win_con_Width, Win_con_Height);
	CP_Settings_Fill(CP_Color_Create(0, 0, 128, 255));
	DrawPlatform(150.0f, 850.0f, 300.0f, 50.0f, 0, 0);
	DrawPlatform(500.0f, 800.0f, 400.0f, 50.0f, 0, 0);
	DrawPlatform(900.0f, 700.0f, 500.0f, 50.0f, 0, 0);
	if (Win_con(Win_con_Box.x, Win_con_Box.y, Win_con_Height, Win_con_Height,Player_XPos,Player_YPos,100.0f) ==1)
	return 1;
}

int level_2(float Player_XPos, float Player_YPos) {

	//Draws Win Condition Box
	CP_Settings_Fill(CP_Color_Create(0, 128, 0, 150));
	Win_con_Box = CP_Vector_Set(1000.0f, 500.0f);
	CP_Settings_RectMode(CP_POSITION_CORNER);
	CP_Graphics_DrawRect(Win_con_Box.x, Win_con_Box.y, Win_con_Width, Win_con_Height);
	CP_Settings_Fill(CP_Color_Create(0, 0, 128, 255));
	DrawPlatform(150.0f, 850.0f, 300.0f, 50.0f, 0, 0);
	DrawPlatform(500.0f, 800.0f, 400.0f, 50.0f, 0, 0);
	DrawPlatform(900.0f, 700.0f, 500.0f, 50.0f, 0, 0);
	if (Win_con(Win_con_Box.x, Win_con_Box.y, Win_con_Height, Win_con_Height, Player_XPos, Player_YPos, 100.0f) == 1)
		return 1;
}

int level_3(float Player_XPos, float Player_YPos) {

	//Draws Win Condition Box
	CP_Settings_Fill(CP_Color_Create(0, 128, 0, 150));
	Win_con_Box = CP_Vector_Set(500.0f, 300.0f);
	CP_Settings_RectMode(CP_POSITION_CORNER);
	CP_Graphics_DrawRect(Win_con_Box.x, Win_con_Box.y, Win_con_Width, Win_con_Height);
	CP_Settings_Fill(CP_Color_Create(0, 0, 128, 255));
	DrawPlatform(150.0f, 850.0f, 300.0f, 50.0f, 0, 0);
	DrawPlatform(500.0f, 800.0f, 400.0f, 50.0f, 0, 0);
	DrawPlatform(900.0f, 700.0f, 500.0f, 50.0f, 0, 0);
	DrawPlatform(600.0f, 500.0f, 300.0f, 50.0f, 0, 0);
	DrawPlatform(500.0f, 400.0f, 500.0f, 50.0f, 0, 0);
	if (Win_con(Win_con_Box.x, Win_con_Box.y, Win_con_Height, Win_con_Height, Player_XPos, Player_YPos, 100.0f) == 1)
		return 1;
}


int Level_Selection(int Level_Num, float Player_XPos, float Player_YPos) {
	int win;
	switch (Level_Num) {
	case 1:
		win = level_1(Player_XPos,  Player_YPos);
		break;
	case 2:
		win = level_2(Player_XPos,  Player_YPos);
		break;
	case 3:
		win = level_3(Player_XPos,  Player_YPos);
		break;
	default:
		break;
	}
	return win;
}

