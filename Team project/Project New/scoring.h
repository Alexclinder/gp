#pragma once

extern int* score;

void game_score(void);

void time_score(void);

void score_reset(void);

void score_complete_level(void);

void score_teleport_used(void);

extern int reflect_score;
extern int npc_score;


extern int timer_bonus_awarded;
extern int level_score_locked;