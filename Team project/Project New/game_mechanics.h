#pragma once

#define PLAYER_SIZE 50.f
#define SHIELD_SIZE (PLAYER_SIZE * 1.6f)  // diameter of the circle
#define SHIELD_DURATION    1.0f                  // total time shield exists
#define SHIELD_PARRY_TIME  0.5f                  // reflect window

extern int   shieldActive;
extern float shieldTimer;

extern CP_Image npc1;
extern int isCarrying;
extern int pickingUp;
extern int dropping;
extern float carry_timer;
extern int onGround;
extern int player_action;
extern int kickedObject;
extern CP_Vector position;
extern CP_Vector velocity;
extern float platform_velocity;
extern float platform_move;
extern int Teleport_count;
extern int   is_clinging;
extern int   cling_side;
extern float wall_timer; 

void Draw_ClingBar(void);

void Shield_Draw(void);
void Shield_Update(void);

void TeleportTimer(void);
int Teleport_Mechannic(void);

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

void Player_Starting_Pos(struct spawn arr[], int level_selected);

void Pressure_Plate_Door(float pres_x, float pres_y, float pres_width, float pres_height,
					     float door_x, float door_y, float door_width, float door_height);

void kill_zone(float x, float y, float w, float h);

void platform_alternating_movement(float x, float y, float w, float h,
	float degrees, float cornerRadius,
	float left_boundary, float right_boundary);



extern int* score;

void game_score(void);

extern int reflect_score;
extern int npc_score;
extern int pressed;