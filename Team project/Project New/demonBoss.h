/*!
@file     demonBoss.h
@author   Paul David Sheeba Christobel Kezin David (david.p)
@module   csd1401f26 Software Engineering Project 1
@section  B
@date     27/10/26
@brief    Declarations for the demon boss functionality
*//*______________________________________________________________________*/
#pragma once   

/*!
@brief  Initialises the demon boss. Resets the boss state machine to the
        hover state, clears the state timer, and places the boss at a
        random position inside the boss safe zone at the top of the screen.
        Should be called once when the boss level is loaded or restarted.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void BOSS_Init(void);

/*!
@brief  Updates the boss state machine and movement for the current frame.
        The boss cycles through three states:
          - BOSS_HOVER: waits in place until the attack timer elapses
            (BOSS_ATTACK_TIME seconds), then locks onto the player's
            current position.
          - BOSS_ATTACK: charges at the locked position at
            BOSS_ATTACK_SPEED, stopping at BOSS_STOP_DISTANCE from the
            target. If it comes within BOSS_DAMAGE_DISTANCE of the player
            it deals damage once per attack (unless the shield is active),
            and the game switches to the lose screen if player health
            reaches zero.
          - BOSS_RETURN: flies back to a new random safe-zone position at
            BOSS_RETURN_SPEED, then resumes hovering.
        Movement is frame-rate independent (scaled by delta time).

@param  None.

@return None.
*//*_______________________________________________________________________*/
void BOSS_Movement(void);

/*!
@brief  Draws the demon boss image at its current position, scaled to
        BOSS_SIZE x BOSS_SIZE with full opacity.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void BOSS_Draw(void);

/*!
@brief  Per-frame update function for the boss. Calls BOSS_Movement() to
        update the boss state and position, then BOSS_Draw() to render it.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void BOSS_Update(void);

/*!
@brief  Cleans up the demon boss. Frees the demon boss image and the
        level images. Should be called when leaving the boss level to
        avoid memory leaks.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void BOSS_Exit(void);