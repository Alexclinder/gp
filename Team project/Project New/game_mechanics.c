#include "cprocessing.h"
#include "PlayerMechanics.h"
#include "game_mechanics.h"
#include "Player's_Elements.h"

float collision_radius = 50.0f;
float degrees;
float cornerRadius;

int win = 0;

void DrawPlatform(float x, float y, float w, float h,
	float degrees, float cornerRadius)
{
	CP_Settings_Fill(CP_Color_Create(100, 180, 255, 255));
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Graphics_DrawRectAdvanced(x, y, w, h, degrees, cornerRadius);

	collision_check(x, y, w, h);
}

int CheckWinCondition(float x, float y, float w, float h, float degrees, float cornerRadius)
{
	return (position.x + PLAYER_SIZE > x && position.x < x + w && position.y + PLAYER_SIZE > y && position.y < y + h);
}
