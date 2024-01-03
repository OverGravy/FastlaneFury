#ifndef __ASSETS_H__
#define __ASSETS_H__

// This file .h contains all the assets of the game

#include <allegro.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Game_core.h"

extern BITMAP* Veicles[CAR_NUMBER+TRUCK_NUMBER+MOTORCYCLE_NUMBER+SUPERCAR_NUMBER] ; // array of veicles bitmaps

// load function that takes references of bitmap and font array
int load_graphics_assets();

// function that returns the veicle bitmap pointer
BITMAP* get_veicle_bitmap(int type);

// function that returns the veicle bitmap height
int get_veicle_height(int type);

// function that returns the veicle bitmap width
int get_veicle_width(int type);


#endif // __ASSETS_H__