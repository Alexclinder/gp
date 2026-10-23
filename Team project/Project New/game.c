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
<<<<<<< Updated upstream
<<<<<<< HEAD
CP_Image background2;
=======
CP_Image background2;

>>>>>>> Stashed changes
int status;
=======
>>>>>>> d122ac9bb0eab149c3d52d4a85aceabb2c1b96e8

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
	background2 = CP_Image_Load("Assets/Background2.png");
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
<<<<<<< HEAD
	CP_Graphics_ClearBackground(CP_Color_Create(200, 200, 200, 0));
	while (status == 1) {
		Character_MovementDefault();
<<<<<<< Updated upstream
=======
		CP_Graphics_ClearBackground(CP_Color_Create(200, 200, 200, 0));
	
		Character_MovementDefault();
		level_selection(level_selected);
				

>>>>>>> d122ac9bb0eab149c3d52d4a85aceabb2c1b96e8
=======
>>>>>>> Stashed changes
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