#pragma once

#define DESTINATION_BUFFER_SIZE 256
#define MAX_CHAR_LENGTH 255

// Player Infomation
typedef struct {
	char name[DESTINATION_BUFFER_SIZE];
	int levels_unlocked[4];
	int abilities_unlocked[4];
	int score;

} player_data;

player_data player_1st_save;

int writing_to_player_file(const char* name, int* levels_unlocked, int* abilities_unlocked, int* score);

int reading_from_player_file(char* name, int* levels_unlocked, int* abilities_unlocked, int* score);