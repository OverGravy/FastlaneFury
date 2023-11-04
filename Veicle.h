#ifndef __VEICLE__
#define __VEICLE__

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "Game.h"
#include "List.h"
#include "Ptask.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NORMAL 0
#define SLOWDOWN 1
#define OVERTAKE 2
#define CRASH 3
#define ABORTOVERTAKE 4



void *veicleTask(void *arg);

#endif