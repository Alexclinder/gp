/*!
@file     screen_seq.c
@author   Sin Wee
@module   csd1401f26 Software Engineering Project 1
@section  B
@date     01/10/26
@brief  This file contains the screen sequence for the game
        1) Main Menu
        2) Pause Menu
        3) Win Menu
        4) Lose Menu
        5) Leaderboard Menu
*//*______________________________________________________________________*/
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
#include <stdio.h>

// Global variables are required for constant updates.
float textSize = 50.0f;

float first_y;
float second_y;
float third_y;
float fourth_y;

float rect_width = 400.0f;
float rect_height = 100.0f;
float scalar;

// Slide-in animation timer for the Level Select screen
static float level_anim_time = 0.0f;

/*******Transition Sequences*******/
void Screen_Init(void)
{
    Font = CP_Font_Load("Assets/Exo2-Regular.ttf");
    //Use to load all the sounds to be used in the game
    LoadSounds();
    // Use to initalize the color variables with values stored
    color_create();
    // Use to initalize the Assets Images used in the game
    LoadImages();
    if (Font == NULL) {
        //if this prints, the file path or name is wrong!
        printf("ERROR: Failed to load font!\n");
    }
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

    // free images from leaderboard screen
    CP_Image_Free(background);
    CP_Image_Free(backgroundtwo);
    CP_Image_Free(backgroundthree);
    CP_Image_Free(backgroundfour_pre);
}

/*
Next_level_Init

GO to next level
RESET game
*/
void Next_level_Init(void) {
    Game_Next_Level();
    Game_Reset();
}

void Exit_Main_Menu_Init(void)
{
    // deducted whatever points earned 
    score_abandoned_level();
    level_score_reset();
    Screen_Init();
}



/*
ResetALLTimers_Status

SET timer to restart
CLEAR saved pause time
SET teleport charges to 3
*/
void ResetALLTimers_Status(void) {
    TimeSet = 0;
    ResetTime = 1;
    Teleport_count = 3;
}
/*
ResetALLTimers_StatusBossLvl

SET timer to restart
CLEAR saved pause time
SET teleport charges to 4 (unlimited)
*/
void ResetALLTimers_StatusBossLvl(void) {
    TimeSet = 0;
    ResetTime = 1;
    Teleport_count = 4;//Set as four but represents unlimited teleports
}
/***********************Level Initialization Functions******************/
// score_reset reset level score but not the player_score (cumulative of all level_score)

void Level_1_Init(void) { level_selected = level_1; ResetALLTimers_Status();  Game_Init(); Player_Starting_Pos(player_spawns, level_selected); score_remove_level(level_selected); NPC_Starting_Pos(NPC_1_spawns, level_selected); }
void Level_2_Init(void) { level_selected = level_2; ResetALLTimers_Status();  Game_Init(); Player_Starting_Pos(player_spawns, level_selected); score_remove_level(level_selected); NPC_Starting_Pos(NPC_1_spawns, level_selected); }
void Level_3_Init(void) { level_selected = level_3; ResetALLTimers_Status();  Game_Init(); Player_Starting_Pos(player_spawns, level_selected); score_remove_level(level_selected); NPC_Starting_Pos(NPC_1_spawns, level_selected); }
void Level_4_Init(void) { level_selected = level_4; ResetALLTimers_StatusBossLvl();  Game_Init(); Player_Starting_Pos(player_spawns, level_selected); score_remove_level(level_selected); NPC_Starting_Pos(NPC_1_spawns, level_selected); }


/*****************************Main Menu Screens*******************************/
void Main_Menu_Update(void) {

    static float anim_time = 0.0f;   // keeps its value between frames
    float rect_x = 320.0f;

    CP_Graphics_ClearBackground(CP_Color_Create(100, 100, 100, 0));
    CP_Image_Draw(mainbackground, (float)CP_System_GetWindowWidth() / 2.0f, (float)CP_System_GetWindowHeight() / 2.0f, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight(), 255);
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
    p = (anim_time - 1.0f) / duration;
    if (p < 0.0f) p = 0.0f;
    if (p > 1.0f) p = 1.0f;
    float exit_offset = -distance * (1.0f - p) * (1.0f - p) * (1.0f - p);
    // -------------------------------------

    // ---------- Title ----------
    CP_Settings_Save();   // so these text settings don't affect your buttons
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
    dynamicBtn("Start Game!", rect_x + start_offset, first_y, white, Start_Game_Init, Level_Select_Screen, Screen_Exit);
    CP_Settings_Fill(light_sky_blue);
    dynamicBtn("Controls", rect_x + middle_offset, second_y, white, Screen_Init, Controls_Screen, Screen_Exit);
    CP_Settings_Fill(dark_blue);
    Exit_dynamicBtn("Exit to Desktop", rect_x + exit_offset, third_y, white);

    CP_Settings_Fill(dark_blue);
    dynamicBtn("Leaderboard", 1400.0, 745.0, white, Screen_Init, Leaderboard_Screen, Screen_Exit); // Leaderboard button
}

/*
Start_Game_Init

RESET score
SET UP screen
*/
void Start_Game_Init(void)
{
    score_reset();
    Screen_Init();
}

/*
Game_Resume_Init

LOAD font
CREATE colours
LOAD level images
SET frame rate to 120
SET timer to restart (start time is set again next frame)
*/
void Game_Resume_Init(void)
{
    Font = CP_Font_Load("Assets/Exo2-Regular.ttf");
    color_create();
    LoadLevelImages();
    CP_System_SetFrameRate(120.0f);
    TimeSet = 0;
}



void Pause_Menu_Update(void)
{
    float rect_x = 250.0f;
    CP_Graphics_ClearBackground(CP_Color_Create(100, 200, 100, 0));
    CP_Image_Draw(pausemenu, (float)CP_System_GetWindowWidth() / 2.0f, (float)CP_System_GetWindowHeight() / 2.0f, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight(), 255);
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
    CP_Settings_TextSize(100.0f);
    CP_Font_DrawText("PAUSED", 250.0f, 80.0f);

    // Buttons are spaced evenly down the screen,
    // each one 13% of two_third_section below the one before
    float two_third_section = CP_System_GetWindowHeight() + 700.0f;
    float y_offset = 0.13f;
    // Resume
    first_y = two_third_section * y_offset;
    // Restart
    second_y = two_third_section * (y_offset * 2);
    // Controls
    third_y = two_third_section * (y_offset * 3);
    // Exit to Main Menu
    fourth_y = two_third_section * (y_offset * 4);


    CP_Settings_Fill(light_sky_blue);
    dynamicBtn("Resume Game!", rect_x, first_y, white, Game_Resume_Init, Game_Update, Game_Exit);
    CP_Settings_Fill(light_sky_blue);
    dynamicBtn("Restart Level", rect_x, second_y, white, Game_Restart_Init, Game_Update, Game_Exit);
    CP_Settings_Fill(light_sky_blue);
    dynamicBtn("Controls", rect_x, third_y, white, Screen_Init, Controls_Screen, Screen_Exit);
    CP_Settings_Fill(light_sky_blue);
    dynamicBtn("Exit to Main Menu", rect_x, fourth_y, white, Exit_Main_Menu_Init, Main_Menu_Update, Screen_Exit);

}

void Win_Screen(void) {
    float rect_x = (float)CP_System_GetWindowWidth() / 2.0f;
    CP_Graphics_ClearBackground(CP_Color_Create(100, 200, 100, 0));

    char score_buf[SCORE_BUF_SIZE] = { 0 };
    sprintf_s(score_buf, SCORE_BUF_SIZE, "Score: %d", level_score);
    drawTextCenter(score_buf, rect_x, 300.0f, 1.0f, white);

    CP_Font_Set(Font);
    CP_Settings_TextSize(50);
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_TOP);
    CP_Settings_Fill(black);
    char buffer[TIME_BUF_SIZE] = { 0 };
    sprintf_s(buffer, TIME_BUF_SIZE, "Time: %.2f", WinScreenTime);

    CP_Font_DrawText("Well Done", rect_x, 100.0f);
    CP_Font_DrawText(buffer, rect_x, 150.0f);

    // Need larger size than 16 chars, otherwise game will crash
    char total_score_buf[SCORE_BUF_SIZE * 2] = { 0 };
    sprintf_s(total_score_buf, SCORE_BUF_SIZE * 2, "Total Score: %d", player_score);

    CP_Font_DrawText(total_score_buf, rect_x, 200.0f);

    // Next Level
    first_y = (float)CP_System_GetWindowHeight() / 2.0f;
    // Restart
    second_y = (float)CP_System_GetWindowHeight() / 2.0f + 150.0f;
    // Exit to Main Menu
    third_y = (float)CP_System_GetWindowHeight() / 2.0f + 300.0f;

    CP_Settings_Fill(orange);
    dynamicBtn("Next Level", rect_x, first_y, white, Next_level_Init, Game_Update, Game_Exit);
    CP_Settings_Fill(orange);
    dynamicBtn("Restart Level", rect_x, second_y, white, Game_Restart_Init, Game_Update, Game_Exit);
    CP_Settings_Fill(orange);
    dynamicBtn("Exit to Main Menu", rect_x, third_y, white, Exit_Main_Menu_Init, Main_Menu_Update, Screen_Exit);

}

void Lose_Screen(void) {
    float rect_y = CP_System_GetWindowHeight() / 2.0f;
    CP_Graphics_ClearBackground(CP_Color_Create(100, 100, 100, 0));
    CP_Image_Draw(defeat, (float)CP_System_GetWindowWidth() / 2.0f, (float)CP_System_GetWindowHeight() / 2.0f, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight(), 255);

    // Restart Level
    first_y = (float)CP_System_GetWindowHeight() / 2.0f - 150.0f;
    // Exit to Main Menu
    second_y = (float)CP_System_GetWindowHeight() / 2.0f + 150.0f;
    CP_Settings_Fill(purple_blue);
    dynamicBtn("Restart Level", 300, rect_y, white, Game_Reset, Game_Update, Game_Exit);
    CP_Settings_Fill(purple_blue);
    dynamicBtn("Exit to Main Menu", 1300, rect_y, white, Exit_Main_Menu_Init, Main_Menu_Update, Screen_Exit);


}

/*
Game_Restart_Init

SET UP game
RESET game
REMOVE score earned in the current level
*/
void Game_Restart_Init(void)
{
    Game_Init();
    Game_Reset();
    score_remove_level(level_selected);
}

/*
Leaderboard_Screen

CLEAR background to dark blue
SET text, box, padding and background image sizes
SET UP text buffers for each level's time and score, and the totals

FOR each row (2 rows)
    FOR each column (2 columns)
        WORK OUT box position
        DRAW purple box

        CHECK which box this is
            TOP LEFT     -> LEVEL 1
            TOP RIGHT    -> LEVEL 2
            BOTTOM LEFT  -> LEVEL 3
            BOTTOM RIGHT -> LEVEL 4

        DRAW that level's background image inside the box
        DRAW level name above the box
        DRAW level's time near the bottom of the box
        DRAW level's score below the time

ADD all 4 level times to get total time
DRAW total time on the left side in orange
DRAW total score below total time

DRAW "Level Select" button      -> GO to level select screen
DRAW "Exit to Main Menu" button -> GO to main menu
*/
void Leaderboard_Screen(void) {


    float rect_x = (float)CP_System_GetWindowWidth() / 2.0f;
    CP_Graphics_ClearBackground(CP_Color_Create(0, 50, 100, 100));

    float left_offset = 100.0f;
    float right_offset = 500.0f;

    float top_timer_y = 300.0f;
    float bottom_timer_y = 700.0f;

    float score_offset_y = 50.0f;

    float timer_score_offset_x = 300.0f;
    float timer_score_top_y = 200.0f;
    float timer_score_bot_y = 600.0f;

    float timer_score_w = 400.0f;

    CP_Color text_color = white;
    float text_scalar = 0.5f;

    // Drawing 2x2 matrix rectangles
    // first rect starts at left row, left col
    // 0 0 
    // 0 1
    // 1 0 
    // 1 1
    float level_box_width = 450.0f;
    float level_box_height = 360.0f;
    float padding = 50.0f;
    float box_start_posX = rect_x - 220.0f;
    float box_start_posY = 120.0f;

    float bg_width = level_box_width - 30.0f;
    // height follows the bg_width - current screen size is 16 by 9. so 9*bg_width/16 = bg_height 
    float bg_height = ((float)CP_System_GetWindowHeight() * bg_width) / (float)CP_System_GetWindowWidth();
    float alpha = 255.0f;

    float level_text_scalar = 0.8f;
    float level_text_yOffset = 23.0f;

    // Buffers
    char Level1_Time[TIME_BUF_SIZE] = { 0 };
    char Level1_Score[SCORE_BUF_SIZE] = { 0 };

    char Level2_Time[TIME_BUF_SIZE] = { 0 };
    char Level2_Score[SCORE_BUF_SIZE] = { 0 };

    char Level3_Time[TIME_BUF_SIZE] = { 0 };
    char Level3_Score[SCORE_BUF_SIZE] = { 0 };

    char Level4_Time[TIME_BUF_SIZE] = { 0 };
    char Level4_Score[SCORE_BUF_SIZE] = { 0 };

    // need more space to store cumulative, otherwise game will crash
    char LevelTotal_Time[TIME_BUF_SIZE * 2] = { 0 };
    char LevelTotal_Score[SCORE_BUF_SIZE * 2] = { 0 };

    CP_Settings_RectMode(CP_POSITION_CORNER); // To match DrawBG Corner Mode

    for (int row = 0; row < 2; row++) {
        for (int col = 0; col < 2; col++) {

            float x = box_start_posX + col * (padding + level_box_width);
            float y = box_start_posY + row * (padding + level_box_height);
            float card_center_x = x + (level_box_width * 0.5f);

            CP_Settings_Fill(purple_blue);
            CP_Graphics_DrawRect(x, y, level_box_width, level_box_height);

            // matrix = 0,0 - Level 1
            if (!row && !col) {
                DrawBG(backgroundthree, x + 15.0f, y + 15.0f, bg_width, bg_height, alpha);

                drawTextCenter("Level 1", card_center_x, y - level_text_yOffset, level_text_scalar, text_color);

                sprintf_s(Level1_Time, TIME_BUF_SIZE, "Time: %.2f", Level_Timer.level1_time);
                drawTextCenter(Level1_Time, card_center_x, y + level_box_height - 65.0f, text_scalar, text_color);

                sprintf_s(Level1_Score, SCORE_BUF_SIZE, "Score: %d", Level_Score.level1_score);
                drawTextCenter(Level1_Score, card_center_x, y + level_box_height - 30.0f, text_scalar, text_color);
                continue;
            }

            // matrix = 0,1 - Level 2
            if (!row && col) {
                DrawBG(backgroundtwo, x + 15.0f, y + 15.0f, bg_width, bg_height, alpha);

                drawTextCenter("Level 2", card_center_x, y - level_text_yOffset, level_text_scalar, text_color);

                sprintf_s(Level2_Time, TIME_BUF_SIZE, "Time: %.2f", Level_Timer.level2_time);
                drawTextCenter(Level2_Time, card_center_x, y + level_box_height - 65.0f, text_scalar, text_color);

                sprintf_s(Level2_Score, SCORE_BUF_SIZE, "Score: %d", Level_Score.level2_score);
                drawTextCenter(Level2_Score, card_center_x, y + level_box_height - 30.0f, text_scalar, text_color);
                continue;
            }

            // matrix = 1,0 - Level 3
            if (row && !col) {
                DrawBG(background, x + 15.0f, y + 15.0f, bg_width, bg_height, alpha);

                drawTextCenter("Level 3", card_center_x, y - level_text_yOffset, level_text_scalar, text_color);

                sprintf_s(Level3_Time, TIME_BUF_SIZE, "Time: %.2f", Level_Timer.level3_time);
                drawTextCenter(Level3_Time, card_center_x, y + level_box_height - 65.0f, text_scalar, text_color);

                sprintf_s(Level3_Score, SCORE_BUF_SIZE, "Score: %d", Level_Score.level3_score);
                drawTextCenter(Level3_Score, card_center_x, y + level_box_height - 30.0f, text_scalar, text_color);
                continue;
            }

            // matrix = 1,1 - Level 4
            if (row && col) {
                DrawBG(backgroundfour_pre, x + 15.0f, y + 15.0f, bg_width, bg_height, alpha);

                drawTextCenter("Level 4", card_center_x, y - level_text_yOffset, level_text_scalar, text_color);

                sprintf_s(Level4_Time, TIME_BUF_SIZE, "Time: %.2f", Level_Timer.level4_time);
                drawTextCenter(Level4_Time, card_center_x, y + level_box_height - 65.0f, text_scalar, text_color);

                sprintf_s(Level4_Score, SCORE_BUF_SIZE, "Score: %d", Level_Score.level4_score);
                drawTextCenter(Level4_Score, card_center_x, y + level_box_height - 30.0f, text_scalar, text_color);
                continue;
            }
        }
    }

    CP_Color total_text_color = orange;

    float total_text_xOffset = 500.0f;

    float total_text_scalar = 1.3f;

    float total_time_text_y = CP_System_GetWindowHeight() / 2.0f - 200.0f;

    float total_time = Level_Timer.level1_time + Level_Timer.level2_time + Level_Timer.level3_time + Level_Timer.level4_time;

    sprintf_s(LevelTotal_Time, TIME_BUF_SIZE * 2, "Total Time: %.2f", total_time);
    drawTextCenter(LevelTotal_Time, rect_x - total_text_xOffset, total_time_text_y, total_text_scalar, total_text_color);

    sprintf_s(LevelTotal_Score, SCORE_BUF_SIZE * 2, "Total Score: %d", player_score);
    drawTextCenter(LevelTotal_Score, rect_x - total_text_xOffset, total_time_text_y + 100.0f, total_text_scalar, total_text_color);

    // Navigation Buttons
    first_y = CP_System_GetWindowHeight() - 250.0f;
    second_y = CP_System_GetWindowHeight() - 100.0f;
    CP_Settings_Fill(orange);
    dynamicBtn("Level Select", rect_x - right_offset, first_y, white, Screen_Init, Level_Select_Screen, Screen_Exit);
    CP_Settings_Fill(orange);
    dynamicBtn("Exit to Main Menu", rect_x - right_offset, second_y, white, Screen_Init, Main_Menu_Update, Screen_Exit);
}

/*
Level_Select_Screen

CLEAR background
DRAW level select background image

ADD frame time to animation timer
SET slide duration, start distance and delay between items

FOR each item (title, 4 level buttons, back button)
    WORK OUT how far along its slide is (0 to 1)
        (each item starts a little later than the one before)
    SET its offset so it slides in from the left and slows down at the end

DRAW "SELECT A LEVEL" title

DRAW "Level 1" button       -> START level 1
DRAW "Level 2" button       -> START level 2
DRAW "Level 3(HARD)" button -> START level 3
DRAW "Level 4" button       -> START level 4

DRAW "Back to Menu" button  -> GO to main menu
*/
void Level_Select_Screen(void) {
    CP_Graphics_ClearBackground(CP_Color_Create(80, 80, 80, 0));
    CP_Image_Draw(levelselection, (float)CP_System_GetWindowWidth() / 2.0f, (float)CP_System_GetWindowHeight() / 2.0f, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight(), 255);
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

    drawTextCenter("SELECT A LEVEL", btn_x + offset[0], 100.0f, 1.2f, white);

    // Level buttons (stacked on the left)
    CP_Settings_Fill(orange);
    dynamicBtn("Level 1", btn_x + offset[1], 230.0f, white, Level_1_Init, Game_Update, Game_Exit);
    CP_Settings_Fill(orange);
    dynamicBtn("Level 2", btn_x + offset[2], 350.0f, white, Level_2_Init, Game_Update, Game_Exit);
    CP_Settings_Fill(orange);
    dynamicBtn("Level 3(HARD)", btn_x + offset[3], 470.0f, white, Level_3_Init, Game_Update, Game_Exit);
    CP_Settings_Fill(orange);
    dynamicBtn("Level 4", btn_x + offset[4], 590.0f, white, Level_4_Init, Game_Update, Game_Exit);

    // Back to Menu (extra gap below the levels)
    third_y = 740.0f;
    CP_Settings_Fill(orange);
    dynamicBtn("Back to Menu", btn_x + offset[5], third_y, white, Exit_Main_Menu_Init, Main_Menu_Update, Screen_Exit);
}


enum { NUMBER_OF_CONTROLS = 7 };
/*
Controls_Screen

CLEAR background to light sky blue

SET list of control names (jump, left, right, kick, pick up / drop, parry / block, pause / back)
SET list of matching keys (W / Space, A, D, F, E, P, Esc)
SET box positions, sizes and spacing

FOR each control
    DRAW box with control name on the left
    DRAW box with its key on the right

DRAW "Back to Menu" button at bottom middle
    IF opened from main menu
        GO back to main menu
    ELSE
        GO back to pause menu
*/
void Controls_Screen(void) {
    CP_Graphics_ClearBackground(light_sky_blue);

    const char* controls_text[NUMBER_OF_CONTROLS] =
    {
        "JUMP",
        "LEFT",
        "RIGHT",
        "KICK",
        "PICK UP / DROP",
        "PARRY / BLOCK",
        "PAUSE/BACK"
    };
    // x, y, w, h, text, scalar

    const char* control_keys[NUMBER_OF_CONTROLS] =
    {
        "W / SPCBAR",
        "A",
        "D",
        "F",
        "E",
        "P",
        "ESC"
    };

    float first_control_y = 100.0f;
    float first_control_x = 550.0f;
    float y_offset = 100.0f; // vertical spacing between controls

    float first_controlkey_x = first_control_x * 2.0f;

    const float controls_rect_width = 400.0f;
    const float controlskeys_rect_width = 350.0f;
    const float control_rect_height = 70.0f;

    // Draw the boxes and text for each control and corresponding key
    for (int i = 0; i < NUMBER_OF_CONTROLS; i++) {
        CP_Settings_Fill(purple_blue);
        drawBtn(first_control_x, first_control_y + (i * y_offset), controls_rect_width, control_rect_height, controls_text[i], 1.0f, white);
        CP_Settings_Fill(purple_blue);
        drawBtn(first_controlkey_x, first_control_y + (i * y_offset), controlskeys_rect_width, control_rect_height, control_keys[i], 1.0f, white);
    }

    // Back to Menu button
    float control_backmenu_btn_y = CP_System_GetWindowHeight() - 70.0f;
    float control_backmenu_btn_x = CP_System_GetWindowWidth() / 2.0f;
    CP_Settings_Fill(orange);

    if (level_selected == main_menu) {
        dynamicBtn("Back to Menu", control_backmenu_btn_x, control_backmenu_btn_y, white, Exit_Main_Menu_Init, Main_Menu_Update, Screen_Exit);
    }
    else {
        dynamicBtn("Back to Menu", control_backmenu_btn_x, control_backmenu_btn_y, white, Exit_Main_Menu_Init, Pause_Menu_Update, Screen_Exit);
    }
}



// Score: = 6 chars, 1 white space, 
// assume score can reach up to 99,999.00 (excluding comma - 8 char) 
// 1 null terminator - 6 + 1 + 8 + 1 = 16

/*
The functions below are the elements or functionalities of each updated screen
*/

/*********************Objective & Score Text Position Functions*****************/
/*
drawTextTopLeft

SET font
SET text size = default size * scalar
SET text colour
SET text to start from its top left corner
DRAW text at x, y
*/
void drawTextTopLeft(const char* text, float x, float y, float scalar, CP_Color color)
{
    CP_Font_Set(Font);
    CP_Settings_TextSize(textSize * scalar);
    CP_Settings_Fill(color);
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_TOP);
    CP_Font_DrawText(text, x, y);
}

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

/*
drawBtn

SET rectangle to draw from its center
DRAW button box (uses whatever fill colour was set before calling)
DRAW text in the middle of the box
*/
void drawBtn(float x, float y, float w, float h, const char* text, float scalar, CP_Color color)
{
    CP_Settings_RectMode(CP_POSITION_CENTER);
    //CP_Settings_Fill(purple_blue);
    CP_Graphics_DrawRect(x, y, w, h);
    drawTextCenter(text, x, y, scalar, color);
}

/*
dynamicBtn

Returns: 1 = mouse is over the button, 0 = not over the button

IF mouse is over the button
    DRAW button and frame 1.2x bigger
    IF left mouse is clicked
        GO to the given game state (init, update, exit)
    RETURN 1
ELSE
    DRAW button and frame at normal size
    RETURN 0
*/
int dynamicBtn(const char* text, float rect_x, float rect_y, CP_Color color, void (*init)(void), void (*update)(void), void (*exit)(void)) {
    if (IsAreaClicked(rect_x, rect_y, rect_width, rect_height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
        scalar = 1.2f;
        drawBtn(rect_x, rect_y, rect_width * scalar, rect_height * scalar, text, scalar, color);
        CP_Settings_ImageMode(CP_POSITION_CENTER);
        CP_Image_Draw(frame, rect_x, rect_y, rect_width * scalar, rect_height * scalar, 255);
        if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT))
            CP_Engine_SetNextGameStateForced(init, update, exit);
        return 1;
    }
    else {
        scalar = 1.0f;
        drawBtn(rect_x, rect_y, rect_width, rect_height, text, scalar, color);
        CP_Settings_ImageMode(CP_POSITION_CENTER);
        CP_Image_Draw(frame, rect_x, rect_y, rect_width, rect_height, 255);
        return 0;
    }
}

/*
drawTextCenter

SET font
SET text size = default size * scalar
SET text colour
SET text to be centered horizontally and vertically
DRAW text at x, y
*/
void drawTextCenter(const char* text, float x, float y, float scalar, CP_Color color)
{
    CP_Font_Set(Font);
    CP_Settings_TextSize(textSize * scalar);
    CP_Settings_Fill(color);
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
    CP_Font_DrawText(text, x, y);
}

// Exclusively for exit to desktop button
/*
Exit_dynamicBtn

IF mouse is over the button
    DRAW button and frame 1.2x bigger
    IF left mouse is clicked
        CLOSE the game
ELSE
    DRAW button and frame at normal size
*/
void Exit_dynamicBtn(const char* text, float rect_x, float rect_y, CP_Color color) {
    if (IsAreaClicked(rect_x, rect_y, rect_width, rect_height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
        scalar = 1.2f;
        drawBtn(rect_x, rect_y, rect_width * scalar, rect_height * scalar, text, scalar, color);
        CP_Settings_ImageMode(CP_POSITION_CENTER);
        CP_Image_Draw(frame, rect_x, rect_y, rect_width * scalar, rect_height * scalar, 255);
        if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT))
            CP_Engine_Terminate();

    }
    else {
        scalar = 1.0f;
        drawBtn(rect_x, rect_y, rect_width, rect_height, text, scalar, color);
        CP_Settings_ImageMode(CP_POSITION_CENTER);
        CP_Image_Draw(frame, rect_x, rect_y, rect_width, rect_height, 255);
    }
}