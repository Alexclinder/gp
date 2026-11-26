/*!
@file     colors.h
@author   Sin Wee
@module   csd1401f26 Software Engineering Project 1
@section  B
@date     20/10/26
@brief    Declarations for the color constants
*//*______________________________________________________________________*/
#pragma once

#include "cprocessing.h"

extern CP_Color purple;
extern CP_Color light_green;
extern CP_Color red;
extern CP_Color light_sky_blue;
extern CP_Color purple_blue;
extern CP_Color white;
extern CP_Color translucent_white;
extern CP_Color medium_light_green;
extern CP_Color orange;
extern CP_Color dark_blue;
extern CP_Color grey;
extern CP_Color yellow;
extern CP_Color blue;
extern CP_Color black;

/*!
@brief  Initialises all of the shared color constants declared in this
        header using CP_Color_Create(). Must be called once at start-up,
        before any of the colors are used, otherwise they will hold
        zero-initialised values (fully transparent black).

@param  None.

@return None.
*//*_______________________________________________________________________*/
void color_create(void);