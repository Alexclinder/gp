#pragma once
#include  "cprocessing.h"

extern CP_Image jump;
extern CP_Image g1;
extern CP_Image g2;
extern CP_Image carry;
extern CP_Image Zilong;
extern CP_Image background;
extern CP_Image backgroundtwo;
extern CP_Image backgroundthree;
extern CP_Image platform;
extern CP_Image npc1;
extern CP_Image bossBG;
extern CP_Image demonBoss;
extern CP_Image projectile;
enum { OBJ_DESK, OBJ_PRINTER, OBJ_TYPE_COUNT };

extern CP_Image objImg[OBJ_TYPE_COUNT];
extern CP_Image objBG[OBJ_TYPE_COUNT];

void Game_Init(void);

void Game_Reset(void);

void Game_Update(void);

void Game_Exit(void);

void Game_Next_Level(void);

