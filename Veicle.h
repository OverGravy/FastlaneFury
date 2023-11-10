#ifndef __VEICLE__
#define __VEICLE__

#include "Game.h"
#include "List.h"
#include "SupportList.h"
#include "Ptask.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define DETECTION_DEGREE 160 // degree arch in which the veicle can detect other veicles, left, right

void *veicleTask(void *arg);

#endif