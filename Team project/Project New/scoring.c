/*!
@file     scoring.c
@author   Sin Wee
@module   csd1401f26 Software Engineering Project 1
@section  B
@date     10/11/26
@brief
*//*______________________________________________________________________*/
#include "cprocessing.h"
#include "game_mechanics.h"
#include "npc_mechanics.h"
#include "level.h"
#include "scoring.h"
#include "colors.h"
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
// LEVELS are 6
int completed_level_score[LEVELS] = { 0 };
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

IF level was not finished
	RESET current level score
	CLEAR saved score for this level
*/
void score_abandoned_level(void) {
	// Player did not complete the level
	if (!level_score_locked)
	{
		level_score_reset();
		save_lvl_score(0); // clears the level score
	}
}



/*
score_remove_level

IF level number is not valid
	RETURN

TAKE this level's old score away from total score
IF total score goes below 0
	SET total score to 0
CLEAR this level's old score
RESET current level score
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
SAVE level score for this level
ADD level score to total score
LOCK level score
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

	completed_level_score[level_selected] = level_score;

	// Transfer level score to cumulative score
	player_score += level_score;

	level_score_locked = 1;

	printf("After completion: level_score = %d, player_score = %d\n",
		level_score, player_score);
}

/*
score_teleport_used

Called once each time the player teleports

PRINT debug message
IF level score is not locked
	ADD 200 to level score
*/
void score_teleport_used(void)
{
	printf("Teleport scoring called!\n");
	// award 200
	// check level_score_locked to avoid score changes after win
	if (!level_score_locked) {
		level_score += 200;
	}
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

		return 1; // Treasure collected this frame
	}

	return 0;
}