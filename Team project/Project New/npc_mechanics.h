#pragma once


void NPC_MovementDefault(void);
void NPC_Attack(void);
void NPC_UpdateProjectiles(void);
void NPC_DrawProjectiles(void);

// Check Collision between player and Projectile 
int NPC_CheckPlayerCollision(void);