#include "cprocessing.h"
#include "sounds.h"
#include "screen_seq.h"

/*****Player Ability Sounds*****/
CP_Sound teleportSound;
CP_Sound grabSound;
CP_Sound dropSound;
CP_Sound kickSound;
CP_Sound shieldSound;

/*****Music*****/
CP_Sound backgroundMusic;
CP_Sound level_Music;
CP_Sound level_MusicP1;

/*****Game State Sounds*****/
CP_Sound gameOverSound;
CP_Sound winSound;

/*****Menu Sounds*****/
CP_Sound buttonClickSound;
CP_Sound buttonHoverSound;
CP_Sound levelSelectSound;
CP_Sound pauseSound;
CP_Sound resumeSound;

/*****Boss Music & Sounds*****/
CP_Sound bossMusic;
CP_Sound bossAttackSound;
CP_Sound bossDefeatSound;
CP_Sound bossPhaseChangeSound;
CP_Sound bossRoarSound;
CP_Sound bossSummonSound;
CP_Sound Keybooard_Attack_Sound;

enum {

};
void LoadSounds(void) {
	// Load sound files
	/*****Main Menu Music Sounds*****/
	backgroundMusic = CP_Sound_LoadMusic("Assets/Sounds/Music/MainMenu.wav");

	/*****Player Ability Sounds*****/
	teleportSound = CP_Sound_LoadMusic("Assets/Sounds/Sound Effects/Teleport_Sound.wav");

	/*****Level Music & Sounds*****/
	level_MusicP1 = CP_Sound_LoadMusic("Assets/Sounds/Music/Level_MusicPart1.wav");

	/*****Boss Music & Sounds*****/
}

void PlayMainMenu(int *PlayOnce) {

	if (*PlayOnce == 1) {
		CP_Sound_PlayAdvanced(backgroundMusic, 1.0f, 1.0f,1, CP_SOUND_GROUP_1);
		*PlayOnce = 0;
	}
}


void PlaylevelMusicP1(void) {
	CP_Sound_PlayAdvanced(level_MusicP1, 0.5f, 1.0f, 1, CP_SOUND_GROUP_2);

}
void StopMusic(int* PlayOnce) {
	CP_Sound_StopAll();
	*PlayOnce = 1;
}


