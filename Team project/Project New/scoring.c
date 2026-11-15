#include "cprocessing.h"
#include "game_mechanics.h"
#include "npc_mechanics.h"
#include "level.h"
#include "scoring.h"
#include "colors.h"
#include <stdio.h>


// player_score stores the total accumulated score across all completed levels.
// It carries forward the scores earned from previous levels.
 
// player_score will be used to store inside json if i got time....T_T
// Ask sin wee if dont knowwwww

int player_score = 0;  // Total score across all levels

// level_score stores the points earned in the current level only.
// It is reset to 0 whenever a new level starts or the current level restarts.
// store the level_score to each level
// Once the player completes the level, its final score is added to player_score.
int level_score = 0;   // Score earned in the current level

int teleport_score = 0;
int reflect_score = 0;
int npc_score = 0;

int timer_bonus_awarded = 0;
int level_score_locked = 0;

void game_score(void) {

	if (level_score_locked) {
		return;
	}

	// award 100 if reflect projectiles
	if (NPC_reflected && reflect_score == 0) {
		level_score += 100;
		reflect_score = 1;
	}

	// award 300 points if kill npc
	if (NPC_ded && npc_score == 0) {
		level_score += 300;
		npc_score = 1;
	}

	// award 750 if completed milestone objectives (or opened the door)
	if (pressed && unlocked_gate_score == 0) {
		level_score += 750;
		unlocked_gate_score = 1;
	}
}

	

	void time_score(void) {
		if (timer_bonus_awarded == 0) {

			if (Seconds < 25.0f)
				level_score += 1000;
			else if (Seconds < 35.0f)
				level_score += 500;

			timer_bonus_awarded = 1;
		}
	}


	void score_reset(void)
	{

		printf("Before reset: level_score = %d, player_score = %d\n",
			level_score, player_score);

		level_score = 0;

		reflect_score = 0;
		npc_score = 0;
		unlocked_gate_score = 0;

		timer_bonus_awarded = 0;
		level_score_locked = 0;
		teleport_score = 0;

		printf("After reset: level_score = %d, player_score = %d\n",
			level_score, player_score);
	}

	void score_complete_level(void)
	{
		printf("Complete level scoring called!\n");
		printf("Before completion: level_score = %d, player_score = %d\n",
			level_score, player_score);

		// safety check - just to make sure player dont receive duplicate score if this function kena called many times
		if (level_score_locked)
			return;

		game_score();
		
		time_score();

		// Transfer level score to cumulative score
		
		player_score += level_score;
		
		level_score_locked = 1;

		printf("After completion: level_score = %d, player_score = %d\n",
			level_score, player_score);
	}
	
	// for teleport activated - award points when teleport is active for 1 frame or small number of frames (event)
	void score_teleport_used(void)
	{
		printf("Teleport scoring called!\n");
		// award 200 we
		// check level_score_locked to avoid score changes after win
		if (!level_score_locked) {
			level_score += 200;
		}
	}

	// called when exiting from pause menu
	// called when win the game
	// called when lose the game
	// nah - just keep saving it while gameplay is running.
	
	// referenced from lewin's code
	void save_lvl_score(int score) {
		switch (level_selected) {
		case level_1:
			// Save score for level 1
			Level_Score.level1_score = score;
			break;
		case level_2:
			// Save score for level 2
			Level_Score.level2_score = score;
			break;
		case level_3:
			// Save score for level 3
			Level_Score.level3_score = score;
			break;
		case level_4:
			// Save score for level 3
			Level_Score.level4_score = score;
			break;
		}
	}


	// player_score is set to 0 first

	// If teleport has been used, +200 
	// If completed milestone objectives (level dependent)- do last since levels 1,2 & 4 not completed 
	// If deflected projectiles, +100
	// If kill enemies, +300


//Made by Jun kai, moved by Lewin was orginally in level.c
/**********************************************************Treasure/Collectables Functions*********************************************************************/
int Trea_score = 0;
int Trea;

int treasure_collected[MAX_TREASURES] = { 0,0 };

int Treasure(float x, float y, int treasure_id)
{
	float trea_w = 20.0f;
	float trea_h = 20.0f;

	// Check whether treasure has already been collected
	if (treasure_collected[treasure_id] == 1)
	{
		return 0;
	}


	CP_Settings_Fill(orange);
	CP_Graphics_DrawRect(x, y, trea_w, trea_h);

	float trea_left = x - trea_w / 2.0f;
	float trea_right = x + trea_w / 2.0f;
	float trea_top = y - trea_h / 2.0f;
	float trea_bottom = y + trea_h / 2.0f;


	float player_left = position.x;
	float player_right = position.x + PLAYER_SIZE;
	float player_top = position.y;
	float player_bottom = position.y + PLAYER_SIZE;


	if (player_right > trea_left &&
		player_left < trea_right &&
		player_bottom > trea_top &&
		player_top < trea_bottom)
	{
		treasure_collected[treasure_id] = 1;

		return 1; // Treasure collected this frame
	}

	return 0;
}

