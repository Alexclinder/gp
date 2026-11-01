#pragma once

typedef enum {
	NONE,			// 0
	WALL_CLIMB,		// 1
	AIR_DASH		// 2
} ability_type;

typedef struct {
	ability_type type;
	int unlocked;
} ability;

typedef int BOOL;