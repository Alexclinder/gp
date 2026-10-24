#include "cprocessing.h"
#include "game.h"
#include "game_mechanics.h"
#include "lvl1.h"

#if 0
#define PLATFORM_COUNT 5

static Platform platforms[PLATFORM_COUNT] = {
	{    0.0f, 229.0f, 335.0f, 45.0f },   /* entrance */
	{  465.0f, 504.0f, 223.0f, 46.0f },
	{  821.0f, 443.0f, 223.0f, 46.0f },
	{ 1116.0f, 320.0f, 223.0f, 46.0f },
	{ 1379.0f, 229.0f, 221.0f, 45.0f }    /* exit */
};

static NPC enemy;

void lvl1_Init(void) {
	g1 = CP_Image_Load("Assets/Grab1.png");
	g2 = CP_Image_Load("Assets/Grab2.png");
	carry = CP_Image_Load("Assets/Carry.png");
	background = CP_Image_Load("Assets/lvl1Back.png");
	jump = CP_Image_Load("Assets/Jumping.png");
	Zilong = CP_Image_Load("Assets/Main_Character.png");

	//ENEMY NPC
	npc1 = CP_Image_Load("Assets/NPC1.png");
	enemy.image = npc1;
	enemy.w = 100.0f;
	enemy.h = 100.0f;
	enemy.x = 880.0f;
	enemy.y = 443.0f - enemy.h;
	enemy.speed = 100.0f;
	enemy.dir = 1.0f;
	enemy.minX = platforms[2].x;
	enemy.maxX = platforms[2].x + platforms[2].w - enemy.w;
	enemy.throwTimer = 2.0f;
	enemy.shot.size = 30.0f;
	enemy.shot.active = 0;

	CP_System_SetFrameRate(120.0f);
}

void lvl1_Update(void)
{
	Character_MovementDefault(1);
	NPC_Movement(&enemy);
	NPC_Attack(&enemy, NPC_CanSeePlayer(&enemy, platforms, PLATFORM_COUNT));

	for (int i = 0; i < PLATFORM_COUNT; i++) {
		collision_check(platforms[i].x + platforms[i].w / 2.0f,
			platforms[i].y + platforms[i].h / 2.0f,
			platforms[i].w, platforms[i].h);
	}
}

void lvl1_Exit(void) {

}


/* TEMPORARY: remove once the real Win_Screen is in the project */
void Win_Screen(void)
{
}
#endif

/*
* 
* Use this to call your enemies in levels 
float player_spawn[LEVELS][PLAYER_SPAWN_POS] = {
					{ 800.0f, 100.0f },
					{ 100.0f, 100.0f },
					{ 200.0f, 100.0f },
					{ 400.0f, 100.0f }
};


Character_Starting_Pos(player_spawn, level_selected);
To spawn your enemies
*/

