#pragma once

int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y);

int IsCircleClicked(float circle_center_x, float circle_center_y, float diameter, float click_x, float click_y);

float Character_MovementDefault_x(void);
float Character_MovementDefault_y(void);

void Character_MovementCarry(void);