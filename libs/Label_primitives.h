#ifndef __Label_primitives_h__
#define __Label_primitives_h__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <allegro.h>
#include "Game_core.h"

// function that render a lable considering a given position and a given text and given dest_buffer
void render_label(int x, int y, char *text, BITMAP *dest_buffer);


#endif