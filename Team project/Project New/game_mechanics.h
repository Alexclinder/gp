#pragma once


void DrawPlatform(float x, float y, float w, float h, float degrees, float cornerRadius);

int sphere_aabb_intersection(float cx, float cy, float radius, float x, float y, float w, float h);

int CheckWinCondition(float x, float y, float w, float h, float degrees, float cornerRadius);

int collision_check(float area_center_x, float area_center_y, float area_width, float area_height);
