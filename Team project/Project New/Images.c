/*!
@file     Images.c
@author   Lee Jun Kai (l.junkai)
@co-author Lewin Yeo - Free Level Images
@module   csd1401f26 Software Engineering Project 1
@section  B
@date     15/11/26
@brief  Defines the shared image and font variables, and implements the functions
        that load and free them.
*//*______________________________________________________________________*/
#include "Images.h"
#include "game.h"

static int level_images_loaded = 0;

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

/*
LoadImages

LOAD menu images (main menu, defeat, level select, button frame, pause)
LOAD menu font
LOAD level background images for the leaderboard screen
*/
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

/*
LoadLevelImages

IF level images are already loaded
    RETURN

LOAD game font
LOAD player sprites (jump, grab 1, grab 2, carry, standing)
LOAD level backgrounds and boss background
LOAD NPC, boss, vent, platform and projectile images
LOAD object images and their highlight images

SET level images as loaded
*/
void LoadLevelImages(void)
{
    if (level_images_loaded)
        return;

    Font = CP_Font_Load("Assets/Exo2-Regular.ttf");

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
    demonBoss = CP_Image_Load("Assets/DemonBoss.png");
    vent = CP_Image_Load("Assets/Vent.png");
    platform = CP_Image_Load("Assets/Platform.png");
    projectile = CP_Image_Load("Assets/paperball.png");

    objImg[OBJ_DESK] = CP_Image_Load("Assets/desk.png");
    objBG[OBJ_DESK] = CP_Image_Load("Assets/desk_background.png");
    objImg[OBJ_PRINTER] = CP_Image_Load("Assets/printer.png");
    objBG[OBJ_PRINTER] = CP_Image_Load("Assets/printer_background.png");

    level_images_loaded = 1;
}

/*
FreeLevelImages

IF level images are not loaded
    RETURN

FREE all level images
FREE all object images and highlight images
SET every freed image to NULL

SET level images as not loaded
*/
void FreeLevelImages(void)
{
    if (!level_images_loaded)
        return;

    CP_Image_Free(g1);
    CP_Image_Free(g2);
    CP_Image_Free(carry);
    CP_Image_Free(background);
    CP_Image_Free(backgroundtwo);
    CP_Image_Free(backgroundthree);
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

        objImg[i] = NULL;
        objBG[i] = NULL;
    }

    g1 = NULL;
    g2 = NULL;
    carry = NULL;
    background = NULL;
    backgroundtwo = NULL;
    backgroundthree = NULL;
    bossBG = NULL;
    jump = NULL;
    Zilong = NULL;
    npc1 = NULL;
    demonBoss = NULL;
    vent = NULL;
    platform = NULL;
    projectile = NULL;

    level_images_loaded = 0;
}
