#ifndef __PAUSE_H__
#define __PAUSE_H__

#include <pthread.h>
#include "Game_core.h"
#include "List.h"
#include "Menu.h"
#include "Support_list.h"

extern int paused[MAX_TASKS];             // array of pause

// function that pause all the veicles
void pause_veicles(pthread_mutex_t *mutex, struct Shared_List *shared);

// function that pause veicle and open the menu 
void pause_for_menu(pthread_mutex_t *mutex, struct Shared_List *shared);

// function that resume all the veicles
void resume_veicles(pthread_mutex_t *mutex, struct Shared_List *shared);

// function that resume veicle and close the menu
void resume_from_menu(pthread_mutex_t *mutex, struct Shared_List *shared);

// function that cheks if a veicle is paused
int check_pause();

// function that cheks if the menu is open
int check_menu();

#endif