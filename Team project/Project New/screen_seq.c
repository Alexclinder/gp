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

void Level_1_Init(void) { level_selected = 0;  Game_Init(); Starting_Pos(player_spawns, 0); }
void Level_2_Init(void) { level_selected = 1;  Game_Init(); Starting_Pos(player_spawns, 1); }
void Level_3_Init(void) { level_selected = 2;  Game_Init(); Starting_Pos(player_spawns, 2); }
void Level_4_Init(void) { level_selected = 3;  Game_Init(); Starting_Pos(player_spawns, 3); }

void Screen_Init(void)
{
    Font = CP_Font_Load("Assets/SuperAdorable-MAvyp.ttf");
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
    float rect_x = CP_System_GetWindowWidth() / 2.0;
    CP_Graphics_ClearBackground(CP_Color_Create(100, 100, 100, 0));
    level_selected = 0;

    // Start Game
	first_y = CP_System_GetWindowHeight() / 2.0 - 150.0f;
	// Exit to Desktop
	second_y = CP_System_GetWindowHeight() / 2.0 + 150.0f;
    
    dynamicBtn("Start Game!", rect_x, first_y, Screen_Init, Level_Select_Screen, Screen_Exit);
    Exit_dynamicBtn("Exit to Desktop", rect_x, second_y);
}

void Pause_Menu_Update(void)
{
    float rect_x = CP_System_GetWindowWidth() / 2.0;
    CP_Graphics_ClearBackground(CP_Color_Create(100, 200, 100, 0));

    // Resume
    first_y = CP_System_GetWindowHeight() / 2.0 - 150.0f;
    // Restart
    second_y = CP_System_GetWindowHeight() / 2.0; 
	// Exit to Main Menu
    third_y = CP_System_GetWindowHeight() / 2.0 + 150.0f;
    
    dynamicBtn("Resume Game!", rect_x, first_y, Game_Init, Game_Update, Game_Exit);
    dynamicBtn("Restart Level", rect_x, second_y, Game_Reset, Game_Update, Game_Exit);
    dynamicBtn("Exit to Main Menu", rect_x, third_y, Screen_Init, Main_Menu_Update, Screen_Exit);

    
}

void Win_Screen(void) {
    float rect_x = CP_System_GetWindowWidth() / 2.0;
    CP_Graphics_ClearBackground(CP_Color_Create(100, 200, 100, 0));

    // Next Level
    first_y = CP_System_GetWindowHeight() / 2.0 - 150.0f;
    // Restart
    second_y = CP_System_GetWindowHeight() / 2.0;
    // Exit to Main Menu
    third_y = CP_System_GetWindowHeight() / 2.0 + 150.0f;
    
    
    dynamicBtn("Next Level", rect_x, first_y, Next_level_Init, Game_Update, Game_Exit);
    dynamicBtn("Restart Level", rect_x, second_y, Game_Reset, Game_Update, Game_Exit);
    dynamicBtn("Exit to Main Menu", rect_x, third_y, Screen_Init, Main_Menu_Update, Screen_Exit);

    
}

void Lose_Screen(void) {
    float rect_x = CP_System_GetWindowWidth() / 2.0;
    CP_Graphics_ClearBackground(CP_Color_Create(100, 100, 100, 0));

	// Restart Level
    first_y = CP_System_GetWindowHeight() / 2.0 - 150.0f;
	// Exit to Main Menu
    second_y = CP_System_GetWindowHeight() / 2.0 + 150.0f;

    dynamicBtn("Restart Level", rect_x, first_y, Game_Reset, Game_Update, Game_Exit);
    dynamicBtn("Exit to Main Menu", rect_x, second_y, Screen_Init, Main_Menu_Update, Screen_Exit);

    
}

void Leaderboard_Screen(void) {
    float rect_x = CP_System_GetWindowWidth() / 2.0;
    CP_Graphics_ClearBackground(CP_Color_Create(100, 140, 200, 0));

    // Restart Level
    first_y = CP_System_GetWindowHeight() / 2.0 - 150.0f;
    // Exit to Main Menu
    second_y = CP_System_GetWindowHeight() / 2.0 + 150.0f;

    dynamicBtn("Level Select", rect_x, first_y, Screen_Init, Level_Select_Screen, Screen_Exit);
    dynamicBtn("Exit to Main Menu", rect_x, second_y, Screen_Init, Main_Menu_Update, Screen_Exit);

}

void Completed_Game_Screen(void) {
    float rect_x = CP_System_GetWindowWidth() / 2.0;
    CP_Graphics_ClearBackground(CP_Color_Create(100, 200, 100, 0));

    // Next Level
    first_y = CP_System_GetWindowHeight() / 2.0 - 150.0f;
    // Restart
    second_y = CP_System_GetWindowHeight() / 2.0;
    // Exit to Main Menu
    third_y = CP_System_GetWindowHeight() / 2.0 + 150.0f;

    dynamicBtn("Leaderboard", rect_x, first_y, Screen_Init, Leaderboard_Screen, Screen_Exit);
    dynamicBtn("Level Select", rect_x, second_y, Screen_Init, Level_Select_Screen, Screen_Exit);
    dynamicBtn("Exit to Main Menu", rect_x, third_y, Screen_Init, Main_Menu_Update, Screen_Exit);
}

void Level_Select_Screen(void) {
        CP_Graphics_ClearBackground(CP_Color_Create(80, 80, 80, 0));

        float center_x = CP_System_GetWindowWidth()/2.0f;
        float center_y = CP_System_GetWindowHeight()/2.0f;

        drawTextCenter("SELECT A LEVEL", center_x, center_y - 250.0f, 1.2f);

        // Level Button Positions
        float col1_x = center_x - 250.0f;
        float col2_x = center_x + 250.0f;

        float row1_y = center_y - 80.0f;
        float row2_y = center_y + 80.0f;


        // Render in 2 by 2 matrix
        /*
        
        btn 1 , btn 2
        btn 3,  btn 4
        
        1,1 1,2
        2,1 2,2
        */

        // Level 1
        dynamicBtn("Level 1", col1_x, row1_y, Level_1_Init, Game_Update, Game_Exit);
     
        // Level 2
        dynamicBtn("Level 2", col2_x, row1_y, Level_2_Init, Game_Update, Game_Exit);

        // Level 3
        dynamicBtn("Level 3", col1_x, row2_y, Level_3_Init, Game_Update, Game_Exit);

        // Level 4
        dynamicBtn("Level 4", col2_x, row2_y, Level_4_Init, Game_Update, Game_Exit);

        third_y = center_y + 220.0f;
        float menu_btn_x = center_x;

        dynamicBtn("Back to Menu", menu_btn_x, third_y, Screen_Init, Main_Menu_Update, Screen_Exit);
    }

void Screen_Exit(void)
{
    CP_Font_Free(Font);
}

/*
The functions below are the elements or functionalities of each updated screen*/

void drawBtn(float x, float y, float w, float h, const char* text, float scalar)
{
    CP_Settings_RectMode(CP_POSITION_CENTER);
    CP_Settings_Fill(CP_Color_Create(255, 100, 100, 100));
    CP_Graphics_DrawRect(x, y, w, h);

    drawTextCenter(text, x, y, scalar);
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
void dynamicBtn(const char* text, float rect_x, float rect_y, void* init(void), void* update(void), void* exit(void)) {
    if (IsAreaClicked(rect_x, rect_y, rect_width, rect_height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
        scalar = 1.2f;
        drawBtn(rect_x, rect_y, rect_width * scalar, rect_height * scalar, text, scalar);
        if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT))
            CP_Engine_SetNextGameStateForced(init, update, exit);
    }
    else {
        scalar = 1.0f;
        drawBtn(rect_x, rect_y, rect_width, rect_height, text, scalar);
    }
}

// Exclusively for exit to desktop button
void Exit_dynamicBtn(const char* text, float rect_x, float rect_y) {
    if (IsAreaClicked(rect_x, rect_y, rect_width, rect_height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
        scalar = 1.2f;
        drawBtn(rect_x, rect_y, rect_width * scalar, rect_height * scalar, text, scalar);
        if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT))
            CP_Engine_Terminate();
    }
    else {
        scalar = 1.0f;
        drawBtn(rect_x, rect_y, rect_width, rect_height, text, scalar);
    }
}
