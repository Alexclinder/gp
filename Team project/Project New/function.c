#include "cprocessing.h"
#include "game.h"

static CP_Vector position = { 100, 200 };
static CP_Vector velocity = { 0, 0 };

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


void Character_MovementDefault(void)
{
	float currentElapsedTime = CP_System_GetDt();
	/*updating total elapsed time*/
	static float totalElapsedTime = 0;
	totalElapsedTime += currentElapsedTime;

	if (CP_Input_KeyDown(KEY_W)) {
		velocity.y = -200.0f;
		velocity.x = 0.0f;
	}
	else if (CP_Input_KeyDown(KEY_S)) {
		velocity.y = 200.0f;
		velocity.x = 0.0f;
	}
	else if (CP_Input_KeyDown(KEY_A)) {
		velocity.x = -200.0f;
		velocity.y = 0.0f;
	}
	else if (CP_Input_KeyDown(KEY_D)) {
		velocity.x = 200.0f;
		velocity.y = 0.0f;
	}
	else
	{
		velocity.x = 0.0f;
		velocity.y = 0.0f;
	}
	position = CP_Vector_Add(position, CP_Vector_Scale(velocity, currentElapsedTime));
	CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));
	CP_Image_Draw(background, 0, 0, CP_System_GetWindowWidth(), CP_System_GetWindowHeight(), 255);
	CP_Image_Draw(Zilong, position.v[0], position.v[1], 100, 100, 255);
}

void Character_MovementCarry(void)
{

}