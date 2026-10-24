/* Find sin wee for any concerns or questions about the code.
@brief This file contains the screen sequence for the game
		1) Main Menu
		2) Pause Menu
		3) Win Menu
		4) Lose Menu

		The remaining functions are used to simplify the code and make it more readable.
*/

#include "cprocessing.h"
#include "game.h"
#include "screen_seq.h"
#include "Player's_Elements.h"

// Global variables are required for constant updates.
CP_Font Font;
int level_selected;

float textSize = 50.0f;     //Text Size variable
float Button_x_CenterScreen;//All Button's X-coords variable at the Center of Screen
float Exit_Button_y;        //Exit Button Y-coords
float Button_1_y;           //"Button 1" Y-coords variable
float Button_2_y;           //"Button 2" Y-coords variable
float Button_3_y;           //"Button 3" Y-coords variable

float rect_width = 400.0f; //Width of Bottom
float rect_height = 100.0f;//Height of Bottom
float scalar;              //Scalar Variable

//****************************Main Menu Functions*************************//
void Screen_Init(void)
{
    Font = CP_Font_Load("Assets/Exo2-Regular.ttf");
    CP_Font_Set(Font);
}

void Main_Menu_Update(void) {
    /*
    CP_Graphics Clear Background needs to be at the top.
    Otherwise, it might clear buttons and text */
    //Initialse Background Color
    CP_Graphics_ClearBackground(CP_Color_Create(100, 100, 100, 0));

    //*********************Level SELCTION value
    level_selected = 1;

    // Start Game Y Postion
    Button_1_y = CP_System_GetWindowHeight() / 2.0 - 150.0f;

    dynamicBtn("Start Game!", Button_1_y, Game_Init, Game_Update, Game_Exit);
    Exit_dynamicBtn("Exit to Desktop");
} 
/* If Player Press "Start Game": Game Initialises --> dynamicBtn("Start Game!", Button_1_y, Game_Init, Game_Update, Game_Exit)
   If Player Press "Exit to desktop":  Closes program --> Exit_dynamicBtn("Exit to Desktop", Button_2_y);
*/
void Screen_Exit(void)
{
    CP_Font_Free(Font); // Frees up the Font
}





//****************************Menu Button Functions*************************//

//Checks if mouse pointer in within square/rectangle parameters
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

//Checks if mouse pointer in within circle parameters
int IsCircleClicked(float circle_center_x, float circle_center_y, float diameter, float click_x, float click_y)
{
    // check if the click is within 
    // x bound of the area(check the x-axis radius from the centre of the circle(x coordinate - (diameter/2) && x coordinate + (diameter/2)))
    // y bound of the area(check the y-axis radius from the centre of the circle(y coordinate - (diameter/2) && y coordinate + (diameter/2)))
    if ((click_x >= circle_center_x - diameter / 2 && click_x <= circle_center_x + diameter / 2) &&
        (click_y >= circle_center_y - diameter / 2 && click_y <= circle_center_y + diameter / 2))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void drawBtn(float x,float y, float w, float h, const char* text, float scalar)
{
    drawTextCenter(text, x, y, scalar); //Draws & scales Text
    CP_Settings_Fill(CP_Color_Create(255, 100, 100, 100)); //Button Color
    CP_Graphics_DrawRect(x, y, w, h); // Draws Button base on given X & Y coordinates, Width, Height
}

void drawTextCenter(const char* text, float x, float y, float scalar)
{
    CP_Settings_TextSize(textSize * scalar); // Scales Text Size based on the scalar variable
    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255)); //Text Color
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);//Alligns Text is at Center of given X & Y coordinates
    CP_Font_DrawText(text, x, y);  //Draws Text at Center given X & Y coordinates
}

/*
This function checks if the mouse is hovering over a button area
and scales the button accordingly.
If the button is clicked, it transitions to the game state.      */
void dynamicBtn(const char* text, float button_y, void* init(void), void* update(void), void* exit(void)) {

    //Text X position
    Button_x_CenterScreen = CP_System_GetWindowWidth() / 2.0;

    //If mouse pointer is within the button area e.g."Start Game" 
    if (IsAreaClicked(Button_x_CenterScreen, button_y, rect_width, rect_height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
        scalar = 1.2f;
        //Since Scale is 1.2 Button Enlarges
        drawBtn(Button_x_CenterScreen, button_y, rect_width * scalar, rect_height * scalar, text, scalar);
        if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) 
            CP_Engine_SetNextGameStateForced(init, update, exit);
        // If Left-Click is pressed within Button Game Initialises --> (Game_Init, Game_Update, Game_Exit)
    }
    /*Default state of the Text Button
    Scale is 1.0(Not Enlarged)
    Draws Text Button */
    else {
        scalar = 1.0f;
        drawBtn(Button_x_CenterScreen, button_y, rect_width, rect_height, text, scalar);
    }
}

// Exclusively for exit to desktop button
void Exit_dynamicBtn(const char* text) {

    // "Exit to Desktop" Text Y Position
    Exit_Button_y = CP_System_GetWindowHeight() / 2.0 + 150.0f;

    if (IsAreaClicked(Button_x_CenterScreen, Exit_Button_y, rect_width, rect_height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
        scalar = 1.2f;
        drawBtn(Button_x_CenterScreen, Exit_Button_y, rect_width * scalar, rect_height * scalar, text, scalar);
        if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT))
            CP_Engine_Terminate();
    }
    else {
        scalar = 1.0f;
        drawBtn(Button_x_CenterScreen, Exit_Button_y, rect_width, rect_height, text, scalar);
    }
}






//****************************Special Menus & Win/Lose Screens*************************//
// Pause Menu
void Pause_Menu_Update(void)
{
    CP_Graphics_ClearBackground(CP_Color_Create(100, 200, 100, 0));//Background Color

    // "Resume" Button Y Position
    Button_1_y = CP_System_GetWindowHeight() / 2.0 - 150.0f;
    // "Restart" Button Y Position
    Button_2_y = CP_System_GetWindowHeight() / 2.0; 
    // "Exit to Main Menu" Button Y Position
    Button_3_y = CP_System_GetWindowHeight() / 2.0 + 150.0f;
    
    dynamicBtn("Resume Game!", Button_1_y, Game_Init, Game_Update, Game_Exit);
    dynamicBtn("Restart Level", Button_2_y, Game_Reset, Game_Update, Game_Exit);
    dynamicBtn("Exit to Main Menu", Button_3_y, Screen_Init, Main_Menu_Update, Screen_Exit);

    
}

// Win Screen
void Win_Screen(void) {

    CP_Graphics_ClearBackground(CP_Color_Create(100, 200, 100, 0));
    CP_Settings_RectMode(CP_POSITION_CENTER);
    // Next Level
    Button_1_y = CP_System_GetWindowHeight() / 2.0 - 150.0f;
    // Restart
    Button_2_y = CP_System_GetWindowHeight() / 2.0;
    // Exit to Main Menu
    Button_3_y = CP_System_GetWindowHeight() / 2.0 + 150.0f;
    
    
    dynamicBtn("Next Level", Button_1_y, Next_level_Init, Game_Update, Game_Exit);
    dynamicBtn("Restart Level", Button_2_y, Game_Reset, Game_Update, Game_Exit);
    dynamicBtn("Exit to Main Menu", Button_3_y, Screen_Init, Main_Menu_Update, Screen_Exit);

    
}
void Next_level_Init(void) {
    Game_Next_Level();//Increases level_selected variable in game.c
    Game_Reset();     //Resets the Player's position
}

// Lose Screen
void Lose_Screen(void) {
    CP_Graphics_ClearBackground(CP_Color_Create(100, 100, 100, 0));
    CP_Settings_RectMode(CP_POSITION_CENTER);
	// Restart Level
    Button_1_y = CP_System_GetWindowHeight() / 2.0 - 150.0f;
	// Exit to Main Menu
    Button_2_y = CP_System_GetWindowHeight() / 2.0 + 150.0f;

    dynamicBtn("Restart Level", Button_1_y, Game_Reset, Game_Update, Game_Exit);
    dynamicBtn("Exit to Main Menu", Button_2_y, Screen_Init, Main_Menu_Update, Screen_Exit);

    
}







//****************************Not in Use Yet*************************//

void Leaderboard_Screen(void) {
    CP_Graphics_ClearBackground(CP_Color_Create(100, 140, 200, 0));

    // Restart Level
    Button_1_y = CP_System_GetWindowHeight() / 2.0 - 150.0f;
    // Exit to Main Menu
    Button_2_y = CP_System_GetWindowHeight() / 2.0 + 150.0f;

    dynamicBtn("Level Select", Button_1_y, Screen_Init, Level_Select_Screen, Screen_Exit);
    dynamicBtn("Exit to Main Menu", Button_2_y, Screen_Init, Main_Menu_Update, Screen_Exit);

}

void Completed_Game_Screen(void) {
    CP_Graphics_ClearBackground(CP_Color_Create(100, 200, 100, 0));

    // Next Level
    Button_1_y = CP_System_GetWindowHeight() / 2.0 - 150.0f;
    // Restart
    Button_2_y = CP_System_GetWindowHeight() / 2.0;
    // Exit to Main Menu
    Button_3_y = CP_System_GetWindowHeight() / 2.0 + 150.0f;

    dynamicBtn("Leaderboard", Button_1_y, Screen_Init, Leaderboard_Screen, Screen_Exit);
    dynamicBtn("Level Select", Button_2_y, Screen_Init, Level_Select_Screen, Screen_Exit);
    dynamicBtn("Exit to Main Menu", Button_3_y, Screen_Init, Main_Menu_Update, Screen_Exit);
}

void Level_Select_Screen(void) {
    // Draw 4 boxes 

    // Label them 
    dynamicBtn("Exit to Main Menu", Button_3_y, Screen_Init, Main_Menu_Update, Screen_Exit);
}



