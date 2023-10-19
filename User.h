#ifndef __USER_H__
#define __USER_H__

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <allegro5/allegro.h>
#include "List.h"
#include "Ptask.h"
#include "Game.h"
#include "Veicle.h"

// struct that contains all the arguments needed by the user task
struct UserTaskArg{
    ALLEGRO_DISPLAY **display;
    ALLEGRO_EVENT_QUEUE **event_queue;
    ALLEGRO_TIMER **timer;
    struct SharedList *shared;         
};


// veicle argument to create a new vieicle
struct VeicleArgument{
    struct VeicleState veicle;
    struct SharedList *list;         // pointer to the list
    pthread_mutex_t *mutex;          // pointer to the mutex
};

void *userTask(void *arg);


#endif 