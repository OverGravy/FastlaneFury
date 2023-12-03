#ifndef __SENSOR_H__
#define __SENSOR_H__

#include <allegro.h>
#include <math.h>
#include "Game_core.h"

// function that returns distance from other veicle
double proximity_sensor(double x, double y, int range, double alpha);

#endif