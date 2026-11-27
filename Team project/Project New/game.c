/*!
@file     game.c
@author   Sin Wee
@co-author Lee Jun Kai (l.junkai) - Structuring of Game_Update & Game_Reset
@module   csd1401f26 Software Engineering Project 1
@section  B
@date     19/09/26
@brief	  Implements the core game loop: initialising, resetting, updating and
          exiting a level, and advancing to the next level.
*//*______________________________________________________________________*/
#include "cprocessing.h"
#include "screen_seq.h"
#include "game_mechanics.h"
#include "level.h"
#include "game.h"
#include "npc_mechanics.h"
#include "colors.h"
#include "scoring.h"
#include "elevator.h"
#include "Images.h"
#include <stdio.h>
#include "sounds.h"

// For Resume Game Btn
/*
Game_Init

LOAD font
RESET NPC projectile reflection status
RESET reflection score flag
CREATE colours
LOAD level images
RESET level objects
RESET current level score and bonus flags
CLEAR projectile
SET frame rate to 120 FPS
SET player as not on ground
RESET timer
IF selected level is Level 1, 2 or 3
	PLAY level music
*/
void Game_Init(void)
{
	Font = CP_Font_Load("Assets/Exo2-Regular.ttf");
	NPC_reflected = 0;
	reflect_score = 0;
	color_create();
	LoadLevelImages();
	Objects_Reset();
	level_score_reset();
	ProjectileActive = 0;
	CP_System_SetFrameRate(120.0f);
	onGround = 0;
	TimeSet = 0;
	if (level_selected <= level_3)//Only Play Level Music for Levels 1-3
	{
		PlaylevelMusicP1();
	}
}

/*
Game_Reset

LOAD level images
PRINT current level and player spawn position

SET teleport charges (4 = unlimited on boss level, otherwise 3)
RESET teleport cooldown
RESET player state (not on ground, not carrying, not picking up, not dropping, standing)
RESET level objects
RESET timer
MOVE player and NPC to this level's spawn points

RESET scoring flags (NPC, reflect, door, pressure plate)
CLEAR projectile
RESET level score
CLEAR all collected treasures
MOVE level 4 kill zone back to the start

IF boss level
	RESET boss level (done last so its spawn points override the ones above)
*/
void Game_Reset(void)
{
	if (level_selected <= level_3)//Only Play Level Music for Levels 1-3
	{
		PlaylevelMusicP1();
	}
		
	LoadLevelImages();
	printf("Current Level Restarted: %d\n", level_selected);
	printf("Current Player Coordinates After Level Restarted: (%.2f, %.2f)\n",
		player_spawns[level_selected].pos.x,
		player_spawns[level_selected].pos.y);
	
	Teleport_count = level_selected == boss_level ? 4 : 3;
	Teleport_Status = 1;
	TeleportWindowTiming = 0.1f;
	onGround = 0;
	Objects_Reset();
	isCarrying = 0;
	pickingUp = 0;
	dropping = 0;
	player_action = 1;
	TimeSet = 0;
	ResetTime = 1;
	PauseTimeStatus = 0;
	Player_Starting_Pos(player_spawns,level_selected); // shld also reset boss spawn
	NPC_Starting_Pos(NPC_1_spawns, level_selected);


	// Unlocked scoring 
	npc_score = 0;
	reflect_score = 0;
	// only awarded once per level and unlocked only when restarted
	unlocked_gate_score = 0;
	pressed = 0;
	ProjectileActive = 0;
	Trea_score = 0;
	level_score_reset();

	for (int i = 0; i < MAX_TREASURES; i++) {
		treasure_collected[i] = 0;
	}
	

	// Boss level: reset health, lasers, platforms, camera, vent and boss.
	// Called last so the boss level's player/NPC positions override the ones above.
	if (level_selected == boss_level)
		PlayBossMusicP1(&BossMusic_Play);//Play Boss Music
		Elevator_Reset();
}

// Used in Game_Update() to call the appropriate level function based on the current level_selected value.
/*
Game_Update

RUN current level (draw platforms, kill zones, treasures, exit)

IF Esc is pressed
	SAVE current time
	GO to pause menu

DRAW objective and score
UPDATE and DRAW timer
UPDATE teleport cooldown and run teleport mechanic

UPDATE NPC (move, attack, projectiles, shield and reflect checks)
DRAW projectiles

IF carrying an object
	MOVE player with carry controls
ELSE
	MOVE player with normal controls
UPDATE shield

IF NPC hits the player
	CLEAR level score
	CLEAR projectiles
	GO to lose screen
	RETURN

DRAW objects, player, wall cling bar and shield
SAVE level score for leaderboard
*/
void Game_Update(void)
{		
		level_selection(level_selected);
		if (CP_Input_KeyTriggered(KEY_ESCAPE)) {
			PauseingameTime = Seconds;
			CP_Engine_SetNextGameState(Screen_Init, Pause_Menu_Update, Screen_Exit);
		}
		

		Objective_Text();
		ingameTimer();
		TeleportTimer();
		Teleport_Mechannic();
		
		// Spawn in Enemy
		NPC_MovementDefault();
		NPC_Attack();
		NPC_UpdateProjectiles(1);
		NPC_CheckShieldCollision();
		NPC_CheckReflectedHit();
		NPC_DrawProjectiles();

		if (isCarrying) Character_MovementCarry();
		else Character_MovementDefault();

		Shield_Update();

		// Check for collision then end game if hit
		if (NPC_CheckPlayerCollision() == 1)
		{
			score_abandoned_level();
			NPC_UpdateProjectiles(0);
			CP_Engine_SetNextGameState(Screen_Init, Lose_Screen, Screen_Exit);
			return;
		}

		Objects_Draw();
		Player_Animation();
		Draw_ClingBar();
		Shield_Draw();

		save_lvl_score(level_score);
}

/*
Game_Next_Level

GO to next level number
RESET game
*/
void Game_Next_Level(void) {


		level_selected++;
		Game_Reset();
	
}

/*
Game_Exit

FREE level images
FREE font
*/
void Game_Exit(void)
{
	CP_Sound_StopGroup(CP_SOUND_GROUP_3);//Stop Pause Menu Music
	PauseMenu_Music = 1; //Renable Pause Music
	Lose_Sound = 1;//Renable Lose Sound
	Win_Sound = 1;//Renable Win Sound
	BossMusic_Play = 1;//Renable Boss Sound
	FreeLevelImages();
	CP_Font_Free(Font);
}