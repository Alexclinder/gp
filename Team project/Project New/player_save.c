// Ask sin wee for any concerns or questions about the code. Or just read the comments. I suck at explaining things.

/* References used:
* https://github.com/davegamble/cjson
* https://www.geeksforgeeks.org/c/structure-pointer-in-c/
* https://www.youtube.com/watch?v=n_Se6bt8jM0
* https://www.youtube.com/watch?v=BR5TlKwpr9M
* https://en.cppreference.com/c/io/fseek
* https://en.cppreference.com/c/string/byte/strncpy 
*/



// cJSON.h must be included before cprocessing.h to avoid conflicts with the bool type defined in cprocessing.h.
#include "cJSON.h"
#include "cprocessing.h"
#include <stdio.h>
// need to include string.h for strcpy and strlen, and stdlib.h for malloc and free
#include <string.h>
#include <stdlib.h>
#include "player_save.h"

#include "level.h"
#include "player_abilities.h"

/*
@brief Writes player data to a JSON save file
@return 0 on success, 1 on failure
@param player_data
*/
int writing_to_player_file(const char *name, int*levels_unlocked, int *abilities_unlocked, int *score) {

	// Create a JSON object
	cJSON *player_save_json = cJSON_CreateObject();
	// Add some data to the JSON object
	cJSON_AddStringToObject(player_save_json, "name", name);
	cJSON_AddNumberToObject(player_save_json, "levels_unlocked", *levels_unlocked);
	cJSON_AddNumberToObject(player_save_json, "abilities_unlocked", *abilities_unlocked);
	cJSON_AddNumberToObject(player_save_json, "score", *score);

	// Convert the JSON object to a string
	char* json_string = cJSON_Print(player_save_json);

	// Save the JSON string to a player save file
	FILE *player_file = fopen("player_save.json", "w");
	
	if (player_file == NULL) {
		printf("Error opening file for writing.\n");
		return 1;
	}
	fputs(json_string, player_file);
	fclose(player_file);

	// free the JSON string and object to avoid memory leaks
	// Avoid dangling pointers by setting the pointer to NULL after freeing it
	cJSON_free(json_string);
	cJSON_Delete(player_save_json);
	json_string = NULL;
	player_save_json = NULL;

	return 0;
}


int reading_from_player_file(char* name, int* levels_unlocked, int* abilities_unlocked, int* score) {
	// Open the player save file for reading
	FILE* player_file = fopen("player_save.json", "r");

	if (player_file == NULL) {
		printf("Error opening file for reading.\n");
		return 1;
	}
	// Read the JSON string from the file

	// fseek is first used to move the file pointer to the end of the file, 
	// so ftell can be used to get the current position of the file pointer, 
	// which is the size of the file in bytes.
	fseek(player_file, 0, SEEK_END);
	// Get the size of the file in bytes
	long file_size = ftell(player_file);
	// fseek is used to move the file pointer back to the beginning of the file.
	fseek(player_file, 0, SEEK_SET);

	// malloc() or memory allocation() = A function in C that dynamically allocates 
	//									 a specified number of bytes in memory
	
	// malloc is used to allocate memory for the JSON string, and +1 is added to account for the null terminator
	// The allocated memory is cast to a char pointer and assigned to the json_string variable
	char* json_string = (char*)malloc(file_size + 1);
	// fread is used to read the contents of the file into the player_save_json buffer, 
	// and the null terminator is added at the end
	fread(json_string, 1, file_size, player_file);
	json_string[file_size] = '\0';
	fclose(player_file);

	// Parse the JSON string into a cJSON object
	cJSON* player_save_json = cJSON_Parse(json_string);
	if (player_save_json == NULL) {
		printf("Error parsing JSON string.\n");
		free(json_string);
		return 1;
	}
	// Extract data from the cJSON object
	cJSON* name_json = cJSON_GetObjectItemCaseSensitive(player_save_json, "name");
	cJSON* levels_unlocked_json = cJSON_GetObjectItemCaseSensitive(player_save_json, "levels_unlocked");
	cJSON* abilities_unlocked_json = cJSON_GetObjectItemCaseSensitive(player_save_json, "abilities_unlocked");
	cJSON* score_json = cJSON_GetObjectItemCaseSensitive(player_save_json, "score");

	// Check if the extracted items are of the expected type and assign them to the output parameters
	if (cJSON_IsString(name_json) && (name_json->valuestring != NULL)) {
		// strncpy_s is a safer version of strcpy that prevents buffer overflows by 
		// specifying the maximum number of characters to copy.
		
		//errno_t strncpy_s( char* restrict dest, rsize_t destsz,
		//const char* restrict src, rsize_t count );
		strncpy_s(name, DESTINATION_BUFFER_SIZE, name_json->valuestring, MAX_CHAR_LENGTH);
	}
	if (cJSON_IsNumber(levels_unlocked_json)) {
		*levels_unlocked = levels_unlocked_json->valueint;
	}
	if (cJSON_IsNumber(abilities_unlocked_json)) {
		*abilities_unlocked = abilities_unlocked_json->valueint;
	}
	if (cJSON_IsNumber(score_json)) {
		*score = score_json->valueint;
	}

	// Free the JSON string and object to avoid memory leaks
	// Avoid dangling pointers by setting the pointer to NULL after freeing it
	cJSON_free(json_string);
	json_string = NULL;
	cJSON_Delete(player_save_json);
	player_save_json = NULL;
	return 0;
}

// Only 1 save for now!!
// To use array to append levels and abilities unlocked to store them.

void player_1st_save_init(void) {
	// Initialize player data with default values
	// Maybe enable the player to input their name in the future, but for now, just use "Player_1"
	strncpy_s(player_1st_save.name, DESTINATION_BUFFER_SIZE, "Player_1", MAX_CHAR_LENGTH);

	*player_1st_save.levels_unlocked = (int*)malloc(sizeof(int) * 4);
	*player_1st_save.abilities_unlocked = (int*)malloc(sizeof(int) * 4);
	*player_1st_save.score = (int*)malloc(sizeof(int));

	if (player_1st_save.levels_unlocked == NULL || player_1st_save.abilities_unlocked == NULL || player_1st_save.score == NULL) {
		printf("Memory allocation failed for player data.\n");
		exit(1); // Exit if memory allocation fails. 1 means not good.
	}
	*player_1st_save.levels_unlocked = level_selected; // Level 1 is unlocked by default
	*player_1st_save.abilities_unlocked = NONE; // No abilities unlocked by default
	*player_1st_save.score = 0; // Score starts at 0
	// Save the initial player data to the save file
	writing_to_player_file(player_1st_save.name, player_1st_save.levels_unlocked, player_1st_save.abilities_unlocked, player_1st_save.score);
}

void player_level_progress(int* levels_unlocked) {
	// Saved player data to player save file first

	switch (*levels_unlocked) {
		// level 1 is unlocked by default, so no expression needed
	case 1:
	case 2:
		*player_1st_save.abilities_unlocked = WALL_CLIMB ; // Unlock ability 1
		*player_1st_save.levels_unlocked = 2; // Unlock level 2
		*player_1st_save.score += 100; // Add 100 points to score
		break;
	case 3:
		*player_1st_save.abilities_unlocked = AIR_DASH; // Unlock ability 2
		*player_1st_save.levels_unlocked = 3; // Unlock level 3
		*player_1st_save.score += 100; // Add 100 points to score
		break;
	default:
		// idk for now..
		break;
	}
}

void player_score(int* score) {
	// Add 100 points to score
	*score += 100;
}

