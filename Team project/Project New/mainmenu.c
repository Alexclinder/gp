#include "cprocessing.h"
#include "function.h"
#include "game.h"


float textSize;
CP_Font Font;

void Main_Menu_Init(void)
{
    Font = CP_Font_Load("Assets/Exo2-Regular.ttf");
    CP_Font_Set(Font);
    textSize = 100.0f;
    CP_Settings_TextSize(textSize);
    CP_Graphics_ClearBackground(CP_Color_Create(100, 100, 100, 0));
}

void Main_Menu_Update(void)
{
    CP_Settings_Fill(CP_Color_Create(255, 100, 100, 100));
    CP_Graphics_DrawRect(CP_System_GetWindowWidth() / 2.0, 700, 300, 200);
    CP_Graphics_DrawRect(CP_System_GetWindowWidth() / 2.0, 200, 300, 200);
    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
    CP_Font_DrawText("Play", CP_System_GetWindowWidth() / 2.0, 200);
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
    CP_Font_DrawText("Exit", CP_System_GetWindowWidth() / 2.0, 700);

    if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT))
    {
        if (IsAreaClicked(CP_System_GetWindowWidth() / 2.0, 200, 300, 200, CP_Input_GetMouseX(), CP_Input_GetMouseY()) == 1) {
            CP_Engine_SetNextGameState(Game_Init, Game_Update, Game_Exit);
        }
        else if (IsAreaClicked(CP_System_GetWindowWidth() / 2.0, 700, 300, 200, CP_Input_GetMouseX(), CP_Input_GetMouseY()) == 1) {
            CP_Engine_Terminate();
        }
    }
    else {}
}

void Main_Menu_Exit(void)
{
    CP_Font_Free(Font);
}