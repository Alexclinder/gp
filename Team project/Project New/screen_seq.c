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

// Global variables are required for constant updates.
float textSize= 50.0f;
CP_Font Font;

float first_y;
float second_y;
float third_y;

float rect_width = 200.0f;
float rect_height = 100.0f;
float scalar;


void Screen_Init(void)
{
    Font = CP_Font_Load("Assets/Exo2-Regular.ttf");
    CP_Font_Set(Font);
}

void drawRect(float y, float w, float h, const char* text, float scalar)
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
void dynamicBtn(const char* text, float rect_y, void *init(void), void *update(void), void *exit(void)) {
    if (IsAreaClicked(CP_System_GetWindowWidth() / 2.0, rect_y, rect_width, rect_height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
        scalar = 1.2f;
        drawRect(rect_y, rect_width * scalar, rect_height * scalar, text, scalar);
        if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT))
            CP_Engine_SetNextGameStateForced(init, update, exit);
    }
    else {
        scalar = 1.0f;
        drawRect(rect_y, rect_width, rect_height, text, scalar);
    }
}

// Exclusively for exit to desktop button
void Exit_dynamicBtn(const char* text, float rect_y) {
    if (IsAreaClicked(CP_System_GetWindowWidth() / 2.0, rect_y, rect_width, rect_height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
        scalar = 1.2f;
        drawRect(rect_y, rect_width * scalar, rect_height * scalar, text, scalar);
        if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT))
            CP_Engine_Terminate();
    }
    else {
        scalar = 1.0f;
        drawRect(rect_y, rect_width, rect_height, text, scalar);
    }
}


void Main_Menu_Update(void) {
	first_y = CP_System_GetWindowHeight() / 2.0 - 150.0f;
	second_y = CP_System_GetWindowHeight() / 2.0 + 150.0f;
	CP_Graphics_ClearBackground(CP_Color_Create(100, 100, 100, 0));
    dynamicBtn("Start", first_y, Game_Reset, Game_Update, Game_Exit);
    Exit_dynamicBtn("Exit", second_y);
}


void Pause_Menu_Update(void)
{
    first_y = CP_System_GetWindowHeight() / 2.0 - 150.0f;
    second_y = CP_System_GetWindowHeight() / 2.0;
    third_y = CP_System_GetWindowHeight() / 2.0 + 150.0f;
	CP_Graphics_ClearBackground(CP_Color_Create(100, 200, 100, 0));
    dynamicBtn("Resume", first_y, Game_Init, Game_Update, Game_Exit);
    dynamicBtn("Restart", second_y, Game_Reset, Game_Update, Game_Exit);
    dynamicBtn("Exit", third_y, Screen_Init, Main_Menu_Update, Screen_Exit);
}

void Screen_Exit(void)
{
    CP_Font_Free(Font);
}