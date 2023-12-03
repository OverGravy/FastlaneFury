#ifndef FILE_H
#define FILE_H

#include <stdlib.h>
#include <stdio.h>
#include "Game_core.h"

// Veicle Statistics struct
struct Veicle_Statistics{
    double maxSpeed;        // max speed in m/s
    double maxAcceleration; // max acceleration in m/s^2
    double maxDeceleration; // max deceleration in m/s^2
    double minDistance;     // min distance in m that the veicle can be from another veicle
};

struct Data_File{
    int rowNumber;
    FILE *fp;
};

// File variables
extern struct Data_File CarFile;
extern struct Data_File TruckFile;
extern struct Data_File MotorcycleFile;
extern struct Data_File SupercarFile;

// function that open the file and return the file pointer
int check_statistic_file();

// function that read line and retun car Statistic struct pointer
void get_veicle_staitistics(int type, struct Veicle_Statistics *Statistics);

// function that close the file
void close_statistic_file();

#endif