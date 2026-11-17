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


void LoadImages(void);

void LoadLevelImages(void);

void FreeLevelImages(void);

