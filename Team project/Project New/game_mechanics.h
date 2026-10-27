#pragma once

#define PLAYER_SIZE 100.f
extern int isCarrying;
extern int pickingUp;
extern int dropping;
extern float carry_timer;
extern int onGround;
extern int player_action;
extern CP_Vector position;
extern CP_Vector velocity;

int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y);

int IsCircleClicked(float circle_center_x, float circle_center_y, float diameter, float click_x, float click_y);

void Character_MovementDefault(void);

void Character_MovementCarry(void);

void DrawPlatform(float x, float y, float w, float h, float degrees, float cornerRadius);

int sphere_aabb_intersection(float cx, float cy, float radius, float x, float y, float w, float h);

void Player_Animation_Checks(void);

void Player_Animation(void);

int CheckWinCondition(float x, float y, float w, float h, float degrees, float cornerRadius);

int collision_check(float area_center_x, float area_center_y, float area_width, float area_height);

void Starting_Pos(float arr[][2], int level_selected);

