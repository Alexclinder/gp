/*!
@file     Images.h
@author   Lee Jun Kai (l.junkai)
@co-author Lewin Yeo
@module   csd1401f26 Software Engineering Project 1
@section  B
@date     20/10/26
@brief    Declarations for the image constants
*//*______________________________________________________________________*/
#pragma once
#include "cprocessing.h"
#include "game.h"

extern CP_Image mainbackground;
extern CP_Image frame;
extern CP_Image levelselection;
extern CP_Image defeat;
extern CP_Image pausemenu;
extern CP_Font Font;

extern CP_Image jump;
extern CP_Image g1;
extern CP_Image g2;
extern CP_Image carry;
extern CP_Image Zilong;
extern CP_Image background;
extern CP_Image backgroundtwo;
extern CP_Image backgroundthree;
extern CP_Image backgroundfour_pre;
extern CP_Image platform;
extern CP_Image npc1;
extern CP_Image bossBG;
extern CP_Image vent;
extern CP_Image demonBoss;
extern CP_Image projectile;
extern CP_Image objImg[OBJ_TYPE_COUNT];
extern CP_Image objBG[OBJ_TYPE_COUNT];

/*!
@brief  Loads the images and font used by the menu screens: main menu,
        defeat, level select, button frame and pause menu, as well as
        the level backgrounds shown on the leaderboard screen.
        Should be called once at start-up.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void LoadImages(void);

/*!
@brief  Loads the images and font used during gameplay: player sprites,
        level and boss backgrounds, NPC, demon boss, vent, platform,
        projectile, and the object and object-background images.
        Should be called when a level is entered.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void LoadLevelImages(void);

/*!
@brief  Frees the gameplay images loaded by LoadLevelImages(), including
        the object and object-background image arrays. Should be called
        once when leaving a level.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void FreeLevelImages(void);

