#pragma once

int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y);

int IsCircleClicked(float circle_center_x, float circle_center_y, float diameter, float click_x, float click_y);

void Character_Starting_Pos(float Character_x, float Character_y);
float Character_MovementDefault_x(void);
float Character_MovementDefault_y(void);

void Character_MovementCarry(void);

int sphere_aabb_intersection(float cx, float cy, float radius, float x, float y, float w, float h);

void DrawPlatform(float x, float y, float w, float h,
	float degrees, float cornerRadius);

int Win_con(float area_corner_x, float area_corner_y, float area_width, float area_height, float Player_PosX, float Player_PosY, float Player_Size);