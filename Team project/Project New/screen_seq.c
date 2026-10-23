/* Find sin wee for any concerns or questions about the code.
@brief This file contains the screen sequence for the game
		1) Main Menu
		2) Pause Menu
		3) Win Menu
		4) Lose Menu

		The remaining functions are used to simplify the code and make it more readable.
*/

#include "cprocessing.h"
#include "game_mechanics.h"
#include "game.h"
#include "screen_seq.h"
#include "level.h"

// Global variables are required for constant updates.
float textSize= 50.0f;
CP_Font Font;

float first_y;
float second_y;
float third_y;

float rect_width = 400.0f;
float rect_height = 100.0f;
float scalar;


void Screen_Init(void)
{
    Font = CP_Font_Load("Assets/Exo2-Regular.ttf");
    CP_Font_Set(Font);
}

void Next_level_Init(void) {
    Game_Next_Level();
    
    Game_Reset();
}

/*
CP_Graphics Clear Background needs to be at the top.
Otherwise, it might clear buttons and text */
void Main_Menu_Update(void) {
    CP_Graphics_ClearBackground(CP_Color_Create(100, 100, 100, 0));
    level_selected = 1;

    // Start Game
	first_y = CP_System_GetWindowHeight() / 2.0 - 150.0f;
	// Exit to Desktop
	second_y = CP_System_GetWindowHeight() / 2.0 + 150.0f;
    
    dynamicBtn("Start Game!", first_y, Game_Init, Game_Update, Game_Exit);
    Exit_dynamicBtn("Exit to Desktop", second_y);

    
}

void Pause_Menu_Update(void)
{
    CP_Graphics_ClearBackground(CP_Color_Create(100, 200, 100, 0));

    // Resume
    first_y = CP_System_GetWindowHeight() / 2.0 - 150.0f;
    // Restart
    second_y = CP_System_GetWindowHeight() / 2.0; 
	// Exit to Main Menu
    third_y = CP_System_GetWindowHeight() / 2.0 + 150.0f;
    
    dynamicBtn("Resume Game!", first_y, Game_Init, Game_Update, Game_Exit);
    dynamicBtn("Restart Level", second_y, Game_Reset, Game_Update, Game_Exit);
    dynamicBtn("Exit to Main Menu", third_y, Screen_Init, Main_Menu_Update, Screen_Exit);

    
}

void Win_Screen(void) {

    CP_Graphics_ClearBackground(CP_Color_Create(100, 200, 100, 0));

    // Next Level
    first_y = CP_System_GetWindowHeight() / 2.0 - 150.0f;
    // Restart
    second_y = CP_System_GetWindowHeight() / 2.0;
    // Exit to Main Menu
    third_y = CP_System_GetWindowHeight() / 2.0 + 150.0f;
    
    
    dynamicBtn("Next Level", first_y, Next_level_Init, Game_Update, Game_Exit);
    dynamicBtn("Restart Level", second_y, Game_Reset, Game_Update, Game_Exit);
    dynamicBtn("Exit to Main Menu", third_y, Screen_Init, Main_Menu_Update, Screen_Exit);

    
}

void Lose_Screen(void) {
    CP_Graphics_ClearBackground(CP_Color_Create(100, 100, 100, 0));

	// Restart Level
    first_y = CP_System_GetWindowHeight() / 2.0 - 150.0f;
	// Exit to Main Menu
    second_y = CP_System_GetWindowHeight() / 2.0 + 150.0f;

    dynamicBtn("Restart Level", first_y, Game_Reset, Game_Update, Game_Exit);
    dynamicBtn("Exit to Main Menu", second_y, Screen_Init, Main_Menu_Update, Screen_Exit);

    
}

void Leaderboard_Screen(void) {
    CP_Graphics_ClearBackground(CP_Color_Create(100, 140, 200, 0));

    // Restart Level
    first_y = CP_System_GetWindowHeight() / 2.0 - 150.0f;
    // Exit to Main Menu
    second_y = CP_System_GetWindowHeight() / 2.0 + 150.0f;

    dynamicBtn("Level Select", first_y, Screen_Init, Level_Select_Screen, Screen_Exit);
    dynamicBtn("Exit to Main Menu", second_y, Screen_Init, Main_Menu_Update, Screen_Exit);

}

void Completed_Game_Screen(void) {
    CP_Graphics_ClearBackground(CP_Color_Create(100, 200, 100, 0));

    // Next Level
    first_y = CP_System_GetWindowHeight() / 2.0 - 150.0f;
    // Restart
    second_y = CP_System_GetWindowHeight() / 2.0;
    // Exit to Main Menu
    third_y = CP_System_GetWindowHeight() / 2.0 + 150.0f;

    dynamicBtn("Leaderboard", first_y, Screen_Init, Leaderboard_Screen, Screen_Exit);
    dynamicBtn("Level Select", second_y, Screen_Init, Level_Select_Screen, Screen_Exit);
    dynamicBtn("Exit to Main Menu", third_y, Screen_Init, Main_Menu_Update, Screen_Exit);
}

void Level_Select_Screen(void) {
    // Draw 4 boxes 

    // Label them 
    dynamicBtn("Exit to Main Menu", third_y, Screen_Init, Main_Menu_Update, Screen_Exit);
}

void Screen_Exit(void)
{
    CP_Font_Free(Font);
}

/*
The functions below are the elements or functionalities of each updated screen*/

void drawBtn(float y, float w, float h, const char* text, float scalar)
{
    float rect_x = CP_System_GetWindowWidth() / 2.0;
    drawTextCenter(text, rect_x, y, scalar);
    CP_Settings_Fill(CP_Color_Create(255, 100, 100, 100));
    CP_Graphics_DrawRect(rect_x, y, w, h);
}

void drawTextCenter(const char* text, float x, float y, float scalar)
{
    CP_Settings_TextSize(textSize * scalar);
    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
    CP_Font_DrawText(text, x, y);
}

/*
This function checks if the mouse is hovering over a button area
and scales the button accordingly.
If the button is clicked, it transitions to the game state.      */
void dynamicBtn(const char* text, float rect_y, void* init(void), void* update(void), void* exit(void)) {
    if (IsAreaClicked(CP_System_GetWindowWidth() / 2.0, rect_y, rect_width, rect_height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
        scalar = 1.2f;
        drawBtn(rect_y, rect_width * scalar, rect_height * scalar, text, scalar);
        if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT))
            CP_Engine_SetNextGameStateForced(init, update, exit);
    }
    else {
        scalar = 1.0f;
        drawBtn(rect_y, rect_width, rect_height, text, scalar);
    }
}

// Exclusively for exit to desktop button
void Exit_dynamicBtn(const char* text, float rect_y) {
    if (IsAreaClicked(CP_System_GetWindowWidth() / 2.0, rect_y, rect_width, rect_height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
        scalar = 1.2f;
        drawBtn(rect_y, rect_width * scalar, rect_height * scalar, text, scalar);
        if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT))
            CP_Engine_Terminate();
    }
    else {
        scalar = 1.0f;
        drawBtn(rect_y, rect_width, rect_height, text, scalar);
    }
}
