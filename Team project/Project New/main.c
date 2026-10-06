#include "cprocessing.h"
#include "mainmenu.h"

CP_Image jump;
CP_Image g1;
CP_Image g2;
CP_Image c;
CP_Image Zilong;
CP_Image background;
float x;
float y;
float g = 100.0f;
float yv = 100.0f;
float y_ground = 1000.f;
int grabbing = 0;
float grabTimer = 0.0f;
BOOL j = FALSE;

void game_init(void)
{
	CP_System_SetFrameRate(60.0f);
	CP_System_Fullscreen();
	g1 = CP_Image_Load("Assets/Grab1.png");
	g2 = CP_Image_Load("Assets/Grab2.png");
	c = CP_Image_Load("Assets/Carry.png");
	background = CP_Image_Load("Assets/Background.png");
	jump = CP_Image_Load("Assets/Jumping.png");
	Zilong = CP_Image_Load("Assets/Main_Character.png");
	CP_Settings_ImageMode(CP_POSITION_CORNER);
	CP_Settings_ImageWrapMode(CP_IMAGE_WRAP_CLAMP);
	float x = 0.f;
	float y = 1000.f;
	int life = 1;
}

void game_update(void)
{
	if (CP_Input_KeyTriggered(KEY_Q)) // drop the object
	{
		grabbing = 0;
	}
	if (!grabbing) // If not grabbing, allow movement
	{
		CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));
		CP_Image_Draw(background, 0, 0, CP_System_GetWindowWidth(), CP_System_GetWindowHeight(), 255);
		if (CP_Input_KeyDown(KEY_DOWN)) // Move down
		{
			y += 10.0f;
		}
		if (CP_Input_KeyDown(KEY_RIGHT)) // Move right
		{
			x += 10.0f;
		}
		if (CP_Input_KeyDown(KEY_LEFT)) // Move left
		{
			x -= 10.0f;
		}
		if (CP_Input_KeyTriggered(KEY_UP)) // Jump
		{
			if (y >= y_ground) {
				j = TRUE;
			}
		}
		if (j == TRUE)
		{
			if (y <= y_ground - 300.f)
			{
				j = FALSE;
			}
			else
			{
				y -= 10.0f;
			}
		}
		else if (y < y_ground)
		{
			y += 10.0f;
		}
		CP_Image_Draw(Zilong, x, y, 100, 100, 255);
	}
		if (grabbing == 1) // grabbing resets movement animation
		{
			CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));
			CP_Image_Draw(background, 0, 0, CP_System_GetWindowWidth(), CP_System_GetWindowHeight(), 255);
			if (CP_Input_KeyDown(KEY_DOWN)) // Move down
			{
				y += 10.0f;
			}
			if (CP_Input_KeyDown(KEY_RIGHT)) // Move right
			{
				x += 10.0f;
			}
			if (CP_Input_KeyDown(KEY_LEFT)) // Move left
			{
				x -= 10.0f;
			}
			if (CP_Input_KeyTriggered(KEY_UP)) // Jump
			{
				if (y >= y_ground) {
					j = TRUE;
				}
			}
			if (j == TRUE)
			{
				if (y <= y_ground - 300.f)
				{
					j = FALSE;
				}
				else
				{
					y -= 10.0f;
				}
			}
			else if (y < y_ground)
			{
				y += 10.0f;
			}
			else if (y = y_ground){}
			CP_Image_Draw(c, x, y, 100, 100, 255);
		}
		if (CP_Input_KeyTriggered(KEY_E))
		{
			grabbing = 1;
			grabTimer = 0.0f;
			
		}
		if (grabbing == 1)
		{
			grabTimer += CP_System_GetDt();

			if (grabTimer < 0.5f)
			{
				CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));
				CP_Image_Draw(background, 0, 0, CP_System_GetWindowWidth(), CP_System_GetWindowHeight(), 255);
				CP_Image_Draw(g1, x, y, 80, 100, 255);
			}
			else if (grabTimer < 1.0f)
			{
				CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));
				CP_Image_Draw(background, 0, 0, CP_System_GetWindowWidth(), CP_System_GetWindowHeight(), 255);
				CP_Image_Draw(g2, x, y, 80, 100, 255);
			}
			else if (grabTimer < 1.5f)
			{
				CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));
				CP_Image_Draw(background, 0, 0, CP_System_GetWindowWidth(), CP_System_GetWindowHeight(), 255);
				CP_Image_Draw(c, x, y, 100, 100, 255);
			}
		}
		CP_Graphics_DrawRect(20, 900, 100, 100);
}
// main() the starting point for the program
// CP_Engine_SetNextGameState() tells CProcessing which functions to use for init, update and exit
// CP_Engine_Run() is the core function that starts the simulation
int main(void)
{
	CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
	CP_System_SetWindowSize(1600, 900);
	CP_Engine_Run(0);
	return 0;
}