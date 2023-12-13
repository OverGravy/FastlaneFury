#ifndef __ZOOM_STREET_H__
#define __ZOOM_STREET_H__

#include "User.h"
#include "Game_core.h"


// zoom buffer work as normal buffer, i use it to place everything on it, and then copy it on screen
extern BITMAP *zoom_street_buffer;  

// funtion that clear the zoom buffer
void clear_zoom_street_buffer();

// function that draw the zoom street screen 
void draw_zoom_street_screen();

// function that flip the zoom street screen
void flip_zoom_street_screen();


#endif