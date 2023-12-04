#ifndef __ALL_H__
#define __ALL_H__

#include <allegro.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "Draw.h"
#include "Game_core.h"
#include "Zoom_veicle.h"


// ALLEGRO FUNCTIONS    

// funtion that initialize allegro 4.2
int init_allegro();

// close duntion that take reference to everithing needed to close allegro
void close_allegro();


#endif