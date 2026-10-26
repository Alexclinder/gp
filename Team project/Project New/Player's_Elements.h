#pragma once
#include "cprocessing.h"
/*USE THIS HEADER FILE TO ASSIGN GLOBAL VARIABLES & DEFINITIONS FOR ALL SOURCE FILES*/
#define PLAYER_SIZE    100.0f
#define MOVE_SPEED     300.0f
#define JUMP_SPEED     650.0f
#define GRAVITY        1500.0f
#define WALL_CLING_TIME 2.0f
#define CLIMB_SPEED     250.0f
#define MAX_FALL_SPEED 900.0f
#define GRAB_FRAME_TIME    0.15f

CP_Image jump;
CP_Image g1;
CP_Image g2;
CP_Image carry;
CP_Image Zilong;
CP_Image background;
CP_Image backgroundtwo;
CP_Image backgroundthree;

/*Variable declrations can still assign new values,
but if no value is assigned checks memory & its stored value*/
/*Player Position & Velocity Vectors*/
extern CP_Vector velocity;
extern CP_Vector position;
extern int onGround_status;
extern int wall_climb_status;