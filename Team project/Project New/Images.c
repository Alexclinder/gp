#include "Images.h"
#include "game.h"

CP_Image mainbackground;
CP_Image frame;
CP_Image levelselection;
CP_Image defeat;
CP_Image pausemenu;
CP_Font Font;

CP_Image jump;
CP_Image g1;
CP_Image g2;
CP_Image carry;
CP_Image Zilong;
CP_Image background;
CP_Image backgroundtwo;
CP_Image backgroundthree;
CP_Image backgroundfour_pre;
CP_Image platform;
CP_Image projectile;
CP_Image objImg[OBJ_TYPE_COUNT];
CP_Image objBG[OBJ_TYPE_COUNT];
CP_Image bossBG;
CP_Image demonBoss;
CP_Image vent;
CP_Image npc1;

void LoadImages(void) {
    mainbackground = CP_Image_Load("Assets/MainMenu.png");
    defeat = CP_Image_Load("Assets/Defeat.png");
    levelselection = CP_Image_Load("Assets/LevelSelect.png");
    frame = CP_Image_Load("Assets/buttonframe.png");
    pausemenu = CP_Image_Load("Assets/pause.png");
    Font = CP_Font_Load("Assets/SuperAdorable-MAvyp.ttf");

	// Used to display images in leaderboard screen
	background = CP_Image_Load("Assets/level3background.png");
	backgroundtwo = CP_Image_Load("Assets/level2background.png");
	backgroundthree = CP_Image_Load("Assets/level1background.png");
	backgroundfour_pre = CP_Image_Load("Assets/bossBG.png");
}

void LoadLevelImages(void) {
	jump = CP_Image_Load("Assets/Jumping.png");
	g1 = CP_Image_Load("Assets/Grab1.png");
	g2 = CP_Image_Load("Assets/Grab2.png");
	carry = CP_Image_Load("Assets/Carry.png");
	background = CP_Image_Load("Assets/level3background.png");
	backgroundtwo = CP_Image_Load("Assets/level2background.png");
	backgroundthree = CP_Image_Load("Assets/level1background.png");
	bossBG = CP_Image_Load("Assets/bossBG.png");
	Zilong = CP_Image_Load("Assets/Main_Character.png");
	npc1 = CP_Image_Load("Assets/Worker.png");
	demonBoss = CP_Image_Load("Assets/DemonBoss.png"); //load Boss
	vent = CP_Image_Load("Assets/Vent.png");
	platform = CP_Image_Load("Assets/Platform.png");
	projectile = CP_Image_Load("Assets/paperball.png");
	objImg[OBJ_DESK] = CP_Image_Load("Assets/desk.png");
	objBG[OBJ_DESK] = CP_Image_Load("Assets/desk_background.png");
	objImg[OBJ_PRINTER] = CP_Image_Load("Assets/printer.png");
	objBG[OBJ_PRINTER] = CP_Image_Load("Assets/printer_background.png");
}

void FreeLevelImages(void) {
	CP_Image_Free(g1);
	CP_Image_Free(g2);
	CP_Image_Free(carry);
	CP_Image_Free(background);
	CP_Image_Free(backgroundtwo);
	CP_Image_Free(backgroundthree);
	CP_Image_Free(backgroundfour_pre);
	CP_Image_Free(bossBG);
	CP_Image_Free(jump);
	CP_Image_Free(Zilong);
	CP_Image_Free(npc1);
	CP_Image_Free(demonBoss);
	CP_Image_Free(vent);
	CP_Image_Free(platform);
	CP_Image_Free(projectile);

	for (int i = 0; i < OBJ_TYPE_COUNT; i++) {
		CP_Image_Free(objImg[i]);
		CP_Image_Free(objBG[i]);
	}
}
