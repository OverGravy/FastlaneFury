#ifndef USER_H
#define USER_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "Allegro_core.h"
#include "Game_core.h"
#include "List.h"
#include "Draw.h"


// User variables
extern int selected_veicle;                // index of selected veicle
extern int zoomed_veicle;                  // index of zoomed veicle
extern int selected_button;                // index of selected button

// USER TASK FUNCTIONS

// function that return the selection on the screen and set automatically the id of the selected type of thing
int set_selection(int x, int y, pthread_mutex_t *mutex, struct Shared_List *shared);

// function that returns the index of the selected veicle
int get_selected_veicle();

// function that set the index of the selected veicle
void set_selected_veicle(int id);

// function that set the index of the zoomed veicle
void set_zoomed_veicle(int id);

// function that get the index of the zoomed veicle
int get_zoomed_veicle();

// function that set the index of the selected button
void setSelectedButton(int id);                        // !!!!!!!!!!!! NOT USED !!!!!!!!!!!!!!

// function that return the index of the selected button
int get_selected_button();

#endif