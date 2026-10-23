#include "cprocessing.h"
#include "screen_seq.h"
#include "game_mechanics.h"
#include "level.h"
#include "game.h"

CP_Image jump;
CP_Image g1;
CP_Image g2;
CP_Image carry;
CP_Image Zilong;
CP_Image background;
CP_Image backgroundtwo;
CP_Image backgroundthree;

int status;

int onGround;
CP_Vector position;
CP_Vector velocity;

// For Resume Game Btn
void Game_Init(void)
{
	g1 = CP_Image_Load("Assets/Grab1.png");
	g2 = CP_Image_Load("Assets/Grab2.png");
	carry = CP_Image_Load("Assets/Carry.png");
	background = CP_Image_Load("Assets/Background.png");
	backgroundtwo = CP_Image_Load("Assets/Backgroundtwo.png");
	backgroundthree = CP_Image_Load("Assets/Backgroundthree.png");
	jump = CP_Image_Load("Assets/Jumping.png");
	Zilong = CP_Image_Load("Assets/Main_Character.png");
	CP_System_SetFrameRate(120.0f);

}

void Game_Reset(void)
{
	position = (CP_Vector){ 100,200 };
	velocity = (CP_Vector){ 0,0 };
	onGround = 0;

}


void Game_Update(void)
{
		Character_MovementDefault();

		level_selection(level_selected);
		if (CP_Input_KeyTriggered(KEY_ESCAPE)) {
			CP_Engine_SetNextGameState(Screen_Init, Pause_Menu_Update, Screen_Exit);

		}

		if (CheckWinCondition(1300, 300, 100, 100, 0, 10) == 1) {
			CP_Engine_SetNextGameState(Screen_Init, Win_Screen, Screen_Exit);
		}
		// if player wins, go to win screen
			// CP_Engine_SetNextGameState(Screen_Init, Win_Screen, Screen_Exit);
			// status = 0;

		// if player loses, go to Lose screen
			// CP_Engine_SetNextGameState(Screen_Init, Lose_Screen, Screen_Exit);
			// status = 0;
}


void Game_Next_Level(void) {

	if (level_selected >= 4) {
		CP_Engine_SetNextGameStateForced(Screen_Init, Completed_Game_Screen, Screen_Exit);
		return;
	}
	else {
		level_selected++;
	}
}

void Game_Exit(void)
{

}