#ifndef __ZOOM_STREET_H__
#define __ZOOM_STREET_H__

#include "Game_core.h"
#include "Assets.h"
#include "List.h"
#include "Support_list.h"
#include "Ptask.h"
#include <pthread.h>
#include <allegro.h>
#include <stdio.h>

// function that draw the zoomed veicle 
void render_zoom_veicle(BITMAP *dest_buffer, BITMAP *scene_surce, int selected_veicle, struct Shared_List *shared, pthread_mutex_t *list_mutex, struct Config *config);

// function that draw the info about the veicle
void render_info_zoom(struct Shared_List * shared , struct Support_List *support, pthread_mutex_t *list_mutex, pthread_mutex_t *support_mutex, BITMAP *dest_buffer, int selected_veicle);

#endif