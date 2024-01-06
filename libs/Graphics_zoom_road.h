#ifndef __LIBS_GRAPHICS_ZOOM_ROAD_H__
#define __LIBS_GRAPHICS_ZOOM_ROAD_H__

#include "stdio.h"
#include "stdlib.h"
#include <allegro.h>
#include "List.h"
#include "Ptask.h"
#include "math.h"
#include "Support_list.h"

// function that render the zoom road peace on screen_buffer 
void render_zoom_road(BITMAP *dest_buffer, BITMAP *scene_surce, struct position *pos, struct Config *config);

// function that write some information under the screen
void render_info_zoom_road(struct Shared_List * shared , BITMAP *dest_buffer);

#endif