#pragma once

extern int level_score;

void game_score(void);

void time_score(void);

void score_reset(void);
void level_score_reset(void);

void score_complete_level(void);

void score_teleport_used(void);

extern int reflect_score;
extern int npc_score;

extern int player_score;
extern int level_score;

extern int timer_bonus_awarded;
extern int level_score_locked;

extern struct LevelScore {
	int level1_score;
	int level2_score;
	int level3_score;
	int level4_score;
} Level_Score;

void save_lvl_score(int score);

void score_abandoned_level(void);

void score_remove_level(int level);

// Can move time_buf to timer.h, for now its here
enum BUFFER_SIZE { TIME_BUF_SIZE = 16, SCORE_BUF_SIZE = 16 };

/*********Treasure Related************/
#define MAX_TREASURES 2

extern int Trea_score;
extern int Trea;
extern int treasure_collected[MAX_TREASURES];
int Treasure(float x, float y, int treasure_id);

