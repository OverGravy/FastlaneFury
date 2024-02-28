#ifndef __TASK_CORE_H__
#define __TASK_CORE_H__

// this file .h use ptask to create tasks needed for Fastlane Fury

#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>
#include <pthread.h>
#include "Game_core.h"
#include "List.h"
#include "Ptask.h"
#include "User_task.h"
#include "Graphics_task.h"
#include "Veicle_task.h"
#include "Support_list.h"


// function that init the shared struct
struct argument_struct *init_argument_struct();

// function that create User_task using Ptask
int create_user_task(struct argument_struct argument);

// function that create Graphic_task using Ptask
int create_graphic_task(struct argument_struct argument);

// function that create Veicle_task using Ptask
int create_veicle_task(struct argument_struct argument, int index, int predefined);


#endif