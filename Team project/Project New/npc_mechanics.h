/*!
@file     npc_mechanics.h
@author   Paul David Sheeba Christobel Kezin David (david.p)
@module   csd1401f26 Software Engineering Project 1
@section  B
@date     23/09/26
@brief	  Declarations for the NPC: its movement, its projectile attack,
          and the collision checks between the projectile, the player,
          the player's shield and the NPC itself.
*//*______________________________________________________________________*/
#pragma once

#include "cprocessing.h"
#include "level.h"

/*!
@brief  Updates and draws the NPC. Applies gravity, snaps the NPC to the
        floor, and walks it left or right at 150 pixels per second while
        it is on the ground, turning around at the screen edges. Then
        draws the NPC image at NPC_SIZE x NPC_SIZE. The NPC only walks on
        the floor and ignores platforms. Does nothing if the NPC is dead.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void NPC_MovementDefault(void);

/*!
@brief  Handles the NPC's shooting. Counts up a timer, and once
        SHOOT_INTERVAL (3) seconds have passed and no projectile is
        active, fires a projectile from the NPC's centre towards the
        player's centre at 400 pixels per second. The new projectile is
        marked as not reflected. Does nothing if the NPC is dead.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void NPC_Attack(void);

/*!
@brief  Moves or clears the NPC projectile.

@param  check  1 to move the active projectile by its velocity for this
               frame, removing it once it leaves the screen.
               0 to remove the projectile immediately and reset the
               shooting timer (used when the level ends or restarts).

@return None.
*//*_______________________________________________________________________*/
void NPC_UpdateProjectiles(int);

/*!
@brief  Draws the active projectile as a red circle at its current
        position. Draws nothing if there is no active projectile.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void NPC_DrawProjectiles(void);

/*!
@brief  Checks whether the projectile has hit the player. The player and
        projectile are both treated as circles around their centres.
        A reflected projectile cannot hurt the player. On a hit the
        projectile is removed.

@param  None.

@return 1 if the player was hit, otherwise 0.
*//*_______________________________________________________________________*/
int NPC_CheckPlayerCollision(void);

/*!
@brief  Checks whether the projectile has hit the player's shield. If the
        shield is within its first SHIELD_PARRY_TIME seconds, the
        projectile is reflected: its velocity is reversed, it is marked
        as reflected, and NPC_reflected is set. After that window the
        shield absorbs it and the projectile is removed. Does nothing if
        there is no projectile, the shield is not up, or the projectile
        was already reflected.

@param  None.

@return 1 if the projectile was reflected, 2 if it was blocked (absorbed),
        0 if there was no hit.
*//*_______________________________________________________________________*/
int NPC_CheckShieldCollision(void);

/*!
@brief  Checks whether a reflected projectile has hit the NPC. On a hit the
        NPC is removed (NPCalive set to 0), NPC_ded is set, and the
        projectile is removed. Does nothing if the NPC is already dead
        or the projectile is not reflected.

@param  None.

@return 1 if the NPC was hit, otherwise 0.
*//*_______________________________________________________________________*/
int NPC_CheckReflectedHit(void);     

/*!
@brief  Places the NPC at the spawn point of the given level and brings it
        back to life. Call when a level starts or restarts.

@param  arr             Array of spawn points, indexed by level.
@param  level_selected  Index of the level whose spawn point is used.

@return None.
*//*_______________________________________________________________________*/
void NPC_Starting_Pos(spawn arr[], int level_selected);

extern int NPCalive;        /*!< 1 while the NPC is alive, 0 after a reflected projectile has hit it. */
extern int ProjReflected;   /*!< 1 if the active projectile was parried by the shield. A reflected
                                 projectile cannot hurt the player, but it can kill the NPC. */


/* Flags used by the scoring system. */

extern int NPC_ded;         /*!< Set to 1 when the NPC is killed by a reflected projectile.
                                 Used by scoring to award points for the kill. */
extern int NPC_reflected;   /*!< Set to 1 when a projectile is reflected by the shield.
                                 Used by scoring to award points for the parry. */
extern int unlocked_gate_score; /*!< Flag so the gate-unlock score is only awarded once per level.
                                     Defined in game_mechanics.c and also declared there. */
extern int ProjectileActive;    /*!< 1 while an NPC projectile exists on screen, otherwise 0.
                                     Only one projectile can exist at a time. */
