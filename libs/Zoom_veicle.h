#ifndef __ZOOM_VEICLE_H__
#define __ZOOM_VEICLE_H__

#include "Draw.h"
#include "Veicle.h"
#include "Game_core.h"
#include "List.h"
#include "User.h"

extern BITMAP *zoom_buffer;
extern BITMAP *zoom_veicle;

// function that clear the zoom buffer
void clear_zoom_buffer();

// function that filp the zoom buffer
void flip_zoom_buffer();

//function that draw the zoom screen buffer the zoomd veicle
void draw_zoom(int veicle);

#endif