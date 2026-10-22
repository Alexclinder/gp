#include "cprocessing.h"
#include "screen_seq.h"
#include "lvl1.h"

int main(void)
{
	CP_Engine_SetNextGameState(lvl1_Init, lvl1_Update, lvl1_Exit);
	CP_System_SetWindowSize(1600, 900);
	CP_Engine_Run(0);
	return 0;
}