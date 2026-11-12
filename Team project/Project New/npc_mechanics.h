#pragma once

#include "cprocessing.h"
#include "level.h"

void NPC_MovementDefault(void);
void NPC_Attack(void);
void NPC_UpdateProjectiles(int);
void NPC_DrawProjectiles(void);

// Check Collision between player and Projectile 
int NPC_CheckPlayerCollision(void);
int NPC_CheckShieldCollision(void);
int NPC_CheckReflectedHit(void);     // ADDED

void NPC_Starting_Pos(spawn arr[], int level_selected);

extern int NPCalive;
extern int ProjReflected;

// score is locked to prevent inf loop
extern int NPC_ded;
extern int NPC_reflected;
extern int unlocked_gate_score;
extern int ProjectileActive;