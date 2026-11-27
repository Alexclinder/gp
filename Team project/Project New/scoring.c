/*!
@file     scoring.c
@author   Sin Wee
@module   csd1401f26 Software Engineering Project 1
@section  B
@date     10/11/26
@brief	  Implements the scoring system: bonus awards, level and cumulative
          scores, per-level score saving, and treasure collection.
*//*______________________________________________________________________*/
#include "cprocessing.h"
#include "game_mechanics.h"
#include "npc_mechanics.h"
#include "level.h"
#include "scoring.h"
#include "colors.h"
#include "sounds.h"
#include <stdio.h>


// player_score stores the total accumulated score across all completed levels.
// It carries forward the scores earned from previous levels.
int player_score = 0;  // Total score across all levels

// level_score stores the points earned in the current level only.
// It is reset to 0 whenever a new level starts or the current level restarts.
// Once the player completes the level, its final score is added to player_score.
int level_score = 0;   // Score earned in the current level

// Points awarded:
// Teleport used       +200 (each time)
// Reflect projectile  +100 (once per level)
// Kill NPC            +300 (once per level)
// Open door           +750 (once per level)
// Treasure            +500 (each, added in level.c)
// Time bonus          +1000 under 25s, +500 under 35s
int teleport_score = 0;
int reflect_score = 0;
int npc_score = 0;

int timer_bonus_awarded = 0;
int level_score_locked = 0;

int treasure_collected[MAX_TREASURES] = { 0 };
int completed_level_score[LEVELS] = { 0 }; // LEVELS are 6
struct LevelScore Level_Score = { 0, 0, 0, 0 }; //Was in level.c, moved to scoring.c by Lewin

/*
game_score

IF level score is locked (level already finished)
	RETURN

IF player reflected a projectile AND not awarded yet
	ADD 100 to level score
IF player killed the NPC AND not awarded yet
	ADD 300 to level score
IF pressure plate is pressed AND not awarded yet
	ADD 750 to level score
*/
void game_score(void)
{
	// Do not award points after completing the level
	if (level_score_locked)
		return;

	// Award 100 points for reflecting a projectile (once per level)
	if (NPC_reflected && reflect_score == 0)
	{
		level_score += 100;
		reflect_score = 1;
	}

	// Award 300 points for killing an NPC (once per level)
	if (NPC_ded && npc_score == 0)
	{
		level_score += 300;
		npc_score = 1;
	}

	// Award 750 points for opening the door (once per level)
	if (pressed && unlocked_gate_score == 0)
	{
		level_score += 750;
		unlocked_gate_score = 1;
	}
}

/*
level_score_reset

SET level score to 0
CLEAR all bonus flags (reflect, NPC, door, teleport, time bonus)
UNLOCK level score
CLEAR all collected treasures

(player_score is not changed)
*/
void level_score_reset(void)
{
	level_score = 0;

	reflect_score = 0;
	npc_score = 0;
	unlocked_gate_score = 0;
	teleport_score = 0;

	timer_bonus_awarded = 0;
	level_score_locked = 0;

	for (int i = 0; i < MAX_TREASURES; i++) {
		treasure_collected[i] = 0;
	}
}

/*
time_score

IF time bonus not given yet
	IF under 25 seconds -> ADD 1000 to level score
	ELSE IF under 35 seconds -> ADD 500 to level score
	SET time bonus as given
*/
void time_score(void) {
	if (timer_bonus_awarded == 0) {

		if (Seconds < 25.0f)
			level_score += 1000;
		else if (Seconds < 35.0f)
			level_score += 500;

		timer_bonus_awarded = 1;
	}
}


/*
score_reset

PRINT scores before reset
RESET current level score
SET total score to 0
CLEAR every level's completed score
CLEAR saved scores for the leaderboard
PRINT scores after reset
*/
void score_reset(void)
{

	printf("Before reset: level_score = %d, player_score = %d\n",
		level_score, player_score);

	level_score_reset();
	player_score = 0;

	for (int i = 0; i < LEVELS; i++) {
		completed_level_score[i] = 0;
	}


	Level_Score = (struct LevelScore){ 0, 0, 0, 0 };


	printf("After reset: level_score = %d, player_score = %d\n",
		level_score, player_score);
}

/*
score_abandoned_level

CHECK which level the player is leaving

IF Level 4 or Boss Level is selected
	REMOVE the selected level's score from total player score
	IF total score goes below 0
		SET total score to 0
	CLEAR the selected level's saved score
	UPDATE the shared Level 4/Boss Level leaderboard score
	RESET current level score and bonus flags
	SAVE the updated shared leaderboard score

ELSE IF current level is not completed
	RESET current level score and bonus flags
	CLEAR saved score for the current level

	CHECK which level is selected
		LEVEL 1 -> CLEAR level 1 completed score
		LEVEL 2 -> CLEAR level 2 completed score
		LEVEL 3 -> CLEAR level 3 completed score

KEEP scores from other completed levels unchanged
*/
void score_abandoned_level(void)
{
	// Player is leaving the current level
	if (level_selected == level_4 || level_selected == boss_level)
	{
		// Only remove the score belonging to the selected level
		player_score -= completed_level_score[level_selected];

		if (player_score < 0)
			player_score = 0;

		// Clear the selected level's saved score
		completed_level_score[level_selected] = 0;

		// Update the shared Level 4/Boss Level leaderboard score
		Level_Score.level4_score =
			completed_level_score[level_4] +
			completed_level_score[boss_level];

		// Reset current level score and flags
		level_score_reset();
		save_lvl_score(Level_Score.level4_score);
	}
	else if (!level_score_locked)
	{
		// Player did not complete the level
		level_score_reset();
		save_lvl_score(0);

		// Clear the completed score based on selected level
		switch (level_selected)
		{
		case level_1:
		case level_2:
		case level_3:
			completed_level_score[level_selected] = 0;
			break;

		default:
			break;
		}
	}
}



/*
score_remove_level

IF level number is not valid
	RETURN

DEDUCT the selected level's previous score from total player score

IF total score goes below 0
	SET total score to 0

CLEAR the selected level's saved score

IF selected level is Level 4 or Boss Level
	UPDATE the shared Level 4/Boss Level leaderboard score
	USING the remaining saved scores from both levels

RESET current level score and scoring flags
(Keep remaining total player score intact)
*/
void score_remove_level(int level)
{
	if (level < 1 || level >= LEVELS)
		return;

	// Deduct the selected level's previous contribution
	player_score -= completed_level_score[level];

	if (player_score < 0)
		player_score = 0;

	// Clear the previous contribution
	completed_level_score[level] = 0;

	// Level 4 and Boss Level share the leaderboard score
	if (level == level_4 || level == boss_level)
	{
		Level_Score.level4_score =
			completed_level_score[level_4] +
			completed_level_score[boss_level];
	}

	// Reset the current level's score and flags, but keep remaining player_score intact
	level_score_reset();
}
/*
score_complete_level

PRINT scores before completion

IF level score is locked
	RETURN

ADD bonus points (reflect, NPC, door)
ADD time bonus

CHECK which level is selected
	LEVEL 1, 2 or 3
		SAVE level score for the selected level

	LEVEL 4 or BOSS LEVEL
		ADD level score to the selected level's saved score
		UPDATE shared Level 4/Boss Level leaderboard score
		USING the combined scores of both levels

ADD current level score to total player score

LOCK level score to prevent duplicate scoring

PRINT scores after completion
*/
void score_complete_level(void)
{
	printf("Complete level scoring called!\n");
	printf("Before completion: level_score = %d, player_score = %d\n",
		level_score, player_score);

	// Safety check: stop the score being added twice if this is called more than once
	if (level_score_locked)
		return;

	game_score();
	time_score();

	// Save score for the completed level
	// Level 4 and Boss Level share the leaderboard score
	switch (level_selected)
	{
	case level_1:
	case level_2:
	case level_3:
		completed_level_score[level_selected] = level_score;
		break;

	case level_4:
	case boss_level:
		completed_level_score[level_selected] += level_score;

		Level_Score.level4_score =
			completed_level_score[level_4] +
			completed_level_score[boss_level];
		break;

	default:
		break;
	}

	// Transfer level score to cumulative score
	player_score += level_score;

	level_score_locked = 1;

	printf("After completion: level_score = %d, player_score = %d\n",
		level_score, player_score);
}

/*
score_teleport_used

Called whenever the player successfully teleports

IF level score is locked
	RETURN

ADD 200 points to current level score
INCREMENT teleport score counter
*/
void score_teleport_used(void)
{
	printf("Teleport scoring called!\n");

	// Prevent scoring after the level has been completed
	if (level_score_locked)
		return;

	// Award 200 points for every successful teleport
	level_score += 200;

	teleport_score++;
}
/*
save_lvl_score

Based on Lewin's SaveTime in level.c

CHECK which level is selected
	LEVEL 1 -> SAVE score as level 1 score
	LEVEL 2 -> SAVE score as level 2 score
	LEVEL 3 -> SAVE score as level 3 score
	LEVEL 4 -> SAVE score as level 4 score
*/
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
		// Save score for level 4
		Level_Score.level4_score = score;
		break;
	}
}




/**********************************************************Treasure/Collectables Functions*********************************************************************/
int Trea_score = 0;
int Trea;


/*
Treasure

Returns: 1 = collected this frame, 0 = not collected

IF treasure is already collected
	RETURN 0

DRAW treasure as an orange square
GET treasure edges from its center and size
GET player edges from its position

IF player overlaps the treasure
	SET treasure as collected
	RETURN 1
RETURN 0
*/
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
		CP_Sound_Play(Treasure_Sound);//Sound when Player collects the treasure
		return 1; // Treasure collected this frame
	}

	return 0;
}