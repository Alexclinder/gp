#include "cprocessing.h"
#include "function.h"
#include "game.h"
#include "mainmenu.h"

float textSize=100.0f;
CP_Font Font;

float f_rect_y = 200.0f;
float s_rect_y = 700.0f;
float f_rect_w = 300.0f;
float f_rect_h = 200.0f;
float s_rect_w = 300.0f;
float s_rect_h = 200.0f;
float scalar;


void Main_Menu_Init(void)
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

void Main_Menu_Update(void)
{
    CP_Graphics_ClearBackground(CP_Color_Create(100, 100, 100, 0));

    
    if (IsAreaClicked(CP_System_GetWindowWidth() / 2.0, f_rect_y, f_rect_w, f_rect_h, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
        scalar = 1.2f;
        drawRect(f_rect_y, f_rect_w * scalar, f_rect_h * scalar, "Play", scalar);
        if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) 
            CP_Engine_SetNextGameState(Game_Init, Game_Update, Game_Exit);
    }
    else {
		scalar = 1.0f;
        drawRect(f_rect_y, f_rect_w, f_rect_h, "Play", scalar);
    }
    
    if (IsAreaClicked(CP_System_GetWindowWidth() / 2.0, s_rect_y, s_rect_w, s_rect_h, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
        scalar = 1.2f;
        drawRect(s_rect_y, s_rect_w * scalar, s_rect_h * scalar, "Exit",scalar);
        if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT))
            CP_Engine_Terminate();
    }
    else {
        scalar = 1.0f;
        drawRect(s_rect_y, s_rect_w, s_rect_h, "Exit", scalar);
    }
}


void Main_Menu_Exit(void)
{
    CP_Font_Free(Font);
}