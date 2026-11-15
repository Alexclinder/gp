#include "Images.h"

CP_Image mainbackground;
CP_Image frame;
CP_Image levelselection;
CP_Image defeat;
CP_Image pausemenu;
CP_Font Font;

void LoadImages(void) {
    mainbackground = CP_Image_Load("Assets/MainMenu.png");
    defeat = CP_Image_Load("Assets/Defeat.png");
    levelselection = CP_Image_Load("Assets/LevelSelect.png");
    frame = CP_Image_Load("Assets/buttonframe.png");
    pausemenu = CP_Image_Load("Assets/pause.png");
    Font = CP_Font_Load("Assets/SuperAdorable-MAvyp.ttf");
}
