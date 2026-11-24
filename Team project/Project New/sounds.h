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
extern CP_Sound teleportSound;
extern CP_Sound grabSound;
extern CP_Sound dropSound;
extern CP_Sound kickSound;
extern CP_Sound shieldSound;

/*****Music*****/
extern CP_Sound backgroundMusic;
extern CP_Sound gameplayMusic;

/*****Game State Sounds*****/
extern CP_Sound gameOverSound;
extern CP_Sound winSound;

/*****Menu Sounds*****/
extern CP_Sound buttonClickSound;
extern CP_Sound buttonHoverSound;
extern CP_Sound levelSelectSound;
extern CP_Sound pauseSound;
extern CP_Sound resumeSound;

/*****Boss Music & Sounds*****/
extern CP_Sound bossMusic;
extern CP_Sound bossAttackSound;
extern CP_Sound bossDefeatSound;
extern CP_Sound bossPhaseChangeSound;
extern CP_Sound bossRoarSound;
extern CP_Sound bossSummonSound;
extern CP_Sound Keybooard_Attack_Sound;

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