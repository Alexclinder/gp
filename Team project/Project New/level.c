/*!
@file     level.c
@author   Sin Wee
@co-author Lewin Yeo -
@module   csd1401f26 Software Engineering Project 1
@section  B
@date     19/09/26
@brief    Implements level layouts, platform data, level timers, objectives, background drawing, and level selection.
@see	  Struct initialization: https://www.geeksforgeeks.org/c/how-to-initialize-structures-in-c/
*//*______________________________________________________________________*/
#include "cprocessing.h"
#include "level.h"
#include "game_mechanics.h"
#include "game.h"
#include "screen_seq.h"
#include "npc_mechanics.h"
#include "colors.h"
#include "sounds.h"
#include "scoring.h"
#include "elevator.h"
#include "Images.h"
#include <stdio.h>


/*Platform positions for Level 3*/
/*USED IN BOTH OBJECTS AND LEVELS FUNCTIONS*/
#define LEVEL3_STATIC_PLATFORMS 29
#define PLATFORM_WIDTH_3 120.0f
#define LEVEL3_PLATFORMS 30
#define LEVEL3_SEC_BUILDING 580.0f
#define LEVEL3_THIRD_BUILDING 280.0f
#define LEVEL3_FOURTH_BUILDING 320.0f
#define LEVEL3_BUILDING_WALL 10.0f
#define LEVEL3_FOURTH_BUILDING_WIDTH 2.0f

#define LEVEL1_STATIC_PLATFORMS 29
#define LEVEL2_STATIC_PLATFORMS 29
#define LEVEL4_STATIC_PLATFORMS 29
/*END*/



// Platform Parameters
#define PARAMETERS 6
#define NO_ROTATION 0
#define CORNER_RADIUS 0
#define RECT_HORIZ_H 15

/*******************Level Timer Functions************/
float Seconds, Zeroing_Time;
int TimeSet, PauseTimeStatus, ResetTime;
float PauseingameTime = 0.0, differenceTime = 0.0, WinScreenTime;
struct LevelTimer Level_Timer = { 0.0f, 0.0f, 0.0f , 0.0f };


float PosX = 0.0f;
float PosY = 0.0f;
float alpha = 255.0f;

/*
SaveTime

CHECK which level is selected
	LEVEL 1 -> SAVE time as level 1 time
	LEVEL 2 -> SAVE time as level 2 time
	LEVEL 3 -> SAVE time as level 3 time
	ANY other level -> SAVE time as level 4 time
*/
void SaveTime(float time) {
	switch (level_selected) {
	case level_1:
		// Save time for level 1
		Level_Timer.level1_time = time;
		break;
	case level_2:
		// Save time for level 2
		Level_Timer.level2_time = time;
		break;
	case level_3:
		// Save time for level 3
		Level_Timer.level3_time = time;
		break;
	default:
		// Save time for level 4
		Level_Timer.level4_time = time;
		break;

	}
}

void LeaderboardScore_Time(int score, float time) {
	// Implementation for updating leaderboard with score and time
	(void)score;
	(void)time;
}


float ingameTimer(void) {

	if (TimeSet == 1) {// If Timer has been set, then we can start counting time
		//differenceTime = Seconds - PauseingameTime;
		CP_Font_Set(Font);
		CP_Settings_TextSize(100);
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_TOP);
		CP_Settings_Fill(translucent_white);
		//Seconds variable is used to store the time that will be displayed on the screen. It is calculated by subtracting 
		// the Zeroing_Time from the current time and adding the PauseingameTime. 
		// This ensures that the timer continues from where it left off after a pause.
		Seconds = CP_System_GetSeconds() - Zeroing_Time + PauseingameTime;

		SaveTime(Seconds);//Save the current time to the Level_Timer struct for the current level, Time stored in the struct is displayed leaderboard
		char buffer[TIME_BUF_SIZE] = { 0 };
		sprintf_s(buffer, TIME_BUF_SIZE, "%.2f", Seconds);
		CP_Font_DrawText(buffer, (float)CP_System_GetWindowWidth() / 2.0f, 0.0f);
		CP_Settings_Fill(yellow);

		switch (level_selected) {
		case 	boss_level:
			CP_Graphics_DrawRect((float)CP_System_GetWindowWidth() / 2.0f + 150.0f, 45.0f, 50.0f, 50.0f);//Left Star
			if (Seconds < 90.0)
				CP_Graphics_DrawRect((float)CP_System_GetWindowWidth() / 2.0f + 225.0f, 45.0f, 50.0f, 50.0f); // Middle Star
			if (Seconds < 70.0)
				CP_Graphics_DrawRect((float)CP_System_GetWindowWidth() / 2.0f + 300.0f, 45.0f, 50.0f, 50.0f); //Right Star
			break;
		default:
			CP_Graphics_DrawRect((float)CP_System_GetWindowWidth() / 2.0f + 150.0f, 45.0f, 50.0f, 50.0f);//Left Star
			if (Seconds < 35.0)
				CP_Graphics_DrawRect((float)CP_System_GetWindowWidth() / 2.0f + 225.0f, 45.0f, 50.0f, 50.0f); // Middle Star
			if (Seconds < 25.0)
				CP_Graphics_DrawRect((float)CP_System_GetWindowWidth() / 2.0f + 300.0f, 45.0f, 50.0f, 50.0f); //Right Star
			break;
		}

	}
	else if (TimeSet == 0) {
		Zeroing_Time = CP_System_GetSeconds();
		//Zeroing_Time is set to the current time when the timer is first initialized. 
		//This serves as a reference point for calculating elapsed time.
		TimeSet = 1;

	}
	if (PauseTimeStatus == 1) {
		PauseingameTime = Seconds;// Store the time when the game is paused
		PauseTimeStatus = 0;// Reset the pause state so that the timer can continue counting when the game is resumed
	}
	if (ResetTime == 1) {
		PauseingameTime = 0.0;// Clear the pause time when the game is reset/New level is loaded
		ResetTime = 0;// Reset the reset state so that the status of the timer can be properly managed when the game is reset or a new level is loaded
	}
	WinScreenTime = Seconds;
	return Seconds;
}

/*********************Prints Objective Text*******************/
/*
Objective_Text

SET text position, offsets and size
UPDATE level score
CONVERT score to text

CHECK which level is selected
	LEVEL 1 -> DRAW "Get into the Building"
	LEVEL 2 -> DRAW "Avoid your Colleagues"
	LEVEL 3 -> DRAW "Get through the Manufacturing floor" (on two lines)
	LEVEL 4 -> DRAW "Fight the Boss"

	DRAW "Score :" and score below the objective
*/
void Objective_Text(void) {


	float top_left_x = 0.0f;
	float top_left_y = top_left_x;
	float score_offset_y = 50.0f;
	float score_offset_x = 150.0f;
	float text_size = 1.0f;
	game_score();

	char buffer[TIME_BUF_SIZE] = { 0 };
	sprintf_s(buffer, TIME_BUF_SIZE, "%d", level_score);

	// Text size = textSize (50.0f) * text_size

	// display level_score that will persists through all levels
	switch (level_selected) {
	case level_4:
		drawTextTopLeft("Objective: Fight the Boss", top_left_x, top_left_y, text_size, white);
		drawTextTopLeft("Score :", top_left_x, top_left_y + score_offset_y, text_size, white);
		drawTextTopLeft(buffer, top_left_x + score_offset_x, top_left_y + score_offset_y, text_size, white);
		break;
	case level_3:
		drawTextTopLeft("Objective: Get through the", top_left_x, top_left_y, text_size, white);
		drawTextTopLeft("Manufacturing floor", top_left_x, top_left_y + 50.0f, text_size, white);
		drawTextTopLeft("Score :", top_left_x, top_left_y + score_offset_y + 50.0f, text_size, white);
		drawTextTopLeft(buffer, top_left_x + score_offset_x, top_left_y + score_offset_y + 50.0f, text_size, white);
		break;

	case level_2:
		drawTextTopLeft("Objective: Avoid your Colleagues", top_left_x, top_left_y, text_size, white);
		drawTextTopLeft("Score :", top_left_x, top_left_y + score_offset_y, text_size, white);
		drawTextTopLeft(buffer, top_left_x + score_offset_x, top_left_y + score_offset_y, text_size, white);

		break;
	case level_1:
		drawTextTopLeft("Objective: Get into the Building", top_left_x, top_left_y, text_size, white);
		drawTextTopLeft("Score :", top_left_x, top_left_y + score_offset_y, text_size, white);
		drawTextTopLeft(buffer, top_left_x + score_offset_x, top_left_y + score_offset_y, text_size, white);

		break;
	default:
		break;


	}
}

/*****************Spawn Locations for Player********************/
spawn player_spawns[LEVELS] = {
							   { 0.0f, 0.0f }, // main menu - ignore, used for controls screen
							   { 100.0f, 800.0f }, // level 1 - player spawn bottom left screen
							   { 100.0f, 60.0f }, // level 2 - player spawn top left screen
							   { 100.0f, 800.0f }, // level 3 - player spawn bottom left screen
							   { 100.0f, 100.0f }, // level 4 
							   { 100.0f, 850.0f }  // boss level  
};


/*****************Spawn Locations for NPCs********************/
// [LEVELS][NUMBER_OF_NPC]
#define NUMBER_OF_NPC 1
spawn NPC_1_spawns[LEVELS] = {

							   { 0.0f, 0.0f }, // main menu - ignore, used for controls screen
							   { 1500.0f, 900.0f }, // level 1
							   { 100.0f, 900.0f }, // level 2
							   { 800.0f, 900.0f }, // level 3
							   { 1200.0f, 900.0f }, // level 4
							   { 300.0f, 900.0f }, // boss level

};

/**********************************************************LEVEL Selection*********************************************************************/

// Level selection function to call the appropriate level function based on the current level_selected value.
int level_selected = level_1;
void level_selection(int level_no) {
	switch (level_no) {
	case main_menu:
		Main_Menu_Update();
		break;
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
	case boss_level:
		Elevator_Update();
		break;
	}
}

/**********************************************************LEVEL Platforms*********************************************************************/

float killZoneX = 0.0f;     // start at the left
const float killZoneEndX = 1700.0f;  // last position
const float killZoneSpeed = 50.0f;   // pixels per second

typedef struct {
	float kx, ky, kw, kh;
} k_zone;

typedef struct {
	float px, py, pw, ph;
	float dx, dy, dw, dh;
	float d2x, d2y, d2w, d2h;
} pressure_door_pos;

//Pressure plate and parameters for levels 
#define PRESSURE_WIDTH 30.0f
#define PRESSURE_HEIGHT 15.0f
#define DOOR_WIDTH 10.0f
#define DOOR_HEIGHT 80.0f

/***************** LEVEL 1: ********************/
/*Platform positions for Level 1*/
float first_level[LEVEL1_STATIC_PLATFORMS][PARAMETERS] = {
	{ 470.0f, 830.0f, 180.0f, 170, NO_ROTATION, CORNER_RADIUS },
	{ 650.0f, 658.0f, 140.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 850.0f, 718.0f, 150.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 1050.0f, 628.0f, 150.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 1390.0f, 600.0f, 250.0f, 200, NO_ROTATION, CORNER_RADIUS },
	{ 1550.0f, 500.0f, 100.0f, 800, NO_ROTATION, CORNER_RADIUS },
	{ 400.0f, 530.0f, 15.0f, 170.0f, NO_ROTATION, CORNER_RADIUS },
	{ 550.0f, 400.0f, 15.0f, 170.0f, NO_ROTATION, CORNER_RADIUS },
	{ 400.0f, 300.0f, 15.0f, 170.0f, NO_ROTATION, CORNER_RADIUS },
	{ 430.0f, 390.0f, 45.0f, 15.0f, NO_ROTATION, CORNER_RADIUS },
	{ 550.0f, 300.0f, 45.0f, 15.0f, NO_ROTATION, CORNER_RADIUS },
	{ 450.0f, 200.0f, 100.0f, 15.0f, NO_ROTATION, CORNER_RADIUS },
	{ 850.0f, 200.0f, 500.0f, 15.0f, NO_ROTATION, CORNER_RADIUS },
	{ 850.0f, 350.0f, 500.0f, 15.0f, NO_ROTATION, CORNER_RADIUS }
};

// Win_con positions in level 1
win_condition level1_win = { 1450, 300, 100, 100, NO_ROTATION, CORNER_RADIUS };





/***************** LEVEL 2: Entering the building********************/
// Win_con positions in level 2
win_condition level2_win = { 1528.0f, 150.0f, 50.0f, 146.0f, NO_ROTATION, CORNER_RADIUS };

/*Kill Zones positions for Level 2*/
k_zone level2_floor = { .kx = 500.0f, .ky = 900.0f, .kw = 2500.0f, .kh = 10.0f };

/* Pressure plate and door positions for level 2*/
pressure_door_pos level2 = { .px = 1100.0f, .py = 570.0f, .pw = PRESSURE_WIDTH, .ph = PRESSURE_HEIGHT,
							.dx = 1360.0f, .dy = 250.0f, .dw = DOOR_WIDTH * 14.0f, .dh = DOOR_HEIGHT * 0.1f,
							.d2x = 1300.0f, .d2y = 490.0f, .d2w = 100.0f, .d2h = RECT_HORIZ_H, };

/*Platform positions for Level 2*/
float second_level[LEVEL2_STATIC_PLATFORMS][PARAMETERS] = {
	{ 0.0f, 250.0f, 630.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 600.0f, 10.0f, 50.0f, 560.0f, NO_ROTATION, CORNER_RADIUS },
	{ 500.0f, 460.0f, 240.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 345.0f, 355.0f, 15.0f, 220.0f, NO_ROTATION, CORNER_RADIUS },
	{ 800.0f, 360.0f, 240.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 1000.0f, 250.0f, 600.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 200.0f, 590.0f, 240.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 600.0f, 590.0f, 240.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 1000.0f, 590.0f, 240.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 1400.0f, 490.0f, 100.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },

	{ 1400.0f, 400.0f, 15.0f, 150.0f, NO_ROTATION, CORNER_RADIUS },
	{ 1540.0f, 260.0f, 220.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS }
};






/***************** LEVEL 3: Outside the building********************/

/* Pressure plate and door positions for level 3*/
pressure_door_pos level3 = { .px = 1185.0f, .py = 380.0f, .pw = PRESSURE_WIDTH, .ph = PRESSURE_HEIGHT,
							.dx = 1460.0f, .dy = 530.0f, .dw = DOOR_WIDTH, .dh = DOOR_HEIGHT };

/*Kill Zones positions for Level 3*/
// Bottom Pit 
k_zone level3_k = { .kx = 1010.0f, .ky = 730.0f, .kw = 910.0f, .kh = 20.0f };
// 4th Roof Kill
k_zone level3_4b = { .kx = 1055.0f, .ky = 295.0f, .kw = 295.0f, .kh = 5.0f };
// 2th and 3rd Building moving platform
k_zone level3_23b = { .kx = 790.0f, .ky = 465.0f, .kw = 210.0f, .kh = 15.0f };

// Win_con positions in level 3
win_condition level3_win = { 1490.0f, 530.0f, 50.0f, DOOR_HEIGHT, NO_ROTATION, CORNER_RADIUS };

float third_level[LEVEL3_STATIC_PLATFORMS][PARAMETERS] = {

	/* ---------- 1st building ----------  */
	// Jump max is 80f
	// 260.0f is first building height
	{ 290.0f, 830.0f, PLATFORM_WIDTH_3 / 2.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 350.0f, 820.0f, PLATFORM_WIDTH_3, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 120.0f, 740.0f, PLATFORM_WIDTH_3, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 330.0f, 660.0f, PLATFORM_WIDTH_3 + 30.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 120.0f, 580.0f, PLATFORM_WIDTH_3, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 350.0f, 500.0f, PLATFORM_WIDTH_3, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 250.0f, 420.0f, PLATFORM_WIDTH_3, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	{ 120.0f, 340.0f, PLATFORM_WIDTH_3, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
	// first building roof
	{ 300.0f, 260.0f, 240.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },

	/* ---------- 2nd building ----------  */

   // Left wall
   { 545.0f, 610.0f, LEVEL3_BUILDING_WALL, LEVEL3_SEC_BUILDING, NO_ROTATION, CORNER_RADIUS },
   // Roof
   { 660.0f, 320.0f, PLATFORM_WIDTH_3 * 2.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
   // 6th floor
   { 583.08f, 540.0f, PLATFORM_WIDTH_3 / 2.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },

   /* ---------- 2nd/3rd building ----------  */

   // Bottom Platform connecting 2nd and 3rd building 
   { 784.0f, 650.0f, PLATFORM_WIDTH_3 * 1.4, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },


   /* ---------- 3rd building ----------  */

   // Left Wall
   { 905.0f, 400.0f,  LEVEL3_BUILDING_WALL, LEVEL3_THIRD_BUILDING - 58.0f, NO_ROTATION, CORNER_RADIUS },

   // 4th Level	
   { 1038.0f, 500.0f, LEVEL3_FOURTH_BUILDING_WIDTH, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
   { 1190.0f, 500.0f, LEVEL3_FOURTH_BUILDING_WIDTH, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
   { 940.0f, 445.0f, LEVEL3_FOURTH_BUILDING_WIDTH * 20.0f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
   { 1080.0f, 580.0f, LEVEL3_FOURTH_BUILDING_WIDTH, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
   { 951.0f, 592.0f, LEVEL3_FOURTH_BUILDING_WIDTH, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },
   { 1055.0f, 710.0f, LEVEL3_FOURTH_BUILDING_WIDTH, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },


   // Right Wall
   { 1206.0f, 333.0f,  LEVEL3_BUILDING_WALL, LEVEL3_THIRD_BUILDING / 3.0f, NO_ROTATION, CORNER_RADIUS },

   /* ---------- 3rd/4th building ----------  */

   // Bottom Platform connecting 3rd and 4th building 
   { 1250.0f, 660.0f, PLATFORM_WIDTH_3, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },

   { 1300.0f, 390.0f, PLATFORM_WIDTH_3 * 2.8f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },

   /* ---------- 4th building ----------  */

   // Right Wall
   { 1490.0f, 730.0f,  LEVEL3_BUILDING_WALL * 4.8f, LEVEL3_FOURTH_BUILDING, NO_ROTATION, CORNER_RADIUS },

   // Top Right Wall
   { 1460.0f, 435.0f,  LEVEL3_BUILDING_WALL, LEVEL3_FOURTH_BUILDING / 3.0f, NO_ROTATION, CORNER_RADIUS },

   // Top Left Wall
   { 1320.0f, 530.0f,  LEVEL3_BUILDING_WALL, LEVEL3_FOURTH_BUILDING / 2.8f, NO_ROTATION, CORNER_RADIUS },

   // Top Left Platform
   { 1300.0f, 480.0f,  PLATFORM_WIDTH_3 / 2.2f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },

   // Platform
   { 1415.0f, 580.0f,  PLATFORM_WIDTH_3 * 1.6f, RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS },

   // Stair Platform
   { 1350.0f, 520.0f,  PLATFORM_WIDTH_3 / 3.0f, RECT_HORIZ_H * 1.5f, NO_ROTATION, CORNER_RADIUS },

};





/***************** LEVEL 4: ********************/
/*Platform positions for Level 4*/
float fourth_level[LEVEL4_STATIC_PLATFORMS][PARAMETERS] = {
	{ 150.0f, 500.0f, 300.0f, 600.0f, NO_ROTATION, CORNER_RADIUS },
	{ 550.0f, 500.0f, 300.0f, 600.0f, NO_ROTATION, CORNER_RADIUS },
	{ 950.0f, 230.0f, 300.0f, 300.0f, NO_ROTATION, CORNER_RADIUS },
	{ 950.0f, 750.0f, 300.0f, 300.0f, NO_ROTATION, CORNER_RADIUS },
	{ 800.0f, 35.0f, 1800.0f, 100.0f, NO_ROTATION, CORNER_RADIUS },
	{ 600.0f, 140.0f, 120.0f, 120.0f, NO_ROTATION, CORNER_RADIUS },
	{ 1400.0f, 500.0f, 400.0f, 600.0f, NO_ROTATION, CORNER_RADIUS }

};

// Parameters is for drawing and checking win_con in level 4
win_condition level4_win = { 1500, 130, 100, 100, NO_ROTATION, CORNER_RADIUS };







/**********************************************************Object Related*********************************************************************/

/**************Objects Checks & Draw Functions****************/
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
/*
Object_Check

Returns: index of the object the player is touching, or -1 if none

SET circle around player (center of player, radius = half player size)

FOR each object
	IF object is being held
		SKIP it
	IF player's circle touches the object
		RETURN object's index

RETURN -1
*/
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


#define HIGHLIGHT_PAD 6.0f
/*
Objects_Draw

IF player is not carrying or picking up
	FIND object the player is touching
GET frame time

GET platforms for the current level

FOR each object
	IF object is not being held
		WORK OUT next y position (falling down)
		SET floor to bottom of screen
		FOR each platform
			IF platform is under the object AND higher than current floor
				SET floor to top of that platform
		IF object reaches floor
			PUT object on top of floor
		ELSE
			KEEP falling

	DRAW highlight behind object (only visible if player is touching it)
	DRAW object
*/
void Objects_Draw(void) {
	int target = (!isCarrying && !pickingUp) ? Object_Check() : -1;
	float dt = CP_System_GetDt();

	/* platforms of the level being played */
	float (*plats)[PARAMETERS] = first_level;
	int plat_count = LEVEL1_STATIC_PLATFORMS;
	if (level_selected == level_2) { plats = second_level; plat_count = LEVEL2_STATIC_PLATFORMS; }
	if (level_selected == level_3) { plats = third_level;  plat_count = LEVEL3_STATIC_PLATFORMS; }
	if (level_selected == level_4) { plats = fourth_level; plat_count = LEVEL4_STATIC_PLATFORMS; }

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
/*
Object_HitCheck

Returns: 1 = hit a platform, 2 = hit another object, 3 = hit the ground, 0 = no hit

GET platforms for the current level

FOR each platform
	IF circle touches the platform
		RETURN 1

FOR each object
	IF object is itself
		SKIP it
	IF circle touches the object
		RETURN 2

IF circle reaches bottom of screen
	RETURN 3

RETURN 0
*/
int Object_HitCheck(float cx, float cy, float radius, int self) {
	/* platforms of the level being played */
	float (*plats)[PARAMETERS] = first_level;
	int plat_count = LEVEL1_STATIC_PLATFORMS;
	if (level_selected == level_2) { plats = second_level; plat_count = LEVEL2_STATIC_PLATFORMS; }
	if (level_selected == level_3) { plats = third_level;  plat_count = LEVEL3_STATIC_PLATFORMS; }
	if (level_selected == level_4) { plats = fourth_level; plat_count = LEVEL4_STATIC_PLATFORMS; }

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

// Points to the in-game object array of the level being played
float (*current_objects)[5] = NULL;
int current_obj_count = 0;
int heldObject = -1;

/***************** LEVEL 1 Objects ********************/
// Object start coords for level 1 (const so the level start never changes)
// { x, y, w, h, type }
const float first_level_objects[2][5] = {
	{ 120.0f, 690.0f, 60.0f, 45.0f, OBJ_DESK },
	{ 970.0f, 470.0f, 50.0f, 55.0f, OBJ_PRINTER }
};

// Object coords in game for level 1 (carrying and dropping change these)
float first_level_objects_ingame[2][5];



/***************** LEVEL 2 Objects ********************/
// Object start coords for level 2 (const so the level start never changes)
// { x, y, w, h, type }
const float second_level_objects[2][5] = {
	{ 120.0f, 690.0f, 60.0f, 45.0f, OBJ_DESK },
	{ 270.0f, 470.0f, 50.0f, 55.0f, OBJ_PRINTER }
};

// Object coords in game for level 2 (carrying and dropping change these)
float second_level_objects_ingame[2][5];



/***************** LEVEL 3 Objects ********************/
// Object start coords for level 3 (const so the level start never changes)
// { x, y, w, h, type }
const float third_level_objects[2][5] = {
	{ 120.0f, 690.0f, 60.0f, 45.0f, OBJ_DESK },
	{ 970.0f, 470.0f, 50.0f, 55.0f, OBJ_PRINTER }
};

// Object coords in game for level 3 (carrying and dropping change these)
float third_level_objects_ingame[2][5];







/**********************************************************LEVEL LOOP*********************************************************************/

/*****************BACKGROUND DRAWING FUNCTION****************/
//Draws the background image for the level being played
void DrawBG(CP_Image BG, float PosX, float PosY, float width, float height, float alpha) {
	/* --- Draw --- */
	CP_Settings_ImageMode(CP_POSITION_CORNER);
	CP_Image_Draw(BG, PosX, PosY, width, height, (int)alpha);
}


/*****************EXIT DRAWING FUNCTION****************/
//Draws the exit (goal) for the level being played
void DrawExit(float x, float y, float w, float h, float degrees, float cornerRadius) {
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Settings_Fill(medium_light_green);
	CP_Graphics_DrawRectAdvanced(x, y, w, h, degrees, cornerRadius);
}

/***************** LEVEL Generation: ********************/

/*
level_1st / level_2nd / level_3rd / level_4th

(LEVEL 4 ONLY) CLEAR background to black
			   MOVE kill zone right until it reaches its end point
			   DRAW kill zone

FOR each platform in this level
	DRAW platform (also checks player collision)

DRAW background
DRAW black bar at top of screen (HUD area)

DRAW level objects (depends on level)
	TREASURES        -> IF player collects one, ADD 500 to score
	KILL ZONES       -> still or moving, send player to lose screen on touch
	MOVING PLATFORMS -> move left and right between boundaries
	PRESSURE PLATES  -> open door(s) when pressed

DRAW exit

IF player touches exit
	ADD level complete score
	CLEAR projectiles
	IF level 4
		SET next level to boss level
		GO to elevator, then boss level
	ELSE
		GO to win screen
*/

/***************** LEVEL 1: ********************/
/* Alternating Kill Zones */
k_zone level1_kwall = { .kx = 600.0f, .ky = 300.0f, .kw = 5.0f, .kh = 100.0f };
k_zone level1_kwall2 = { .kx = 900.0f, .ky = 300.0f, .kw = 5.0f, .kh = 100.0f };

void level_1st(void) {



	for (int i = 0; i < LEVEL1_STATIC_PLATFORMS; i++) {
		DrawPlatform(first_level[i][0],
			first_level[i][1],
			first_level[i][2],
			first_level[i][3],
			first_level[i][4],
			first_level[i][5]);
	}
	DrawBG(backgroundthree, PosX, PosY, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight(), alpha);
	CP_Settings_Fill(black);
	CP_Graphics_DrawRect(800, 50, 1600, 100);

	if (Treasure(410.0f, 150.0f, 0))
	{
		level_score += 500;
	}

	if (Treasure(800.0f, 300.0f, 1))
	{
		level_score += 500;
	}
	// level 1 kill boundary 

	kill_zone_alternating_movement(level1_kwall.kx, level1_kwall.ky, level1_kwall.kw, level1_kwall.kh, 500.0f, 800.0f);
	kill_zone_alternating_movement(level1_kwall2.kx, level1_kwall2.ky, level1_kwall2.kw, level1_kwall2.kh, 900.0f, 1100.0f);


	DrawExit(level1_win.x, level1_win.y, level1_win.w, level1_win.h, level1_win.degrees, level1_win.cornerRadius);

	if (CheckWinCondition(level1_win.x, level1_win.y, level1_win.w, level1_win.h, level1_win.degrees, level1_win.cornerRadius) == 1) {
		score_complete_level();
		NPC_UpdateProjectiles(0);
		CP_Engine_SetNextGameState(Screen_Init, Win_Screen, Screen_Exit);
	}

}



/***************** LEVEL 2: ********************/
void level_2nd(void) {



	for (int i = 0; i < LEVEL2_STATIC_PLATFORMS; i++) {
		DrawPlatform(second_level[i][0],
			second_level[i][1],
			second_level[i][2],
			second_level[i][3],
			second_level[i][4],
			second_level[i][5]);
	}
	DrawBG(backgroundtwo, PosX, PosY, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight(), alpha);
	CP_Settings_Fill(black);
	CP_Graphics_DrawRect(800, 50, 1600, 100);
	kill_zone(level2_floor.kx, level2_floor.ky, level2_floor.kw, level2_floor.kh);

	Pressure_Plate_2Door(level2.px, level2.py, level2.pw, level2.ph,
		level2.dx, level2.dy, level2.dw, level2.dh,
		level2.d2x, level2.d2y, level2.d2w, level2.d2h
	);
	// Treasure at right side
	if (Treasure(1350.0f, 460.0f, 0)) {
		level_score += 500;
	}

	// Treasure at left side 
	if (Treasure(150.0f, 540.0f, 1)) {
		level_score += 500;
	}

	DrawExit(level2_win.x, level2_win.y, level2_win.w, level2_win.h, level2_win.degrees, level2_win.cornerRadius);

	if (CheckWinCondition(level2_win.x, level2_win.y, level2_win.w, level2_win.h, level2_win.degrees, level2_win.cornerRadius) == 1) {

		score_complete_level();

		NPC_UpdateProjectiles(0);
		CP_Engine_SetNextGameState(Screen_Init, Win_Screen, Screen_Exit);
	}

}



/***************** LEVEL 3: ********************/
#define LEFT_BOUNDARY 650.0f
#define RIGHT_BOUNDARY 850.0f

void level_3rd(void) {



	// Draw every platform (each row is one platform: x, y, w, h, rotation, corner radius)
	for (int i = 0; i < LEVEL3_STATIC_PLATFORMS; i++) {
		DrawPlatform(third_level[i][0],
			third_level[i][1],
			third_level[i][2],
			third_level[i][3],
			third_level[i][4],
			third_level[i][5]);
	}


	DrawBG(background, PosX, PosY, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight(), alpha);
	CP_Settings_Fill(black);
	CP_Graphics_DrawRect(800, 50, 1600, 100);

	// on tiny platform directly ontop of longest killzone
	if (Treasure(1050.0f, 680.0f, 0))
	{
		level_score += 500;
	}

	if (Treasure(1550.0f, 850.0f, 1))
	{
		level_score += 500;
	}

	// Top Platform connecting 2nd and 3rd building 

	platform_alternating_movement(850.0f, 450.0f, PLATFORM_WIDTH_3 / 3.0f,
		RECT_HORIZ_H, NO_ROTATION, CORNER_RADIUS,
		LEFT_BOUNDARY, RIGHT_BOUNDARY);


	// Produce the Pressure_Plate_Door
	Pressure_Plate_Door(level3.px, level3.py, level3.pw, level3.ph,
		level3.dx, level3.dy, level3.dw, level3.dh);

	kill_zone(level3_k.kx, level3_k.ky, level3_k.kw, level3_k.kh);
	kill_zone(level3_4b.kx, level3_4b.ky, level3_4b.kw, level3_4b.kh);
	kill_zone(level3_23b.kx, level3_23b.ky, level3_23b.kw, level3_23b.kh);


	DrawExit(level3_win.x, level3_win.y, level3_win.w, level3_win.h, level3_win.degrees, level3_win.cornerRadius);

	if (CheckWinCondition(level3_win.x, level3_win.y, level3_win.w, level3_win.h, level3_win.degrees, level3_win.cornerRadius) == 1) {

		score_complete_level();

		NPC_UpdateProjectiles(0);
		CP_Engine_SetNextGameState(Screen_Init, Win_Screen, Screen_Exit);
	}
}



/***************** LEVEL 4: ********************/
void level_4th(void) {



	CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));



	float dt = CP_System_GetDt();
	killZoneX += killZoneSpeed * dt;

	if (killZoneX > killZoneEndX)
		killZoneX = killZoneEndX;   // stop at endpoint

	kill_zone(killZoneX - 500.0f, 500.0f, 1000.0f, 900.0f);


	for (int i = 0; i < LEVEL4_STATIC_PLATFORMS; i++) {
		DrawPlatform(fourth_level[i][0],
			fourth_level[i][1],
			fourth_level[i][2],
			fourth_level[i][3],
			fourth_level[i][4],
			fourth_level[i][5]);
	}
	CP_Settings_Fill(black);
	CP_Graphics_DrawRect(800, 50, 1600, 100);

	DrawExit(level4_win.x, level4_win.y, level4_win.w, level4_win.h, level4_win.degrees, level4_win.cornerRadius);

	if (CheckWinCondition(level4_win.x, level4_win.y, level4_win.w, level4_win.h, level4_win.degrees, level4_win.cornerRadius) == 1) {
		score_complete_level();
		NPC_UpdateProjectiles(0);
		level_selected = boss_level;
		CP_Engine_SetNextGameState(Elevator_Init, Game_Update, Elevator_Exit);    // Move to boss lvl after lvl 4
	}
}



/**********************************************************Treasure/Collectables Functions*********************************************************************/
// Treasure/Collectables functions were moved to scoring.c by Lewin