#ifndef _DRIVE_
#define _DRIVE_

#include "Sensor.h"
#include "Stat_file.h"
#include "Game_core.h"
#include "List.h"
#include "Assets.h"
#include "Ptask.h"

#define DETECTION_DEGREE 160 // degree arch in which the veicle can detect other veicles, left, right

struct Distances {
    int front;                                       // distance from viecle in the front
    int left;                                        // distance from viecle in the left
    int back;                                        // distance from viecle in the back                                   
    int right;                                       // distance from viecle in the right
};


// function that initialize veicle
void init_veicle_state(struct Veicle_State *state, struct Veicle_Statistics *statistics, struct argument_struct argument, int index);

// function that handle driving movement
void driving_handling(struct Veicle_State *State, struct Veicle_Statistics *Statistics, struct Distances *distances);

// function tha handle sensor measurements
void do_mesurements(struct Veicle_State *State, double measurement[], struct Distances *distances, BITMAP *scene_buffer);

#endif