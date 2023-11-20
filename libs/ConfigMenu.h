#ifndef _PAUSEMENU_H_
#define _PAUSEMENU_H_

#include "Menu.h"
#include "Game.h"

#define OPTION_NUM 3

extern struct option options[OPTION_NUM];   // array of options
extern struct MenuConfig pauseMenuConfig;   // pause menu config

// function that init the pause menu
void initConfigMenu();

// function that draws the pause menu
void DrawConfigMenu();

// function that handle option selection
void handleInputConfigMenu(int mouse_x, int mouse_y);  


#endif 