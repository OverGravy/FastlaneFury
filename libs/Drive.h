#ifndef _DRIVE_
#define _DRIVE_

#include "Game.h"

#define DETECTION_DEGREE 160 // degree arch in which the veicle can detect other veicles, left, right

struct Distances {
    int front;                                       // distance from viecle in the front
    int left;                                        // distance from viecle in the left
    int back;                                        // distance from viecle in the back                                   
    int right;                                       // distance from viecle in the right
};

// function that handle driving movement
void DrivingHandling(struct VeicleState *State, struct VeicleStatistics *Statistics, struct Distances *distances);

// function tha handle sensor measurements
void DoMesurements(struct VeicleState *State, double measurement[], struct Distances *distances);

#endif