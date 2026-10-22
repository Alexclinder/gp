#pragma once

typedef struct {
	float x, y, w, h;
} Platform;

typedef struct {
	float x, y;
	float vx, vy;
	float size;
	int active;       /* 1 = flying, 0 = not thrown */
} Projectile;

typedef struct {
	float x, y;
	float w, h;
	float speed;
	float dir;
	float minX, maxX;
	float throwTimer; /* seconds until the next throw */
	Projectile shot;
	CP_Image image;
} NPC;

void Win_Screen(void); //remove ltr

int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y);

int IsCircleClicked(float circle_center_x, float circle_center_y, float diameter, float click_x, float click_y);

void Character_MovementDefault(int level);

void Character_MovementCarry(void);

void DrawPlatform(float x, float y, float w, float h, float degrees, float cornerRadius);

int sphere_aabb_intersection(float cx, float cy, float radius, float x, float y, float w, float h);

void Player_Animation_Checks(float dt);

void Player_Animation(void);

int WinCondition(float x, float y, float w, float h, float degrees, float cornerRadius);

int collision_check(float area_center_x, float area_center_y, float area_width, float area_height);

//NPC Attack and check if within Line of sight
void NPC_Attack(NPC* n, int canSee);
int NPC_CanSeePlayer(NPC* n, Platform* plats, int count);
void Character_Update(void);
void Character_Draw(void);


