#pragma once



/* abilities.h */
void Draw_ClingBar(void);

// move wall_climb inside collision to abilities.h

extern int   shieldActive;
extern float shieldTimer;

extern int   is_clinging;
extern int   cling_side;
extern float wall_timer;

#define PLAYER_SIZE 50.f
#define SHIELD_SIZE (PLAYER_SIZE * 1.6f)  // diameter of the circle
#define SHIELD_DURATION    1.0f                  // total time shield exists
#define SHIELD_PARRY_TIME  0.5f                  // reflect window

void Shield_Draw(void);
void Shield_Update(void);

extern float TeleportWindowTiming;
extern int Teleport_Status;
extern int Teleport_count;
void TeleportTimer(void);
int Teleport_Mechannic(void);

//---------------------------------------------//

int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y);


/* Character_Movement.h */
extern int isCarrying;
extern int pickingUp;
extern int dropping;
extern float carry_timer;
extern int onGround;
extern int player_action;
extern int kickedObject;
extern CP_Vector position;
extern CP_Vector velocity;


void Character_MovementDefault(void);

void Character_MovementCarry(void);

void DrawPlatform(float x, float y, float w, float h, float degrees, float cornerRadius);


/* collision.h */
int sphere_aabb_intersection(float cx, float cy, float radius, float x, float y, float w, float h);
int collision_check(float area_center_x, float area_center_y, float area_width, float area_height);

/* player_animation.h */
void Player_Animation_Checks(void);

void Player_Animation(void);
//--------------------------------


// level_mechanics.h
int CheckWinCondition(float x, float y, float w, float h, float degrees, float cornerRadius);

void Player_Starting_Pos(struct spawn arr[], int level_selected);
// ----------------------------

// gameplay_mechanics.h --> rename game_mechanics.h
extern int unlocked_gate_score;
extern int pressed;
void Pressure_Plate_Door(float pres_x, float pres_y, float pres_width, float pres_height,
					     float door_x, float door_y, float door_width, float door_height);

void kill_zone(float x, float y, float w, float h);

extern float platform_velocity;
extern float platform_move;

void platform_alternating_movement(float x, float y, float w, float h,
	float degrees, float cornerRadius,
	float left_boundary, float right_boundary);

void kill_zone_alternating_movement(float x, float y, float w, float h, float left_boundary, float right_boundary);
// ------------------------------------------------



