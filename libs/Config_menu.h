#ifndef _PAUSEMENU_H_
#define _PAUSEMENU_H_

#include "Menu.h"
#include "Game_core.h"
#include "Configuration.h"
#include "Draw.h"

extern struct Option options[OPTION_NUM];   // array of options
extern struct Menu_Config pause_menu_config;   // pause menu config

// function that init the pause menu
void init_config_menu();

// function that draws the pause menu
void draw_config_menu();

// function that handle option selection
void handle_input_config_menu(int mouse_x, int mouse_y);  


#endif 