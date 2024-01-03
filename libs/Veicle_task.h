#ifndef __VEICLE__
#define __VEICLE__

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Assets.h"
#include "Tasks_core.h"
#include "Game_core.h"
#include "Veicle.h"
#include "List.h"
#include "User.h"

void *veicle_task(void *arg);

#endif