#include "cprocessing.h"
#include "mainmenu.h"
#include "function.h"

CP_Image jump;
CP_Image g1;
CP_Image g2;
CP_Image c;
CP_Image Zilong;
CP_Image background;
int status;

void Game_Init(void)
{
	g1 = CP_Image_Load("Assets/Grab1.png");
	g2 = CP_Image_Load("Assets/Grab2.png");
	c = CP_Image_Load("Assets/Carry.png");
	background = CP_Image_Load("Assets/Background.png");
	jump = CP_Image_Load("Assets/Jumping.png");
	Zilong = CP_Image_Load("Assets/Main_Character.png");
	status = 1;
	CP_System_SetFrameRate(120.0f);
	CP_Graphics_ClearBackground(CP_Color_Create(200, 200, 200, 0));
}

void Game_Update(void)
{
	while (status == 1) {
		Character_MovementDefault();
		return 0;
	}
	

}

void Game_Exit(void)
{

}