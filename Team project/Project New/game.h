#pragma once
#include  "cprocessing.h"


enum { OBJ_DESK, OBJ_PRINTER, OBJ_TYPE_COUNT };


void Game_Init(void);

void Game_Reset(void);

void Game_Update(void);

void Game_Exit(void);

void Game_Next_Level(void);

