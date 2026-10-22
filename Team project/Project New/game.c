#include "cprocessing.h"
#include "screen_seq.h"
#include "game_mechanics.h"
#include "Levels.h"

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
float PLAYER_SIZE = 100.0f;

// Use to change the Level Number
int level_num = 3; 
// Use to change the Level Number


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

		if (CP_Input_KeyTriggered(KEY_ESCAPE)) {
			CP_Engine_SetNextGameState(Screen_Init, Pause_Menu_Update, Screen_Exit);
			status = 0;
		}
		position.x = Character_MovementDefault_x();
		position.y = Character_MovementDefault_y();
	
		if (Level_Selection(level_num, position.x, position.y) == 1) {
			// Note since CharacterMovement has clear background in it's function and drawing before it would not appear
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