#ifndef __GRAPHICS_INTERFACE_FUNCTION_H__
#define __GRAPHICS_INTERFACE_FUNCTION_H__

#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>
#include <pthread.h>
#include "Ptask.h"
#include "Assets.h"
#include "List.h"
#include "Support_list.h"
#include "Draw_primitives.h"

// function that render info part of the screen in a destination bitmap
void render_info(struct Shared_List * shared , struct Support_List *support, pthread_mutex_t *list_mutex, pthread_mutex_t *support_mutex, BITMAP *dest_buffer, int selected_veicle);

// function that render the pause button on screen
void render_pause_simbol(BITMAP *dest_buffer);

// function that render the mouse
void render_mouse(BITMAP *dest_buffer);

#endif