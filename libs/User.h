#ifndef USER_H
#define USER_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "Assets.h"
#include "Allegro_core.h"
#include "Game_core.h"
#include "List.h"
#include "Ptask.h"


// USER TASK FUNCTIONS

// function that return the selection on the screen and set automatically the id of the selected type of thing
int set_selection(int x, int y, struct Shared_List *shared, pthread_mutex_t *shared_struct_mutex, struct shared_struct *shared_struct);

#endif