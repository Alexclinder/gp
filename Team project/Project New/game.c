#include "cprocessing.h"
#include "screen_seq.h"
#include "game_mechanics.h"

CP_Image jump;
CP_Image g1;
CP_Image g2;
CP_Image c;
CP_Image Zilong;
CP_Image background;
int status;

int onGround;
CP_Vector position;
CP_Vector velocity;

// For Resume Game Btn
void Game_Init(void)
{
	g1 = CP_Image_Load("Assets/Grab1.png");
	g2 = CP_Image_Load("Assets/Grab2.png");
	c = CP_Image_Load("Assets/Carry.png");
	background = CP_Image_Load("Assets/Background.png");
	jump = CP_Image_Load("Assets/Jumping.png");
	Zilong = CP_Image_Load("Assets/Main_Character.png");
	status = 1;
	CP_System_SetFrameRate(120.0f);

}

void Game_Reset(void)
{
	Game_Init();
	position = (CP_Vector){ 100,200 };
	velocity = (CP_Vector){ 0,0 };
	onGround = 0;
}


void Game_Update(void)
{
	CP_Graphics_ClearBackground(CP_Color_Create(200, 200, 200, 0));
	while (status == 1) {
		Character_MovementDefault(1);
		if (CP_Input_KeyTriggered(KEY_ESCAPE)) {
			CP_Engine_SetNextGameState(Screen_Init, Pause_Menu_Update, Screen_Exit);
			status = 0;
		}
		// if player wins, go to win screen
			// CP_Engine_SetNextGameState(Screen_Init, Win_Screen, Screen_Exit);
			// status = 0;
			
		// if player loses, go to Lose screen
			// CP_Engine_SetNextGameState(Screen_Init, Lose_Screen, Screen_Exit);
			// status = 0;

		return 0;
	}
}

void Game_Exit(void)
{

}