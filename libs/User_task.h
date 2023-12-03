#ifndef __USER_H__
#define __USER_H__

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <allegro.h>
#include "Config_menu.h"
#include "Configuration.h"
#include "Ptask.h"
#include "Game_core.h"
#include "Veicle_task.h"
#include "Pause.h"

void *user_task(void *arg);


#endif 