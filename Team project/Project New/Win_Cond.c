#include "Win_Cond.h"
#include "cprocessing.h"

//Checks if the Player is When the Win Condition Trigger Box
int Win_con(float area_corner_x, float area_corner_y, float area_width, float area_height, float Player_PosX, float Player_PosY, float Player_Size) {
	if (
		(Player_PosX <= area_corner_x + area_width) //Checks if Player's Left X-Coodinates is Less the Win condition Box Right X Coor
		&&(Player_PosY <= area_corner_y + area_height) //Checks if Player's Top Y-Coodinates is above the Win condition Bottom Y Coor
		&&(Player_PosX+Player_Size >= area_corner_x) //Checks if Player's Right X-Coodinates is Greater than the Win condition Left X Coor
		&&(Player_PosY+Player_Size >= area_corner_y) //Checks if Player's Bottom Y-Coodinates is within the Win condition
		)
	{
		
		return 1;
	}
	else
		return 0;
}
