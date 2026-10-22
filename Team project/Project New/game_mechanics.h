#pragma once

int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y);

int IsCircleClicked(float circle_center_x, float circle_center_y, float diameter, float click_x, float click_y);

void Character_MovementDefault(int level);

void Character_MovementCarry(void);

void DrawPlatform(float x, float y, float w, float h, float degrees, float cornerRadius);

int sphere_aabb_intersection(float cx, float cy, float radius, float x, float y, float w, float h);

int WinCondition(float x, float y, float w, float h, float degrees, float cornerRadius);

int collision_check(float area_center_x, float area_center_y, float area_width, float area_height);