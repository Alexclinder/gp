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

void Game_Init(void);

void Game_Reset(void);

void Game_Update(void);

void Game_Exit(void);

void Game_Next_Level(void);