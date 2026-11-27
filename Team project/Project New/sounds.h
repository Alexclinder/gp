/*!
@file     sounds.h
@author   Lewin Yeo
@module   csd1401f26 Software Engineering Project 1
@section  B
@date     20/10/26
@brief    Declarations for the sound constants
*//*______________________________________________________________________*/
#pragma once

/* All sounds below are defined in sounds.c. They are only valid after
   LoadSounds() has been called. Note that this header uses CP_Sound
   but does not include "cprocessing.h" itself. */

/*****Player Ability Sounds*****/
extern CP_Sound JumpSound;
extern CP_Sound teleportSound;
extern CP_Sound grabSound;
extern CP_Sound dropSound;
extern CP_Sound kickSound;
extern CP_Sound ShieldSound;

/*****Music*****/
extern CP_Sound backgroundMusic;
extern CP_Sound level_Music;
extern CP_Sound level_MusicP1;

/*****Game State Sounds*****/
extern CP_Sound gameOverSound;
extern CP_Sound winSound;
extern CP_Sound loseSound;

/*****Menu Sounds*****/
extern CP_Sound buttonClickSound;
extern CP_Sound buttonHoverSound;

/*****Boss Music & Enemy Sounds*****/
extern CP_Sound bossMusicBGP1;
extern CP_Sound bossMusicBGP2;
extern CP_Sound bossAttackSound;
extern CP_Sound bossRoarSound;
extern CP_Sound enemy_Shoot;

/*Sound Effects for Objects*/
extern CP_Sound Treasure_Sound;
extern CP_Sound ObjectPickupSound;

/*!
@brief  Loads the sound files used by the game from the Assets/Sounds
        folder and assigns them to the shared sound variables declared
        in this header. Should be called once at start-up.
        Currently only backgroundMusic and teleportSound are loaded;
        the remaining sounds are not yet loaded.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void LoadSounds(void);
void LoadSounds(void);
void PlayMainMenu(int *PlayOnce);//Main Menu
void PauseMenuMusic(int* PlayOnce);//Pause Menu
void PlayWinSound(int* PlayOnce);//Win Menu Sound
void PlayLoseSound(int* PlayOnce);//Lose Menu Sound
void PlaylevelMusicP1(void);//Level Music
void PlayBossMusicP1(int* PlayOnce);//Boss Music Part 1
void StopMusic(int* PlayOnce);