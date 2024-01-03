#ifndef __DRAW_H__
#define __DRAW_H__

// this file contains all teh function to sraw the "scene" of the game so update the street and the veicles

#include <pthread.h>
#include "List.h"
#include "Support_list.h"
#include "Draw_primitives.h"
#include "Game_core.h"

// function that clear the scene buffer
void clear_scene_buffer(BITMAP *dest_buffer);

// function that draws the backgroung
void render_background(BITMAP *dest_buffer, BITMAP *bg);

// function that draws veicle
void render_veicle(double x, double y, BITMAP *veicle, BITMAP *dest_buffer);

// function that draws background in buffer
void prerender_background(BITMAP *background);

#endif