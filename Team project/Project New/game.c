#include "cprocessing.h"
#include "screen_seq.h"
#include "game_mechanics.h"
#include "level.h"
#include "game.h"
#include "npc_mechanics.h"
#include "colors.h"
#include "scoring.h"




// For Resume Game Btn
void Game_Init(void)
{
	color_create();
	LoadLevelImages();
	Objects_Reset();
	score_reset();

	CP_System_SetFrameRate(120.0f);
	onGround = 0;
	TimeSet = 0;
}

void Game_Reset(void)
{
	LoadLevelImages();
	Teleport_count = 3;
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
	Player_Starting_Pos(player_spawns,level_selected);
	NPC_Starting_Pos(NPC_1_spawns, level_selected);

	// Unlocked scoring 
	npc_score = 0;
	reflect_score = 0;
	// only awarded once per level and unlocked only when restarted
	unlocked_gate_score = 0;
	pressed = 0;
	ProjectileActive = 0;
	Trea_score = 0;
	score_reset();

	for (int i = 0; i < MAX_TREASURES; i++) {
		treasure_collected[i] = 0;
	}
	
	killZoneX = 0.0f;
}

// Used in Game_Update() to call the appropriate level function based on the current level_selected value.
void Game_Update(void)
{		
		level_selection(level_selected);
		
		if (CP_Input_KeyTriggered(KEY_ESCAPE)) {

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
			NPC_UpdateProjectiles(0);
			CP_Engine_SetNextGameState(Screen_Init, Lose_Screen, Screen_Exit);
			// Update Player_Score if ded
			return;
		}

		Objects_Draw();
		Player_Animation();
		Draw_ClingBar();
		Shield_Draw();

		save_lvl_score(level_score);
}


void Game_Next_Level(void) {

	if (level_selected > 3) {
		CP_Engine_SetNextGameStateForced(Screen_Init, Completed_Game_Screen, Screen_Exit);
	}
	else {
		level_selected++;
		Game_Reset();
	}
}

void Game_Exit(void)
{
	FreeLevelImages();

}