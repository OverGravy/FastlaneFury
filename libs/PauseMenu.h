#ifndef _PAUSEMENU_H_
#define _PAUSEMENU_H_

#include "Menu.h"
#include "Game.h"

#define OPTION_NUM 1

extern struct option options[OPTION_NUM];  // array of options
extern struct MenuConfig pauseMenuConfig;         // pause menu config

// function that init the pause menu
void initPauseMenu();

// function that draws the pause menu
void DrawPauseMenu();

// function that handle option selection
void handleInputPauseMenu(int mouse_x, int mouse_y, int key);  


#endif 