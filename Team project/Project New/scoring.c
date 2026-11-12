#include "cprocessing.h"
#include "game_mechanics.h"
#include "npc_mechanics.h"
#include "level.h"


// player_score stores the total accumulated score across all completed levels.
// It carries forward the scores earned from previous levels.
 
// player_score will be used to store inside json if i got time....T_T
// Ask sin wee if dont knowwwww

int player_score = 0;  // Total score across all levels


// level_score stores the points earned in the current level only.
// It is reset to 0 whenever a new level starts or the current level restarts.
// Once the player completes the level, its final score is added to player_score.
int level_score = 0;   // Score earned in the current level

int* score = &level_score;

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
		*score += 100;
		reflect_score = 1;
	}

	// award 300 points if kill npc
	if (NPC_ded && npc_score == 0) {
		*score += 300;
		npc_score = 1;
	}

	// award 750 if completed milestone objectives (or opened the door)
	if (pressed && unlocked_gate_score == 0) {
		*score += 750;
		unlocked_gate_score = 1;

	}
}

	

	void time_score(void) {
		if (timer_bonus_awarded == 0) {

			if (Seconds < 25.0f)
				*score += 1000;
			else if (Seconds < 35.0f)
				*score += 500;

			timer_bonus_awarded = 1;
		}
	}


	void score_reset(void)
	{
		level_score = 0;

		reflect_score = 0;
		npc_score = 0;
		unlocked_gate_score = 0;

		timer_bonus_awarded = 0;
		level_score_locked = 0;
	}

	void score_complete_level(void)
	{
		// safety check - just to make sure player dont receive duplicate score if this function kena called many times
		if (level_score_locked)
			return;

		game_score();

		time_score();

		// Transfer level score to cumulative score
		// *score updates the value in address of level_score
		player_score += level_score;

		level_score_locked = 1;
	}

	void score_teleport_used(void)
	{
		// award 200 we
		// check level_score_locked to avoid score changes after win
		if (!level_score_locked) {
			*score += 200;
		}
	}


	// player_score is set to 0 first

	// If teleport has been used, +200 
	// If completed milestone objectives (level dependent)- do last since levels 1,2 & 4 not completed 
	// If deflected projectiles, +100
	// If kill enemies, +300
