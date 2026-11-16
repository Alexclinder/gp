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
#include "sounds.h"
#include "scoring.h"
#include "Images.h"

// Global variables are required for constant updates.
float textSize= 50.0f;

float first_y;
float second_y;
float third_y;
float fourth_y;

float rect_width = 400.0f;
float rect_height = 100.0f;
float scalar;

/*******Transition Sequences*******/
void Screen_Init(void)
{
    //Use to load all the sounds to be used in the game
    LoadSounds();
    // Use to initalize the color variables with values stored
    color_create();
    // Use to initalize the Assets Images used in the game
    LoadImages();

    CP_Font_Set(Font);

    PauseTimeStatus = 1;
}

void Screen_Exit(void)
{

    CP_Image_Free(mainbackground);
    CP_Image_Free(defeat);
    CP_Image_Free(levelselection);
    CP_Image_Free(frame);
    CP_Image_Free(pausemenu);
    CP_Font_Free(Font);
}

void Next_level_Init(void) {
    Game_Next_Level();

    
    Game_Reset();
}

void Exit_Main_Menu_Init(void)
{
    // prevents from constantly incrementing 
    if (!level_score_locked) {
        player_score += level_score;
        level_score_locked = 1;
    }

    Screen_Init();
}



void ResetALLTimers_Status(void) {
    TimeSet = 0;
    ResetTime = 1;
    Teleport_count = 3;
}
void ResetALLTimers_StatusBossLvl(void) {
    TimeSet = 0;
    ResetTime = 1;
    Teleport_count = 4;//Set as four but represents unlimited teleports
}
/***********************Level Initialization Functions******************/
// score_reset reset level score but not the player_score (cumulative of all level_score)
void Level_1_Init(void) { level_selected = level_1; ResetALLTimers_Status();  Game_Init(); Player_Starting_Pos(player_spawns, level_1); NPC_Starting_Pos(NPC_1_spawns, level_selected);  }
void Level_2_Init(void) { level_selected = level_2; ResetALLTimers_Status();  Game_Init(); Player_Starting_Pos(player_spawns, level_2); NPC_Starting_Pos(NPC_1_spawns, level_selected);  }
void Level_3_Init(void) { level_selected = level_3; ResetALLTimers_Status();  Game_Init(); Player_Starting_Pos(player_spawns, level_3); NPC_Starting_Pos(NPC_1_spawns, level_selected);  }
void Level_4_Init(void) { level_selected = level_4; ResetALLTimers_StatusBossLvl();  Game_Init(); Player_Starting_Pos(player_spawns, level_4); NPC_Starting_Pos(NPC_1_spawns, level_selected);  }



/*****************************Main Menu Screens*******************************/
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

    CP_Settings_Fill(dark_blue);
	dynamicBtn("Leaderboard", 1400.0, 745.0, Screen_Init, Leaderboard_Screen, Screen_Exit);//Temporary Leaderboard button for testing purposes Removed if not needed
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
    dynamicBtn("Exit to Main Menu", rect_x, fourth_y, Exit_Main_Menu_Init, Main_Menu_Update, Screen_Exit);
 
}

void Win_Screen(void) {
    float rect_x = CP_System_GetWindowWidth() / 2.0;
    CP_Graphics_ClearBackground(CP_Color_Create(100, 200, 100, 0));

    char score_buf[SCORE_BUF_SIZE] = { 0 };
    sprintf_s(score_buf, SCORE_BUF_SIZE, "Score: %d", level_score);
    // move the actual score value to the right of Score:
    drawTextCenter(score_buf, rect_x, 300.0f, 1.0f);
     
    CP_Font_Set(Font);
    CP_Settings_TextSize(50);
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_TOP);
    CP_Settings_Fill(black);
    char buffer[TIME_BUF_SIZE] = { 0 };
    sprintf_s(buffer, TIME_BUF_SIZE, "Time: %.2f", WinScreenTime);

    CP_Font_DrawText("Well Done", rect_x, 100.0f);
    CP_Font_DrawText(buffer, rect_x, 150.0f);

    // Need larger size than 16 chars, otherwise game will crash
    char total_score_buf[SCORE_BUF_SIZE*2] = { 0 };
    sprintf_s(total_score_buf, SCORE_BUF_SIZE*2, "Total Score: %d", player_score);

    CP_Font_DrawText(total_score_buf, rect_x, 200.0f);

    // Next Level
    first_y = CP_System_GetWindowHeight() / 2.0;
    // Restart
    second_y = CP_System_GetWindowHeight() / 2.0 +150.0f;
    // Exit to Main Menu
    third_y = CP_System_GetWindowHeight() / 2.0 + 300.0f;
    
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

    float left_offset = 100.0f;
    float right_offset = 500.0f;

    float top_timer_y = 300.0f;
    float bottom_timer_y = 700.0f;

    float score_offset_y = 50.0f;

    float timer_score_offset_x = 300.0f;
    float timer_score_top_y = 200.0f;
    float timer_score_bot_y = 600.0f;

    float timer_score_w = 400.0f;

    CP_Settings_TextSize(textSize);
    //Level 1 Leaderboard Time 
    CP_Font_DrawTextBox("Level 1 \nScore & Time", rect_x - left_offset, timer_score_top_y, timer_score_w);
    char Level1_Time[TIME_BUF_SIZE] = { 0 };
    sprintf_s(Level1_Time, TIME_BUF_SIZE, "Time: %.2f", Level_Timer.level1_time);
    CP_Font_DrawText(Level1_Time, rect_x + left_offset, top_timer_y);

    char Level1_Score[SCORE_BUF_SIZE] = { 0 };
    sprintf_s(Level1_Score, SCORE_BUF_SIZE, "Score: %d", Level_Score.level1_score);
    CP_Font_DrawText(Level1_Score, rect_x + left_offset, top_timer_y + score_offset_y);

    //Level 2 Leaderboard Time
    CP_Font_DrawTextBox("Level 2 \nScore & Time", rect_x + timer_score_offset_x, timer_score_top_y, timer_score_w);
    char Level2_Time[TIME_BUF_SIZE] = { 0 };
    sprintf_s(Level2_Time, TIME_BUF_SIZE, "Time: %.2f", Level_Timer.level2_time);
    CP_Font_DrawText(Level2_Time, rect_x + right_offset, top_timer_y);

    char Level2_Score[SCORE_BUF_SIZE] = { 0 };
    sprintf_s(Level2_Score, SCORE_BUF_SIZE, "Score: %d", Level_Score.level2_score);
    CP_Font_DrawText(Level2_Score, rect_x + right_offset, top_timer_y + score_offset_y);

    //Level 3 Leaderboard Time
    CP_Font_DrawTextBox("Level 3 \nScore & Time", rect_x - left_offset, timer_score_bot_y, timer_score_w);
    char Level3_Time[TIME_BUF_SIZE] = { 0 };
    sprintf_s(Level3_Time, TIME_BUF_SIZE, "Time: %.2f", Level_Timer.level3_time);
    CP_Font_DrawText(Level3_Time, rect_x + left_offset, bottom_timer_y);

    char Level3_Score[SCORE_BUF_SIZE] = { 0 };
    sprintf_s(Level3_Score, SCORE_BUF_SIZE, "Score: %d", Level_Score.level3_score);
    CP_Font_DrawText(Level3_Score, rect_x + left_offset, bottom_timer_y + score_offset_y);

    //Level 4 Leaderboard Time
    CP_Font_DrawTextBox("Level 4 \nScore & Time", rect_x + timer_score_offset_x, timer_score_bot_y, timer_score_w);

    // @ToDo Lewin, add time here

    char Level4_Score[SCORE_BUF_SIZE] = { 0 };
    sprintf_s(Level4_Score, SCORE_BUF_SIZE, "Score: %d", Level_Score.level4_score);
    CP_Font_DrawText(Level4_Score, rect_x + right_offset, bottom_timer_y + score_offset_y);

    // Total Score
    CP_Font_DrawTextBox("Total Score", rect_x + timer_score_offset_x, timer_score_bot_y, timer_score_w);

    // @ToDo Lewin, add time here

    // player_score buf needs to be more than 16 char, or game will crash
    char Player_Score[SCORE_BUF_SIZE * 2] = { 0 };
    sprintf_s(Player_Score, SCORE_BUF_SIZE * 2, "Total Score: %d", player_score);
    CP_Font_DrawText(Player_Score, rect_x - right_offset, first_y = CP_System_GetWindowHeight() / 2.0 - 300.0f);


    // Restart Level
    first_y = CP_System_GetWindowHeight() / 2.0 - 150.0f;
    // Exit to Main Menu
    second_y = CP_System_GetWindowHeight() / 2.0 + 150.0f;
    CP_Settings_Fill(orange);
    dynamicBtn("Level Select", rect_x - right_offset, first_y, Screen_Init, Level_Select_Screen, Screen_Exit);
    CP_Settings_Fill(orange);
    dynamicBtn("Exit to Main Menu", rect_x - right_offset, second_y, Screen_Init, Main_Menu_Update, Screen_Exit);




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

enum { NUMBER_OF_CONTROLS = 7 };
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



// Score: = 6 chars, 1 white space, 
// assume score can reach up to 99,999.00 (excluding comma - 8 char) 
// 1 null terminator - 6 + 1 + 8 + 1 = 16

/*
The functions below are the elements or functionalities of each updated screen*/

/*********************Objective & Score Text Position Functions*****************/
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


/*********************Button Functions****************************/
int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y)
{
    // check if the click is within 
    // x bound of the area(check the x-axis radius from the centre of the rectangle(x coordinate - (width/2) && x coordinate + (width/2)))
    // y bound of the area(check the y-axis radius from the centre of the rectangle(y coordinate - (length/2) && x coordinate + (length/2)))
    if ((click_x >= area_center_x - area_width / 2 && click_x <= area_center_x + area_width / 2) &&
        (click_y >= area_center_y - area_height / 2 && click_y <= area_center_y + area_height / 2))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
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
void dynamicBtn(const char* text, float rect_x, float rect_y, void* init(void), void* update(void), void* exit(void)) {
    if (IsAreaClicked(rect_x, rect_y, rect_width, rect_height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
        scalar = 1.2f;
        drawBtn(rect_x, rect_y, rect_width * scalar, rect_height * scalar, text, scalar);
        CP_Settings_ImageMode(CP_POSITION_CENTER);
        CP_Image_Draw(frame, rect_x, rect_y, rect_width * scalar, rect_height * scalar, 255);
        if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT))
            CP_Engine_SetNextGameStateForced(init, update, exit);
        return 1;
    }
    else {
        scalar = 1.0f;
        drawBtn(rect_x, rect_y, rect_width, rect_height, text, scalar);
        CP_Settings_ImageMode(CP_POSITION_CENTER);
        CP_Image_Draw(frame, rect_x, rect_y, rect_width, rect_height, 255);
        return 0;
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
