#include "cprocessing.h"
#include "colors.h"

CP_Color purple;
CP_Color light_green;
CP_Color red;
CP_Color light_sky_blue;
CP_Color translucent_light_red;
CP_Color white;
CP_Color translucent_white;
CP_Color medium_light_green;
CP_Color orange;
CP_Color dark_blue;
CP_Color yellow;

void color_create(void) {
	purple = CP_Color_Create(128, 0, 128, 255);
	light_green = CP_Color_Create(144, 238, 144, 255);
	red = CP_Color_Create(255, 0, 0, 255);
	light_sky_blue = CP_Color_Create(100, 180, 255, 255);
	translucent_light_red = CP_Color_Create(255, 100, 100, 50);
	white = CP_Color_Create(255, 255, 255, 255);
	translucent_white = CP_Color_Create(255, 255, 255, 200);
	medium_light_green = CP_Color_Create(100, 200, 100, 255);
	orange = CP_Color_Create(240, 105, 80, 255);
	dark_blue = CP_Color_Create(40, 55, 105, 255);
	yellow = CP_Color_Create(255, 255, 0, 255);
}