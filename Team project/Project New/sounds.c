/*!
@file     sounds.c
@author	  Lewin Yeo
@module   csd1401f26 Software Engineering Project 1
@section  B
@date     20/10/26
@brief    Implementation for the sound constants
*//*______________________________________________________________________*/
#include "cprocessing.h"
#include "sounds.h"
#include "screen_seq.h"

/*****Player Ability Sounds*****/
CP_Sound JumpSound;
CP_Sound teleportSound;
CP_Sound grabSound;
CP_Sound dropSound;
CP_Sound kickSound;
CP_Sound ShieldSound;

/*****Music*****/
CP_Sound backgroundMusic;
CP_Sound PlayPauseMenu;
CP_Sound level_MusicP1;

/*****Game State Sounds*****/
CP_Sound gameOverSound;
CP_Sound winSound;
CP_Sound loseSound;

/*****Menu Sounds*****/
CP_Sound buttonClickSound;
CP_Sound buttonHoverSound;


/*****Boss Music & Enemy Sounds*****/
CP_Sound bossMusicBGP1;
CP_Sound bossMusicBGP2;
CP_Sound bossAttackSound;
CP_Sound bossRoarSound;
CP_Sound enemy_Shoot;


/*Sound Effects for Objects*/
CP_Sound Treasure_Sound;
CP_Sound ObjectPickupSound;



enum {

};
void LoadSounds(void) {
	// Load sound files
	/*****Main Menu BGMusic Sounds*****/
	backgroundMusic = CP_Sound_LoadMusic("Assets/Sounds/Music/MainMenu.wav");
	//Pause Menu
	PlayPauseMenu = CP_Sound_LoadMusic("Assets/Sounds/Music/PauseMenu.wav");

	/*****Player Ability Sounds*****/
	JumpSound = CP_Sound_LoadMusic("Assets/Sounds/Sound Effects/Player_Jump.wav");
	teleportSound = CP_Sound_LoadMusic("Assets/Sounds/Sound Effects/Teleport_Sound.wav");
	ShieldSound = CP_Sound_LoadMusic("Assets/Sounds/Sound Effects/Shield_Sound.wav");

	/*****Level Music & Sounds*****/
	level_MusicP1 = CP_Sound_LoadMusic("Assets/Sounds/Music/Level_MusicPart1.wav");
	loseSound = CP_Sound_LoadMusic("Assets/Sounds/Sound Effects/Lose.wav");
	winSound = CP_Sound_LoadMusic("Assets/Sounds/Sound Effects/Win.ogg");

	Treasure_Sound = CP_Sound_LoadMusic("Assets/Sounds/Sound Effects/Pick_up_Treasure.wav");
	ObjectPickupSound = CP_Sound_LoadMusic("Assets/Sounds/Sound Effects/Pick_Up Object.wav");

	/*****Boss Music & Enemy Sounds*****/
	enemy_Shoot = CP_Sound_LoadMusic("Assets/Sounds/Sound Effects/Enemy_Shoot.wav");
	bossMusicBGP1 = CP_Sound_LoadMusic("Assets/Sounds/Music/Boss_Music_P1.wav");
	bossMusicBGP2 = CP_Sound_LoadMusic("Assets/Sounds/Music/Boss_Music_P2.wav");
}


void PlayMainMenu(int *PlayOnce) {

	if (*PlayOnce == 1) {
		CP_Sound_PlayAdvanced(backgroundMusic, 1.0f, 1.0f,1, CP_SOUND_GROUP_1);
		*PlayOnce = 0;
	}
}


void PlaylevelMusicP1(void) {
	CP_Sound_PlayAdvanced(level_MusicP1, 0.4f, 1.0f, 1, CP_SOUND_GROUP_2);
}

void PauseMenuMusic(int* PlayOnce) {
	if (*PlayOnce == 1) {
		CP_Sound_PlayAdvanced(PlayPauseMenu, 0.1f, 1.0f, 1, CP_SOUND_GROUP_3);
		*PlayOnce = 0;
	}
}

void PlayLoseSound(int* PlayOnce) {
	if (*PlayOnce == 1) {
		CP_Sound_PlayAdvanced(loseSound, 0.1f, 1.0f, 0, CP_SOUND_GROUP_4);
		*PlayOnce = 0;
	}
}
void PlayWinSound(int* PlayOnce) {
	if (*PlayOnce == 1) {
		CP_Sound_PlayAdvanced(winSound, 0.1f, 1.0f, 0, CP_SOUND_GROUP_4);
		*PlayOnce = 0;
	}
}
void PlayBossMusicP1(int* PlayOnce) {
	if (*PlayOnce == 1) {
		CP_Sound_PlayAdvanced(bossMusicBGP1, 0.7f, 1.0f, 1, CP_SOUND_GROUP_5);//Plays Boss Music Part 1
		*PlayOnce = 0;
	}
}




void StopMusic(int* PlayOnce) {
	CP_Sound_StopAll();
	*PlayOnce = 1;
}


