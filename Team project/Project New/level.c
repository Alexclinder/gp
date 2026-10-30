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
#include "npc_mechanics.h"
#include "colors.h"

<<<<<<< Updated upstream
=======
#define LEVEL1_PLATFORMS 30
#define LEVEL1_SEC_BUILDING 580.0f
#define LEVEL1_THIRD_BUILDING 280.0f
#define LEVEL1_FOURTH_BUILDING 400.0f
#define LEVEL1_BUILDING_WALL 10.0f
#define LEVEL1_FOURTH_BUILDING_WIDTH 3.0f

>>>>>>> Stashed changes
#define LEVELS 4
#define PARAMETERS 6
#define NO_ROTATION 0
#define CORNER_RADIUS 0
#define RECT_HORIZ_H 40
#define HIGHLIGHT_PAD 6.0f


typedef struct {
	float x;
	float y;
	float w;
	float h;
	float degrees;
	float cornerRadius;
} win_condition ;




int level_selected = level_1;
float Seconds, Zeroing_Time;
int TimeSet,PauseTimeCount, ResetTime;
float PauseingameTime = 0.0,differenceTime = 0.0;

spawn player_spawns[LEVELS] = { 
							   { 200.0f, 100.0f},
							   { 100.0f, 100.0f},
							   { 200.0f, 600.0f},
							   { 200.0f, 300.0f}
							  };

spawn NPC_1_spawns[LEVELS] = {
							   { 1030.0f, 200.0f },
							   { 1250.0f, 200.0f },
							   { 860.0f, 200.0f },
							   { 200.0f, 300.0f }
							 };

// Parameters is for drawing and checking win_con in level 1
win_condition level1_win = {1300, 300, 100, 100, NO_ROTATION, CORNER_RADIUS};

// LEVEL 1: Outside the building
<<<<<<< Updated upstream
float first_level[5][PARAMETERS] = {
	{ 180.0f, 800.0f, 360.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 500.0f, 740.0f, 180.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 760.0f, 670.0f, 180.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 1020.0f, 600.0f, 180.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 1370.0f, 530.0f, 300.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS }
=======
float first_level[LEVEL1_PLATFORMS][PARAMETERS] = {

	/* ---------- 1st building ----------  */

	// 260.0f is first building height
	{ 350.0f, 840.0f, PLATFORM_WIDTH_1, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 120.0f, 750.0f, PLATFORM_WIDTH_1, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 330.0f, 680.0f, PLATFORM_WIDTH_1 + 30.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 120.0f, 600.0f, PLATFORM_WIDTH_1, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 350.0f, 530.0f, PLATFORM_WIDTH_1, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 250.0f, 440.0f, PLATFORM_WIDTH_1, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 120.0f, 360.0f, PLATFORM_WIDTH_1, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	// first building roof
	{ 300.0f, 260.0f, 240.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },

	 /* ---------- 2nd building ----------  */

	// Left wall
	{ 545.0f, 610.0f, LEVEL1_BUILDING_WALL, LEVEL1_SEC_BUILDING, NO_ROTATION, CORNER_RADIUS },
	// Roof
	{ 660.0f, 320.0f, PLATFORM_WIDTH_1 * 2.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	// 5th floor
	{ 600.0f, 580.0f, PLATFORM_WIDTH_1 / 2.5f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },

	/* ---------- 2nd/3rd building ----------  */

	// Top Platform connecting 2nd and 3rd building 
	{ 850.0f, 450.0f, PLATFORM_WIDTH_1 / 3.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },

	// Bottom Platform connecting 2nd and 3rd building 
	{ 720.0f, 700.0f, PLATFORM_WIDTH_1 * 0.5f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },

	// Bottom Platform connecting 2nd and 3rd building 
	{ 880.0f, 650.0f, PLATFORM_WIDTH_1*1.5, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },

	
	/* ---------- 3rd building ----------  */

	// Left Wall
	{ 905.0f, 430.0f,  LEVEL1_BUILDING_WALL, LEVEL1_THIRD_BUILDING, NO_ROTATION, CORNER_RADIUS },

	// 4th Level
	{ 1000.0f, 500.0f,  LEVEL1_FOURTH_BUILDING_WIDTH, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 1100.0f, 730.0f,  LEVEL1_FOURTH_BUILDING_WIDTH, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 1100.0f, 730.0f,  LEVEL1_FOURTH_BUILDING_WIDTH, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 1100.0f, 730.0f,  LEVEL1_FOURTH_BUILDING_WIDTH, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 1100.0f, 730.0f,  LEVEL1_FOURTH_BUILDING_WIDTH, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 1100.0f, 730.0f,  LEVEL1_FOURTH_BUILDING_WIDTH, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 1100.0f, 730.0f,  LEVEL1_FOURTH_BUILDING_WIDTH, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },

	// Right Wall
	{ 1206.0f, 430.0f,  LEVEL1_BUILDING_WALL, LEVEL1_THIRD_BUILDING, NO_ROTATION, CORNER_RADIUS },

	/* ---------- 4th building ----------  */

	// Left Wall
	{ 1320.0f, 700.0f,  LEVEL1_BUILDING_WALL, LEVEL1_FOURTH_BUILDING, NO_ROTATION, CORNER_RADIUS },

	// Platform
	{ 1400.0f, 500.0f,  PLATFORM_WIDTH_1 * 1.5f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },

	
	
>>>>>>> Stashed changes
};

// Parameters is for drawing and checking win_con in level 2
win_condition level2_win = { 1528.0f, 150.0f, 50.0f, 146.0f, NO_ROTATION, CORNER_RADIUS };


// LEVEL 2: Entering the building
float second_level[5][PARAMETERS] = {
	{ 0.0f, 250.0f, 630.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 600.0f, 10.0f, 50.0f, 600.0f, NO_ROTATION, CORNER_RADIUS },
	{ 500.0f, 460.0f, 240.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
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

// Object start coords for level 1 (const so the level start never changes)
// { x, y, w, h, type }
const float first_level_objects[2][5] = {
	{ 120.0f, 690.0f, 120.0f, 90.0f, OBJ_DESK },
	{ 970.0f, 470.0f, 100.0f, 110.0f, OBJ_PRINTER }
};


// Object coords in game for level 1 (carrying and dropping change these)
float first_level_objects_ingame[2][5];


// Object start coords for level 2 (const so the level start never changes)
// { x, y, w, h, type }
const float second_level_objects[2][5] = {
	{ 120.0f, 690.0f, 120.0f, 90.0f, OBJ_DESK },
	{ 970.0f, 470.0f, 100.0f, 110.0f, OBJ_PRINTER }
};

// Object coords in game for level 2 (carrying and dropping change these)
float second_level_objects_ingame[2][5];

// Object start coords for level 3 (const so the level start never changes)
// { x, y, w, h, type }
const float third_level_objects[2][5] = {
	{ 120.0f, 690.0f, 120.0f, 90.0f, OBJ_DESK },
	{ 970.0f, 470.0f, 100.0f, 110.0f, OBJ_PRINTER }
};

// Object coords in game for level 3 (carrying and dropping change these)
float third_level_objects_ingame[2][5];


// Points to the in-game object array of the level being played
float (*current_objects)[5] = NULL;
int current_obj_count = 0;
int heldObject = -1;

void ingameTimer(void) {


	if (TimeSet == 1) {
		//differenceTime = Seconds - PauseingameTime;
		Font = CP_Font_Load("Assets/Exo2-Regular.ttf");
		CP_Font_Set(Font);
		CP_Settings_TextSize(100);
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_TOP);
		CP_Settings_Fill(translucent_white);
		Seconds = CP_System_GetSeconds()-Zeroing_Time+ PauseingameTime;

		char buffer[16] = { 0 };
		sprintf_s(buffer, 16, "%.2f", Seconds);
		CP_Font_DrawText(buffer, 0, 0);

	}
	else if (TimeSet == 0) {
		Zeroing_Time= CP_System_GetSeconds();
		TimeSet = 1;

	}
	if (PauseTimeCount == 1) {
		PauseingameTime = Seconds;
			PauseTimeCount = 0;
	}
	if (ResetTime == 1) {
		PauseingameTime = 0.0;
		ResetTime = 0;
	}
}

void level_1st(void) {
	DrawBG(background);

	ingameTimer();

	if (isCarrying) Character_MovementCarry();
	else Character_MovementDefault();

	//  Spawn in Enemy
	NPC_MovementDefault();
	NPC_Attack();
	NPC_UpdateProjectiles(1);
	NPC_DrawProjectiles();
	
	//Check For Collision then end game if hit
	if (NPC_CheckPlayerCollision()==1)
	{
		NPC_UpdateProjectiles(0);
		CP_Engine_SetNextGameState(Screen_Init,Lose_Screen,Screen_Exit);
		return;
	}
	else if (NPC_CheckPlayerCollision() == 2) {
		NPC_UpdateProjectiles(0);
	}

	Objects_Draw();
	//Player_Animation_Checks();
	Player_Animation();
	// iterates thru the 4 rows and 6 col
	// Nested loop not needed as first level columns are iterated through.
	for (int i = 0; i < 5; i++) {
		DrawPlatform(first_level[i][0], 
					 first_level[i][1], 
					 first_level[i][2], 
			         first_level[i][3], 
			         first_level[i][4], 
			         first_level[i][5]);
	}
	DrawExit(level1_win.x, level1_win.y, level1_win.w, level1_win.h, level1_win.degrees, level1_win.cornerRadius);

	if (CheckWinCondition(level1_win.x, level1_win.y, level1_win.w, level1_win.h, level1_win.degrees, level1_win.cornerRadius) == 1) {
		NPC_UpdateProjectiles(0);
		CP_Engine_SetNextGameState(Screen_Init, Win_Screen, Screen_Exit);
	}
		

	
}

void level_2nd(void) {
	DrawBG(backgroundtwo);

	if (isCarrying) Character_MovementCarry();
	else Character_MovementDefault();

	//  Spawn in Enemy
	NPC_MovementDefault();
	NPC_Attack();
	NPC_UpdateProjectiles(1);
	NPC_DrawProjectiles();
	//Check For Collision then end game if hit
	if (NPC_CheckPlayerCollision()==1)
	{
		NPC_UpdateProjectiles(0);
		CP_Engine_SetNextGameState(Screen_Init, Lose_Screen, Screen_Exit);
		return;
	}
	else if (NPC_CheckPlayerCollision() == 2) {
		NPC_UpdateProjectiles(0);
	}

	Objects_Draw();
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

	if (CheckWinCondition(level2_win.x, level2_win.y, level2_win.w, level2_win.h, level2_win.degrees, level2_win.cornerRadius) == 1) {
		NPC_UpdateProjectiles(0);
		CP_Engine_SetNextGameState(Screen_Init, Win_Screen, Screen_Exit);
	}
		
}

void level_3rd(void) {
	DrawBG(backgroundthree);

	if (isCarrying) Character_MovementCarry();
	else Character_MovementDefault();

	//  Spawn in Enemy
	NPC_MovementDefault();
	NPC_Attack();
	NPC_UpdateProjectiles(1);
	NPC_DrawProjectiles();
	//Check For Collision then end game if hit
	if (NPC_CheckPlayerCollision()==1)
	{
		NPC_UpdateProjectiles(0);
		CP_Engine_SetNextGameState(Screen_Init, Lose_Screen, Screen_Exit);
		return;
	}
	else if (NPC_CheckPlayerCollision() == 2) {
		NPC_UpdateProjectiles(0);
	}

	Objects_Draw();
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

	if (CheckWinCondition(level3_win.x, level3_win.y, level3_win.w, level3_win.h, level3_win.degrees, level3_win.cornerRadius) == 1) {
		NPC_UpdateProjectiles(0);
		CP_Engine_SetNextGameState(Screen_Init, Win_Screen, Screen_Exit);
	}
		
}


void level_4th(void) {
	DrawBG(backgroundthree); // Use background 3 as placeholder for level 4

	if (isCarrying) Character_MovementCarry();
	else Character_MovementDefault();

	//  Spawn in Enemy
	NPC_MovementDefault();
	NPC_Attack();
	NPC_UpdateProjectiles(1);
	NPC_DrawProjectiles();
	//Check For Collision then end game if hit
	if (NPC_CheckPlayerCollision()==1)
	{
		NPC_UpdateProjectiles(0);
		CP_Engine_SetNextGameState(Screen_Init, Lose_Screen, Screen_Exit);
		return;
	}
	else if (NPC_CheckPlayerCollision() == 2) {
		NPC_UpdateProjectiles(0);
	}

	Objects_Draw();
	Player_Animation();
	// iterates thru the 4 rows and 6 col
	// Nested loop not needed as first level columns are iterated through.
	for (int i = 0; i < 6; i++) {
		DrawPlatform(fourth_level[i][0],
			fourth_level[i][1],
			fourth_level[i][2],
			fourth_level[i][3],
			fourth_level[i][4],
			fourth_level[i][5]);
	}
	DrawExit(level4_win.x, level4_win.y, level4_win.w, level4_win.h, level4_win.degrees, level4_win.cornerRadius);

	if (CheckWinCondition(level4_win.x, level4_win.y, level4_win.w, level4_win.h, level4_win.degrees, level4_win.cornerRadius) == 1) {
		NPC_UpdateProjectiles(0);
		CP_Engine_SetNextGameState(Screen_Init, Win_Screen, Screen_Exit);
	}
}

void level_selection(int level_no) {
	switch (level_no) {
		case level_1: 
			level_1st();
			break;
		case level_2: 
			level_2nd();
			break;
		case level_3: 
			level_3rd();
			break;
		case level_4: 
			level_4th();
			break;
	}
}

void DrawExit(float x, float y, float w, float h, float degrees, float cornerRadius){
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Settings_Fill(medium_light_green);
	CP_Graphics_DrawRectAdvanced(x, y, w, h, degrees, cornerRadius);
}

void DrawBG(CP_Image BG) {
	/* --- Draw --- */
	CP_Graphics_ClearBackground(CP_Color_Create(0, 100, 0, 100));
	CP_Settings_ImageMode(CP_POSITION_CORNER);
	CP_Image_Draw(BG, 0.0f, 0.0f, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight(), 255);
}
// Copies the level's start coords into its in-game array. Call on level start/restart
void Objects_Reset(void) {
	current_objects = NULL;
	current_obj_count = 0;
	heldObject = -1;
	kickedObject = -1;   /* stop any kick that was still sliding */

	switch (level_selected) {
	case level_1:
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 5; j++)
				first_level_objects_ingame[i][j] = first_level_objects[i][j];
		current_objects = first_level_objects_ingame;
		current_obj_count = 2;
		break;
	case level_2:
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 5; j++)
				second_level_objects_ingame[i][j] = second_level_objects[i][j];
		current_objects = second_level_objects_ingame;
		current_obj_count = 2;
		break;
	case level_3:
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 5; j++)
				third_level_objects_ingame[i][j] = third_level_objects[i][j];
		current_objects = third_level_objects_ingame;
		current_obj_count = 2;
		break;
	}
}

// Returns the object the player is touching, or -1. Skips the held one
int Object_Check(void) {
	float cx = position.x + PLAYER_SIZE / 2.0f;
	float cy = position.y + PLAYER_SIZE / 2.0f;
	float radius = PLAYER_SIZE / 2.0f;

	for (int i = 0; i < current_obj_count; i++) {
		if (i == heldObject) continue;
		if (sphere_aabb_intersection(cx, cy, radius,
			current_objects[i][0], current_objects[i][1],
			current_objects[i][2], current_objects[i][3])) {
			return i;
		}
	}
	return -1;
}

void Objects_Draw(void) {
	int target = (!isCarrying && !pickingUp) ? Object_Check() : -1;
	float dt = CP_System_GetDt();

	/* platforms of the level being played */
	float (*plats)[PARAMETERS] = first_level;
	int plat_count = 5;
	if (level_selected == level_2) { plats = second_level; plat_count = 5; }
	if (level_selected == level_3) { plats = third_level;  plat_count = 6; }
	if (level_selected == level_4) { plats = fourth_level; plat_count = 6; }

	for (int i = 0; i < current_obj_count; i++) {
		int type = (int)current_objects[i][OBJ_TYPE];

		/* --- Fall when not being carried, until it hits something --- */
		if (i != heldObject) {
			float x = current_objects[i][OBJ_X];
			float w = current_objects[i][OBJ_W];
			float h = current_objects[i][OBJ_H];
			float nextY = current_objects[i][OBJ_Y] + 400.0f * dt;   /* fall speed: 400 px/s */
			float floor = (float)CP_System_GetWindowHeight();         /* ground */

			/* closest platform top under the object's middle */
			for (int p = 0; p < plat_count; p++) {
				float top = plats[p][PLAT_Y] - plats[p][PLAT_H] / 2.0f;
				if (x + w > plats[p][PLAT_X] - plats[p][PLAT_W] / 2.0f &&
					x < plats[p][PLAT_X] + plats[p][PLAT_W] / 2.0f &&
					current_objects[i][OBJ_Y] + h / 2.0f <= top && top < floor)
					floor = top;
			}

			if (nextY + h >= floor) current_objects[i][OBJ_Y] = floor - h;   /* hit: sit on top */
			else current_objects[i][OBJ_Y] = nextY;                         /* keep falling */
		}

		/* --- Draw: highlight behind (only when touching), then the object --- */
		CP_Image_Draw(objBG[type],
			current_objects[i][OBJ_X] - HIGHLIGHT_PAD,
			current_objects[i][OBJ_Y] - HIGHLIGHT_PAD,
			current_objects[i][OBJ_W] + HIGHLIGHT_PAD * 2,
			current_objects[i][OBJ_H] + HIGHLIGHT_PAD * 2,
			(i == target) ? 255 : 0);

		CP_Image_Draw(objImg[type],
			current_objects[i][OBJ_X], current_objects[i][OBJ_Y],
			current_objects[i][OBJ_W], current_objects[i][OBJ_H], 255);
	}
}

// Returns 0 = no hit, 1 = platform, 2 = other object, 3 = ground
int Object_HitCheck(float cx, float cy, float radius, int self) {
	/* platforms of the level being played */
	float (*plats)[PARAMETERS] = first_level;
	int plat_count = 5;
	if (level_selected == level_2) { plats = second_level; plat_count = 5; }
	if (level_selected == level_3) { plats = third_level;  plat_count = 6; }
	if (level_selected == level_4) { plats = fourth_level; plat_count = 6; }

	for (int p = 0; p < plat_count; p++) {
		if (sphere_aabb_intersection(cx, cy, radius,
			plats[p][PLAT_X] - plats[p][PLAT_W] / 2.0f,
			plats[p][PLAT_Y] - plats[p][PLAT_H] / 2.0f,
			plats[p][PLAT_W], plats[p][PLAT_H]))
			return 1;
	}

	for (int i = 0; i < current_obj_count; i++) {
		if (i == self) continue;
		if (sphere_aabb_intersection(cx, cy, radius,
			current_objects[i][OBJ_X], current_objects[i][OBJ_Y],
			current_objects[i][OBJ_W], current_objects[i][OBJ_H]))
			return 2;
	}

	if (cy + radius >= (float)CP_System_GetWindowHeight()) return 3;

	return 0;
}