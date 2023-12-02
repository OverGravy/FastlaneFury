#ifndef __DRAW_H__
#define __DRAW_H__

#include <pthread.h>
#include "Draw_primitives.h"
#include "Game.h"

// DRAWING FUNCTIONS

// load function that takes references of bitmap and font array
int load_graphics_assets();

// function that clear the display
void clear_display();

// function that flip the display
void flip_display();

// function that draws the backgroung
void draw_background();

// fucntion that draws info
void draw_info(pthread_mutex_t *mutex, struct SharedList *shared);

// function that draws veicle
void draw_veicle(double x, double y, int type);

// function that draws mouse
void draw_mouse(int x, int y);

// function that draws background in buffer
void prerender_background();

// function that draws pause 
void draw_pause_symbol();

#endif