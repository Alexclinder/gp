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
#include "npc_mechanics.h"
#include "colors.h"

// Global variables are required for constant updates.
float textSize= 50.0f;
CP_Image mainbackground;
CP_Image frame;
CP_Image levelselection;
CP_Image defeat;
CP_Image pausemenu;
CP_Font Font;


float first_y;
float second_y;
float third_y;
float fourth_y;

float rect_width = 400.0f;
float rect_height = 100.0f;
float scalar;

void Level_1_Init(void) { level_selected = level_1; TimeSet = 0;ResetTime = 1; Dash_count = 3;  Game_Init(); Player_Starting_Pos(player_spawns, level_1); NPC_Starting_Pos(NPC_1_spawns, level_selected); }
void Level_2_Init(void) { level_selected = level_2; TimeSet = 0;ResetTime = 1; Dash_count = 3; Game_Init(); Player_Starting_Pos(player_spawns, level_2); NPC_Starting_Pos(NPC_1_spawns, level_selected);}
void Level_3_Init(void) { level_selected = level_3; TimeSet = 0;ResetTime = 1; Dash_count = 3;  Game_Init(); Player_Starting_Pos(player_spawns, level_3); NPC_Starting_Pos(NPC_1_spawns, level_selected);}
void Level_4_Init(void) { level_selected = level_4; TimeSet = 0;ResetTime = 1; Dash_count = 3;  Game_Init(); Player_Starting_Pos(player_spawns, level_4); NPC_Starting_Pos(NPC_1_spawns, level_selected);}

void Screen_Init(void)
{
    // Use to initalize the color variables with values stored
    color_create();

	mainbackground = CP_Image_Load("Assets/MainMenu.png");
	defeat = CP_Image_Load("Assets/Defeat.png");
    levelselection = CP_Image_Load("Assets/LevelSelect.png");
    frame = CP_Image_Load("Assets/buttonframe.png");
    pausemenu = CP_Image_Load("Assets/pause.png");
    Font = CP_Font_Load("Assets/SuperAdorable-MAvyp.ttf");
    CP_Font_Set(Font);
    PauseTimeCount = 1;
}

void Next_level_Init(void) {
    Game_Next_Level();
    
    Game_Reset();
}

void Main_Menu_Update(void) {
    static float anim_time = 0.0f;   // keeps its value between frames
    float rect_x = 320.0f;

    CP_Graphics_ClearBackground(CP_Color_Create(100, 100, 100, 0));
    CP_Image_Draw(mainbackground, CP_System_GetWindowWidth() / 2.0, CP_System_GetWindowHeight() / 2.0, CP_System_GetWindowWidth(), CP_System_GetWindowHeight(), 255);
    level_selected = 0;

    // ---------- Slide-in timing ----------
    anim_time += CP_System_GetDt();

    float duration = 1.0f;     // seconds each element takes to slide in
    float distance = 600.0f;   // how far off-screen they start

    // Title (no delay)
    float p = anim_time / duration;
    if (p > 1.0f) p = 1.0f;
    float title_offset = -distance * (1.0f - p) * (1.0f - p) * (1.0f - p);

    // Start Game (0.5s delay)
    p = (anim_time - 0.5f) / duration;
    if (p < 0.0f) p = 0.0f;
    if (p > 1.0f) p = 1.0f;
    float start_offset = -distance * (1.0f - p) * (1.0f - p) * (1.0f - p);

    // Controls (0.75s delay)
    p = (anim_time - 0.75f) / duration;
    if (p < 0.0f) p = 0.0f;
    if (p > 1.0f) p = 1.0f;
    float middle_offset = -distance * (1.0f - p) * (1.0f - p) * (1.0f - p);

    // Exit to Desktop (1s delay)
    p = (anim_time - 1.0f ) / duration;
    if (p < 0.0f) p = 0.0f;
    if (p > 1.0f) p = 1.0f;
    float exit_offset = -distance * (1.0f - p) * (1.0f - p) * (1.0f - p);
    // -------------------------------------

    // ---------- Title ----------
    CP_Settings_Save();   // so these text settings don't affect your buttons
    // CP_Font_Set(titleFont);   // uncomment if you load a bold title font
    CP_Settings_NoStroke();
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_MIDDLE);

    // "THE"
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
    CP_Settings_TextSize(45.0f);
    CP_Font_DrawText("THE", 125.0f + title_offset, 80.0f);

    // "OFFICE"
    CP_Settings_TextSize(95.0f);
    CP_Font_DrawText("OFFICE", 120.0f + title_offset, 160.0f);

    // "RUN"
    CP_Settings_Fill(CP_Color_Create(255, 204, 51, 255));
    CP_Font_DrawText("RUN", 120.0f + title_offset, 245.0f);

    // Red underline
    CP_Settings_Fill(CP_Color_Create(230, 80, 70, 255));
    CP_Graphics_DrawRect(120.0f + title_offset, 292.0f, 340.0f, 6.0f);

    CP_Settings_Restore();
    // ---------------------------

    // Start Game
    first_y = 390.0f;
    // Controls
    second_y = first_y * 1.5f;
    // Exit to Desktop
    third_y = first_y * 2.0f;


    CP_Settings_Fill(orange);
    dynamicBtn("Start Game!", rect_x + start_offset, first_y, Screen_Init, Level_Select_Screen, Screen_Exit);
    CP_Settings_Fill(light_sky_blue);
    dynamicBtn("Controls", rect_x + middle_offset, second_y, Screen_Init, Controls_Screen, Screen_Exit);
    CP_Settings_Fill(dark_blue);
    Exit_dynamicBtn("Exit to Desktop", rect_x + exit_offset, third_y);
}

void Pause_Menu_Update(void)
{
    float rect_x = 250.0f;
    CP_Graphics_ClearBackground(CP_Color_Create(100, 200, 100, 0));
    CP_Image_Draw(pausemenu, CP_System_GetWindowWidth() / 2.0, CP_System_GetWindowHeight() / 2.0, CP_System_GetWindowWidth(), CP_System_GetWindowHeight(), 255);
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
    CP_Settings_TextSize(100.0f);
    CP_Font_DrawText("PAUSED", 250.0f, 80.0f);

    // Slice the screen into 3 sections.....
    // ...1/3 will be for the pause menu text
	// ...2/3 will be for the buttons layout
	float two_third_section = CP_System_GetWindowHeight() + 700.0f;
    float y_offset = 0.13f;
	// 1/5 = 20% of the 2/3 section for each button
    // Resume
    first_y = two_third_section * y_offset;
    // Restart
    second_y = two_third_section * (y_offset * 2);
	// Controls
	third_y = two_third_section * (y_offset * 3);
    // Exit to Main Menu
    fourth_y = two_third_section * (y_offset * 4);


    CP_Settings_Fill(light_sky_blue);
    dynamicBtn("Resume Game!", rect_x, first_y, Game_Init, Game_Update, Game_Exit);
    CP_Settings_Fill(light_sky_blue);
    dynamicBtn("Restart Level", rect_x, second_y, Game_Reset, Game_Update, Game_Exit);
    CP_Settings_Fill(light_sky_blue);
    dynamicBtn("Controls", rect_x, third_y, Screen_Init, Controls_Screen, Screen_Exit);
    CP_Settings_Fill(light_sky_blue);
    dynamicBtn("Exit to Main Menu", rect_x, fourth_y, Screen_Init, Main_Menu_Update, Screen_Exit);

}

void Win_Screen(void) {
    float rect_x = CP_System_GetWindowWidth() / 2.0;
    CP_Graphics_ClearBackground(CP_Color_Create(100, 200, 100, 0));


    /*
    char score_buf[100] = { 0 };
    sprintf_s(score_buf, 100, "Score: %3d", score);
    // move the actual score value to the right of Score:
    drawTextCenter(score_buf, rect_x + 100.0f, 200.0f, 1.0f);
    */
  
    CP_Font_Set(Font);
    CP_Settings_TextSize(50);
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_TOP);
    CP_Settings_Fill(black);
    char buffer[16] = { 0 };
    sprintf_s(buffer, 16, "Time: %.2f", Time);

    CP_Font_DrawText("Well Done", rect_x, 100.0f);
    CP_Font_DrawText(buffer, rect_x, 150.0f);

    // Next Level
    first_y = CP_System_GetWindowHeight() / 2.0 - 120.0f;
    // Restart
    second_y = CP_System_GetWindowHeight() / 2.0 - 30.0f;
    // Exit to Main Menu
    third_y = CP_System_GetWindowHeight() / 2.0 + 120.0f;
    
    CP_Settings_Fill(orange);
    dynamicBtn("Next Level", rect_x, first_y, Next_level_Init, Game_Update, Game_Exit);
    CP_Settings_Fill(orange);
    dynamicBtn("Restart Level", rect_x, second_y, Game_Reset, Game_Update, Game_Exit);
    CP_Settings_Fill(orange);
    dynamicBtn("Exit to Main Menu", rect_x, third_y, Screen_Init, Main_Menu_Update, Screen_Exit);

}

void Lose_Screen(void) {
    float rect_y = CP_System_GetWindowHeight() / 2.0;
    CP_Graphics_ClearBackground(CP_Color_Create(100, 100, 100, 0));
    CP_Image_Draw(defeat, CP_System_GetWindowWidth() / 2.0, CP_System_GetWindowHeight() / 2.0, CP_System_GetWindowWidth(), CP_System_GetWindowHeight(), 255);

	// Restart Level
    first_y = CP_System_GetWindowHeight() / 2.0 - 150.0f;
	// Exit to Main Menu
    second_y = CP_System_GetWindowHeight() / 2.0 + 150.0f;
    CP_Settings_Fill(translucent_light_red);
    dynamicBtn("Restart Level", 300, rect_y, Game_Reset, Game_Update, Game_Exit);
    CP_Settings_Fill(translucent_light_red);
    dynamicBtn("Exit to Main Menu", 1300, rect_y, Screen_Init, Main_Menu_Update, Screen_Exit);

    
}

void Leaderboard_Screen(void) {
    float rect_x = CP_System_GetWindowWidth() / 2.0;
    CP_Graphics_ClearBackground(CP_Color_Create(100, 140, 200, 0));

    // Restart Level
    first_y = CP_System_GetWindowHeight() / 2.0 - 150.0f;
    // Exit to Main Menu
    second_y = CP_System_GetWindowHeight() / 2.0 + 150.0f;
    CP_Settings_Fill(orange);
    dynamicBtn("Level Select", rect_x, first_y, Screen_Init, Level_Select_Screen, Screen_Exit);
    CP_Settings_Fill(orange);
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
    CP_Settings_Fill(orange);
    dynamicBtn("Leaderboard", rect_x, first_y, Screen_Init, Leaderboard_Screen, Screen_Exit);
    CP_Settings_Fill(orange);
    dynamicBtn("Level Select", rect_x, second_y, Screen_Init, Level_Select_Screen, Screen_Exit);
    CP_Settings_Fill(orange);
    dynamicBtn("Exit to Main Menu", rect_x, third_y, Screen_Init, Main_Menu_Update, Screen_Exit);
}

static float level_anim_time = 0.0f;   // put this at the top of the file

void Level_Select_Screen(void) {
    CP_Graphics_ClearBackground(CP_Color_Create(80, 80, 80, 0));
    CP_Image_Draw(levelselection, CP_System_GetWindowWidth() / 2.0, CP_System_GetWindowHeight() / 2.0, CP_System_GetWindowWidth(), CP_System_GetWindowHeight(), 255);
    // ---------- Slide-in timing ----------
    level_anim_time += CP_System_GetDt();

    float duration = 0.6f;     // seconds each element takes to slide in
    float distance = 600.0f;   // how far off-screen they start
    float delay = 0.25f;    // gap between each element

    // 0 = title, 1-4 = levels, 5 = back button
    float offset[6];
    for (int i = 0; i < 6; i++) {
        float p = (level_anim_time - i * delay) / duration;
        if (p < 0.0f) p = 0.0f;
        if (p > 1.0f) p = 1.0f;
        offset[i] = -distance * (1.0f - p) * (1.0f - p) * (1.0f - p);
    }
    // -------------------------------------

    float btn_x = 320.0f;

    drawTextCenter("SELECT A LEVEL", btn_x + offset[0], 100.0f, 1.2f);

    // Level buttons (stacked on the left)
    CP_Settings_Fill(orange);
    dynamicBtn("Level 1", btn_x + offset[1], 230.0f, Level_1_Init, Game_Update, Game_Exit);
    CP_Settings_Fill(orange);
    dynamicBtn("Level 2", btn_x + offset[2], 350.0f, Level_2_Init, Game_Update, Game_Exit);
    CP_Settings_Fill(orange);
    dynamicBtn("Level 3(HARD)", btn_x + offset[3], 470.0f, Level_3_Init, Game_Update, Game_Exit);
    CP_Settings_Fill(orange);
    dynamicBtn("Level 4", btn_x + offset[4], 590.0f, Level_4_Init, Game_Update, Game_Exit);

    // Back to Menu (extra gap below the levels)
    third_y = 740.0f;
    CP_Settings_Fill(orange);
    dynamicBtn("Back to Menu", btn_x + offset[5], third_y, Screen_Init, Main_Menu_Update, Screen_Exit);
}

#define NUMBER_OF_CONTROLS 7

void Controls_Screen(void) {
    CP_Graphics_ClearBackground(light_sky_blue);

    const char* controls_text[NUMBER_OF_CONTROLS] =
    {
        "JUMP",
        "DOWN",
        "LEFT",
        "RIGHT",
        "KICK",
        "PICK UP / DROP",
        "PAUSE/BACK"
    };
    // x, y, w, h, text, scalar

    const char* control_keys[NUMBER_OF_CONTROLS] =
    {
        "W",
        "S",
        "A",
        "D",
        "F",
        "SPACE",
        "ESC"
    };

    float first_control_y = 100.0f;
    float first_control_x = 550.0f;
    float y_offset = 100.0f; // vertical spacing between controls
    
    float first_controlkey_x = first_control_x * 2.0f;

    const float controls_rect_width = 400.0f;
    const float controlskeys_rect_width = 150.0f;
    const float control_rect_height = 70.0f;

	// Draw the boxes and text for each control and corresponding key
    for (int i = 0; i < NUMBER_OF_CONTROLS; i++) {
		CP_Settings_Fill(translucent_light_red);
        drawBtn(first_control_x, first_control_y + (i * y_offset), controls_rect_width, control_rect_height, controls_text[i], 1.0f);
        CP_Settings_Fill(translucent_light_red);
        drawBtn(first_controlkey_x, first_control_y + (i * y_offset), controlskeys_rect_width, control_rect_height, control_keys[i], 1.0f);
    }

	// Back to Menu button
	float control_backmenu_btn_y = CP_System_GetWindowHeight() - 70.0f;
	float control_backmenu_btn_x = CP_System_GetWindowWidth() / 2.0f;
    CP_Settings_Fill(orange);

    if (level_selected == main_menu) {
        dynamicBtn("Back to Menu", control_backmenu_btn_x, control_backmenu_btn_y, Screen_Init, Main_Menu_Update, Screen_Exit);
    }
    else {
        dynamicBtn("Back to Menu", control_backmenu_btn_x, control_backmenu_btn_y, Screen_Init, Pause_Menu_Update, Screen_Exit);
    }
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
    //CP_Settings_Fill(translucent_light_red);
    CP_Graphics_DrawRect(x, y, w, h);
    drawTextCenter(text, x, y, scalar);
}

void drawTextCenter(const char* text, float x, float y, float scalar)
{
    Font = CP_Font_Load("Assets/Exo2-Regular.ttf");
    CP_Settings_TextSize(textSize * scalar);
    CP_Settings_Fill(white);
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
    CP_Font_DrawText(text, x, y);
}

void drawTextTopLeft(const char* text, float x, float y, float scalar)
{
    Font = CP_Font_Load("Assets/Exo2-Regular.ttf");
    CP_Settings_TextSize(textSize * scalar);
    CP_Settings_Fill(white);
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_TOP);
    CP_Font_DrawText(text, x, y);
}

void drawScoreTopLeft(const int* text, float x, float y, float scalar)
{
    Font = CP_Font_Load("Assets/Exo2-Regular.ttf");
    CP_Settings_TextSize(textSize * scalar);
    CP_Settings_Fill(white);
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_TOP);
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
        CP_Settings_ImageMode(CP_POSITION_CENTER);
        CP_Image_Draw(frame, rect_x, rect_y, rect_width * scalar, rect_height * scalar, 255);
        if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT))
            CP_Engine_SetNextGameStateForced(init, update, exit);
    }
    else {
        scalar = 1.0f;
        drawBtn(rect_x, rect_y, rect_width, rect_height, text, scalar);
        CP_Settings_ImageMode(CP_POSITION_CENTER);
        CP_Image_Draw(frame, rect_x, rect_y, rect_width, rect_height, 255);
    }
}

// Exclusively for exit to desktop button
void Exit_dynamicBtn(const char* text, float rect_x, float rect_y) {
    if (IsAreaClicked(rect_x, rect_y, rect_width, rect_height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
        scalar = 1.2f;
        drawBtn(rect_x, rect_y, rect_width * scalar, rect_height * scalar, text, scalar);
        CP_Settings_ImageMode(CP_POSITION_CENTER);
        CP_Image_Draw(frame, rect_x, rect_y, rect_width * scalar, rect_height * scalar, 255);
        if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT))
            CP_Engine_Terminate();
    }
    else {
        scalar = 1.0f;
        drawBtn(rect_x, rect_y, rect_width, rect_height, text, scalar);
        CP_Settings_ImageMode(CP_POSITION_CENTER);
        CP_Image_Draw(frame, rect_x, rect_y, rect_width, rect_height, 255);
    }
}
