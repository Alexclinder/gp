/*!
@file     elevator.h
@author   Paul David Sheeba Christobel Kezin David (david.p)
@module   csd1401f26 Software Engineering Project 1
@section  B
@date     20/10/26
@brief    Declarations for the elevator functionality
*//*______________________________________________________________________*/
#pragma once 

/*!
@brief  Initialises the elevator (boss) level. Sets the NPC starting
        positions for the selected level, resets the whole level via
        Elevator_Reset(), and resets all boss-level status timers.
        Should be called once when the elevator level is entered.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void Elevator_Init(void);

/*!
@brief  Per-frame update and draw function for the elevator level.
        Each frame it:
          - Draws the boss background.
          - Advances the climb timer. When CLIMB_DURATION elapses,
            platform generation stops and the exit vent appears above
            the highest platform.
          - Scrolls the camera upward so the player never rises above
            SCROLL_Y on screen.
          - Updates the rising laser, which spawns after LASER_DELAY
            seconds.
          - Culls off-screen platforms and generates new ones above
            (until platforms have stopped).
          - Draws the platforms, vent, player health bar, boss
            (via BOSS_Update()), objects, player, cling bar, shield
            and lasers.
        Switches to the lose screen if the player is hit by an NPC
        projectile or touches the laser, and to the leaderboard screen
        if the player reaches the vent.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void Elevator_Update(void);

/*!
@brief  Cleans up the elevator level. Calls BOSS_Exit() to release the
        boss resources, then frees the demon boss image and the level
        images. Should be called when leaving the elevator level.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void Elevator_Exit(void);

/*!
@brief  Resets the elevator level to its starting state. Reloads the
        level images, resets the camera and platform generation,
        places the player at the bottom-left of the screen with zero
        velocity, restores player health to full, resets the laser,
        climb timer and vent, and re-initialises the boss via
        BOSS_Init(). Called by Elevator_Init() and can be used to
        restart the level.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void Elevator_Reset(void);

/*!
@brief  The player's current health, from 0 up to PLAYER_MAX_HEALTH
        (100). Defined in elevator.c and shared with other modules,
        for example demonBoss.c reduces it when the boss hits the
        player. Reset to full by Elevator_Reset().
*//*_______________________________________________________________________*/
extern float playerHealth;
