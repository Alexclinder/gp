/*!
@file     colors.c
@author	  Sin Wee
@module   csd1401f26 Software Engineering Project 1
@section  B
@date     20/10/26
@brief    Implementation for the color constants
*//*______________________________________________________________________*/
#include "cprocessing.h"
#include "colors.h"

CP_Color purple;
CP_Color light_green;
CP_Color red;
CP_Color light_sky_blue;
CP_Color purple_blue;
CP_Color white;
CP_Color translucent_white;
CP_Color medium_light_green;
CP_Color orange;
CP_Color dark_blue;
CP_Color grey;
CP_Color yellow;
CP_Color blue;
CP_Color black;

/*
color_create

CREATE every colour used in the game (RGB + alpha)
	purple, light green, red, grey, light sky blue, purple blue,
	white, translucent white, medium light green, orange,
	dark blue, yellow, blue, black
*/
void color_create(void) {
	purple = CP_Color_Create(128, 0, 128, 255);
	light_green = CP_Color_Create(144, 238, 144, 255);
	red = CP_Color_Create(255, 0, 0, 255);
	grey = CP_Color_Create(128, 128, 128, 255);
	light_sky_blue = CP_Color_Create(100, 180, 255, 255);
	purple_blue = CP_Color_Create(255, 100, 100, 50);
	white = CP_Color_Create(255, 255, 255, 255);
	translucent_white = CP_Color_Create(255, 255, 255, 200);
	medium_light_green = CP_Color_Create(100, 200, 100, 255);
	orange = CP_Color_Create(240, 105, 80, 255);
	dark_blue = CP_Color_Create(40, 55, 105, 255);
	yellow = CP_Color_Create(255, 255, 0, 255);
	blue = CP_Color_Create(0, 0, 180, 255);
	black = CP_Color_Create(0, 0, 0, 255);
}