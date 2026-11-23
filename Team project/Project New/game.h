/*!
@file     game.h
@author   Sin Wee, Jun Kai
@module   csd1401f26 Software Engineering Project 1
@section  B
@date     19/09/26
@brief    Declarations for the core game loop functions and the object type enum
*//*______________________________________________________________________*/
#pragma once
#include  "cprocessing.h"

/*!
@brief  Identifiers for the types of interactable objects in a level.
        Used to index the objImg and objBG image arrays.
        OBJ_TYPE_COUNT is not a real object type. It is the number of
        types, used for array sizes and loops.
*//*_______________________________________________________________________*/
enum { OBJ_DESK, OBJ_PRINTER, OBJ_TYPE_COUNT };

/*!
@brief  Initialises the gameplay state when a level is first entered.
        Loads the gameplay font and level images, creates the shared
        colors, resets the objects and level score, clears any active
        projectile, sets the frame rate to 120 FPS, and resets the
        ground and timer flags.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void Game_Init(void);

/*!
@brief  Resets the current level (level_selected) to its starting state,
        for example when the level is restarted or a new level begins.
        Reloads the level images, restores the teleport count (4 in the
        boss level, 3 otherwise), clears the carry, pick-up and drop
        flags, resets objects, timers, scores, collected treasures and
        the kill zone, and moves the player and NPCs to their spawn
        positions. If the current level is the boss level, it finishes
        by calling Elevator_Reset() so the boss level's positions
        override the ones set earlier.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void Game_Reset(void);

/*!
@brief  Per-frame update function for gameplay. Runs the current level
        via level_selection(), handles pausing with the Escape key,
        and updates the objective text, timers, teleport mechanic, NPCs
        and their projectiles, player movement (carrying or default),
        and the shield. Switches to the lose screen if the player is
        hit by an NPC projectile. Draws objects, the player, the cling
        bar and the shield, and saves the level score.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void Game_Update(void);

/*!
@brief  Cleans up the gameplay state. Frees the level images and the
        gameplay font. Should be called when leaving gameplay.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void Game_Exit(void);

/*!
@brief  Advances to the next level. Increments level_selected and
        calls Game_Reset() to set up the new level.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void Game_Next_Level(void);

