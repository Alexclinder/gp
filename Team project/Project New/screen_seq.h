/*!
@file     screen_seq.h
@author   Sin Wee
@module   csd1401f26 Software Engineering Project 1
@section  B
@date     01/10/26
@brief	  Declarations for the screen sequence: the transition (init and
          exit) functions, the menu, pause, win, lose, leaderboard, level
          select and controls screens, and the button and text drawing
          helpers they use.
*//*______________________________________________________________________*/
#pragma once

#include "cprocessing.h"

/* ===== Transition functions =====
   These are passed as the init and exit functions of a game state, for
   example to CP_Engine_SetNextGameState() or dynamicBtn(). */

   /*!
   @brief  Common start-up for the menu screens. Loads the font, loads the
           sounds, creates the shared colors, loads the menu images, and sets
           PauseTimeStatus to 1 so the in-game timer remembers the elapsed
           time. Prints an error if the font fails to load.

   @param  None.

   @return None.
   *//*_______________________________________________________________________*/
void Screen_Init(void);

/*!
@brief  Common clean-up for the menu screens. Frees the menu images, the
        font, and the level backgrounds used by the leaderboard.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void Screen_Exit(void);

/*!
@brief  Init function for the win screen's Next Level button. Calls
        Game_Next_Level() (which increments level_selected and resets the
        level) and then Game_Reset().

@param  None.

@return None.
*//*_______________________________________________________________________*/
void Next_level_Init(void);

/*!
@brief  Init function for every button that goes back to the main menu.
        Discards the current level's score with score_abandoned_level()
        and level_score_reset(), then calls Screen_Init().

@param  None.

@return None.
*//*_______________________________________________________________________*/
void Exit_Main_Menu_Init(void);

/*!
@brief  Init function for the pause menu's Resume button. Reloads the font,
        colors and level images, sets the frame rate to 120 FPS, and sets
        TimeSet to 0 so the timer restarts from the saved paused time.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void Game_Resume_Init(void);

/*!
@brief  Init function for the main menu's Start Game button. Resets all
        scoring with score_reset() and then calls Screen_Init().

@param  None.

@return None.
*//*_______________________________________________________________________*/
void Start_Game_Init(void);

/*!
@brief  Init function for the Restart Level buttons on the pause and win
        screens. Calls Game_Init() and Game_Reset() for the current level,
        then removes that level's earlier score with score_remove_level().

@param  None.

@return None.
*//*_______________________________________________________________________*/
void Game_Restart_Init(void);


/* ===== Screens =====
   Each of these is the update function of a game state and is called
   every frame while that screen is showing. */

/*!
@brief  Draws the main menu: background, an animated title ("THE OFFICE
        RUN") and the buttons Start Game, Controls, Exit to Desktop and
        Leaderboard. The title and buttons slide in from the left, one
        after another. Sets level_selected to main_menu.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void Main_Menu_Update(void);

/*!
@brief  Draws the pause menu with the buttons Resume Game, Restart Level,
        Controls and Exit to Main Menu.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void Pause_Menu_Update(void);

/*!
@brief  Draws the win screen: "Well Done", the level's time and score, the
        total score, and the buttons Next Level, Restart Level and Exit to
        Main Menu.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void Win_Screen(void);

/*!
@brief  Draws the defeat screen with the buttons Restart Level and Exit to
        Main Menu.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void Lose_Screen(void);

/*!
@brief  Draws the leaderboard: a 2 x 2 grid with a background preview, time
        and score for each of levels 1 to 4, the total time and total
        score, and the buttons Level Select and Exit to Main Menu.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void Leaderboard_Screen(void);

/*!
@brief  Draws the level select screen: a "SELECT A LEVEL" title and buttons
        for levels 1 to 4 and Back to Menu. Everything slides in from the
        left, one after another.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void Level_Select_Screen(void);

/*!
@brief  Draws the controls screen: a list of seven actions (jump, left,
        right, kick, pick up / drop, parry / block, pause) next to their
        keys, and a Back to Menu button. The button returns to the main
        menu if the player came from there, otherwise to the pause menu.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void Controls_Screen(void);

/* ===== Button and text helpers ===== */

/*!
@brief  Tests whether a point is inside a rectangle.

@param  area_center_x  X coordinate of the rectangle's centre.
@param  area_center_y  Y coordinate of the rectangle's centre.
@param  area_width     Width of the rectangle.
@param  area_height    Height of the rectangle.
@param  click_x        X coordinate of the point, such as the mouse.
@param  click_y        Y coordinate of the point, such as the mouse.

@return 1 if the point is inside the rectangle (edges included),
        otherwise 0.
*//*_______________________________________________________________________*/
int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y);

/*!
@brief  Draws a rectangle with centred text on it. The rectangle uses the
        current fill color, so set it before calling.

@param  x       X coordinate of the button's centre.
@param  y       Y coordinate of the button's centre.
@param  w       Width of the button.
@param  h       Height of the button.
@param  text    Text to draw on the button.
@param  scalar  Text size multiplier.
@param  color   Color of the text.

@return None.
*//*_______________________________________________________________________*/
void drawBtn(float x, float y, float w, float h, const char* text, float scalar, CP_Color color);

/*!
@brief  Draws text centred on a point. Uses the shared Font at a size of
        50 times scalar.

@param  text    Text to draw.
@param  x       X coordinate of the text's centre.
@param  y       Y coordinate of the text's centre.
@param  scalar  Text size multiplier.
@param  color   Color of the text.

@return None.
*//*_______________________________________________________________________*/
void drawTextCenter(const char* text, float x, float y, float scalar, CP_Color color);

/*!
@brief  Draws a 400 x 100 button with a frame image and text. While the
        mouse is over it, the button is drawn 1.2 times bigger, and
        clicking the left mouse button switches to the given game state.
        The button's fill color comes from the current fill, so set it
        before calling.

@param  text      Text to draw on the button.
@param  rect_x    X coordinate of the button's centre.
@param  rect_y    Y coordinate of the button's centre.
@param  color     Color of the text.
@param  init      Init function of the game state to switch to.
@param  update    Update function of the game state to switch to.
@param  exit      Exit function of the game state to switch to.

@return 1 if the mouse is over the button, otherwise 0.
*//*_______________________________________________________________________*/
int dynamicBtn(const char* text, float rect_x, float rect_y, CP_Color color, void (*init)(void), void (*update)(void), void (*exit)(void));

/*!
@brief  Same as dynamicBtn(), but clicking it closes the game with
        CP_Engine_Terminate() instead of switching state. Only used for the
        Exit to Desktop button.

@param  text    Text to draw on the button.
@param  rect_x  X coordinate of the button's centre.
@param  rect_y  Y coordinate of the button's centre.
@param  color   Color of the text.

@return None.
*//*_______________________________________________________________________*/
void Exit_dynamicBtn(const char* text, float rect_x, float rect_y, CP_Color color);

/*!
@brief  Draws text aligned to the top-left at a point. Used for the
        objective and score text. Uses the shared Font at a size of 50
        times scalar.

@param  text    Text to draw.
@param  x       X coordinate of the text's top-left corner.
@param  y       Y coordinate of the text's top-left corner.
@param  scalar  Text size multiplier.
@param  color   Color of the text.

@return None.
*//*_______________________________________________________________________*/
void drawTextTopLeft(const char* text, float x, float y, float scalar, CP_Color color);

/*!
@brief  Resets the in-game timer and sets the teleport charges to 4,
        which means unlimited. Meant for the boss level. See
        ResetALLTimers_Status() for the normal levels.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void ResetALLTimers_StatusBossLvl(void);

/*!
@brief  Resets the in-game timer and sets the teleport charges to 3.
        Used when starting levels 1 to 3. See ResetALLTimers_StatusBossLvl()
        for the boss level.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void ResetALLTimers_Status(void);

extern CP_Font Font;  /*!< Font used for text. Defined in Images.c, also declared in Images.h. */
extern int MainMenu_Music;
extern int PauseMenu_Music;
extern int Lose_Sound;
extern int Win_Sound;

