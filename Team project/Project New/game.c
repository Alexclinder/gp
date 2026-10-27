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
CP_Image platform;
CP_Image objImg[OBJ_TYPE_COUNT];
CP_Image objBG[OBJ_TYPE_COUNT];

//NPC
CP_Image npc1;

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
	npc1 = CP_Image_Load("Assets/NPC1.png"); // Load npc 1
	platform = CP_Image_Load("Assets/Platform.png");
	objImg[OBJ_DESK] = CP_Image_Load("Assets/desk.png");
	objBG[OBJ_DESK] = CP_Image_Load("Assets/desk_background.png");
	objImg[OBJ_PRINTER] = CP_Image_Load("Assets/printer.png");
	objBG[OBJ_PRINTER] = CP_Image_Load("Assets/printer_background.png");
	Objects_Reset();

	CP_System_SetFrameRate(120.0f);
	onGround = 0;

	

}

void Game_Reset(void)
{
	onGround = 0;
	Objects_Reset();
	isCarrying = 0;
	pickingUp = 0;
	dropping = 0;
	player_action = 1;
}


void Game_Update(void)
{		
		level_selection(level_selected);
		
		if (CP_Input_KeyTriggered(KEY_ESCAPE)) CP_Engine_SetNextGameState(Screen_Init, Pause_Menu_Update, Screen_Exit);


		// if player loses, go to Lose screen
			// CP_Engine_SetNextGameState(Screen_Init, Lose_Screen, Screen_Exit);
			// status = 0;
}


void Game_Next_Level(void) {

	if (level_selected > 3) {
		CP_Engine_SetNextGameStateForced(Screen_Init, Completed_Game_Screen, Screen_Exit);
	}
	else {
		level_selected++;
		Game_Reset();
		Objects_Reset();
		isCarrying = 0;
		pickingUp = 0;
		dropping = 0;

	}
}

void Game_Exit(void)
{
	for (int i = 0; i < OBJ_TYPE_COUNT; i++) {
		CP_Image_Free(&objImg[i]);
		CP_Image_Free(&objBG[i]);
	}
}