#include "cprocessing.h"
#include "screen_seq.h"
#include "game_mechanics.h"
#include "Win_Cond.h"

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
CP_Vector Win_con_Box;
float Win_con_Height = 50.0f;
float Win_con_Width = 50.0f;
float PLAYER_SIZE = 100.0f;


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
	Win_con_Box = (CP_Vector){1000 , 700};
	onGround = 0;
}


void Game_Update(void)
{
	CP_Graphics_ClearBackground(CP_Color_Create(200, 200, 200, 0));
	while (status == 1) {
		position.x=Character_MovementDefault_x();
		position.y=Character_MovementDefault_y();

		//Draws Win Condition Box
		CP_Settings_RectMode(CP_POSITION_CORNER);
		CP_Graphics_DrawRect(Win_con_Box.x, Win_con_Box.y,Win_con_Width, Win_con_Height);

		if (CP_Input_KeyTriggered(KEY_ESCAPE)) {
			CP_Engine_SetNextGameState(Screen_Init, Pause_Menu_Update, Screen_Exit);
			status = 0;
		}
		if (Win_con(Win_con_Box.x, Win_con_Box.y, Win_con_Width, Win_con_Height,position.x,position.y, PLAYER_SIZE) == 1) {
			CP_Engine_SetNextGameState(Screen_Init, Win_Screen, Screen_Exit);
			status = 0;
		}
		// if player loses, go to Lose screen
			// CP_Engine_SetNextGameState(Screen_Init, Lose_Screen, Screen_Exit);
			// status = 0;

		return 0;
	}
}

void Game_Exit(void)
{

}