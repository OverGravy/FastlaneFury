/*  TYPE OF VEICLE

    1) Car
    2) Truck
    3) Motorcycle
    
*/

#ifndef FILE_H
#define FILE_H

#include <stdlib.h>
#include <stdio.h>

#define CAR 0
#define TRUCK 1
#define MOTORCYCLE 2
#define SUPERCAR 3


// Veicle Statistics struct
struct VeicleStatistics{
    double maxSpeed;        // max speed in m/s
    double maxAcceleration; // max acceleration in m/s^2
    double maxDeceleration; // max deceleration in m/s^2
    double minDistance;     // min distance in m that the veicle can be from another veicle
};

struct DataFile{
    int rowNumber;
    FILE *fp;
};

// File variables
extern struct DataFile CarFile;
extern struct DataFile TruckFile;
extern struct DataFile MotorcycleFile;
extern struct DataFile SupercarFile;

// function that open the file and return the file pointer
int CheckStatisticFile();

// function that read line and retun car Statistic struct pointer
void GetVeicleStaitistics(int type, struct VeicleStatistics *Statistics);

// function that close the file
void CloseStatisticFile();

#endif