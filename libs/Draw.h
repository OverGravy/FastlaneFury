#ifndef __DRAW_H__
#define __DRAW_H__

#include <pthread.h>
#include "List.h"
#include "Support_list.h"
#include "Draw_primitives.h"
#include "Game_core.h"
#include "Ptask.h"
#include "User.h"


// Graphics variables
extern BITMAP* buffer;                                                              // display buffer bitmap
extern BITMAP* background;                                                          // background bitmap
extern BITMAP* Veicles[CAR_NUMBER+TRUCK_NUMBER+MOTORCYCLE_NUMBER+SUPERCAR_NUMBER] ; // array of veicles bitmaps

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
void draw_info(pthread_mutex_t *mutex, struct Shared_List *shared);

// function that draws veicle
void draw_veicle(double x, double y, int type);

// function that draws mouse
void draw_mouse(int x, int y);

// function that draws background in buffer
void prerender_background();

// function that draws pause 
void draw_pause_symbol();

// function that returns the veicle bitmap height
int get_veicle_height(int type);

// function that returns the veicle bitmap width
int get_veicle_width(int type);

// function that clear the zoom screen
void clear_zoom_screen();

// function that draws that flip the zoom screen
void flip_zoom_screen();

//// !!!!!!!!!!! NOT USED !!!!!!!!!!!!!!


// function that returns the pointer to the buffer
BITMAP* get_buffer();

// function that returns the pointer to the background
BITMAP* get_background();

#endif