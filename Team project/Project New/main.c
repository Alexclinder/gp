/*!
@file     main.c
@author   Sin Wee, Lewin Yeo, Kezin , Jun Kai
@module   csd1401f26 Software Engineering Project 1
@section  B
@date     19/09/26
@brief	  Welcome to Office Runner. 
*//*______________________________________________________________________*/
#include "cprocessing.h"
#include "screen_seq.h"

int main(void)
{
	CP_Engine_SetNextGameState(Screen_Init, Main_Menu_Update, Screen_Exit);
	CP_System_SetWindowSize(1600, 900);
	CP_Engine_Run(0);
	return 0;
}