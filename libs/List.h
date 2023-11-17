#ifndef __LIST_H__
#define __LIST_H__

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// structure to represent a position
struct Position{
    double x;               // position in meter
    double y;               // position in meter
};

// Define a structure to represent a veicle state
struct VeicleState{
    double speed;           // speed of the car in m/s
    double steeringAngle;   // steering angle of the car in degree
    double acceleration;    // acceleration of the car in m/s^2
    int state;              // state of the car
    struct Position pos;    // position in the highway in m
    int veicle;             // type of the car
    int lane;               // lane the car is in
};

// Node structure for the list
struct Node{
    int id;                    // thread id
    struct VeicleState Veicle; // veicle state
    struct Node* next;         // pointer to the next node
};

// Define a structure to represent a list
struct SharedList{
   struct Node* head;
   int size;
};


extern struct SharedList *shared;
extern pthread_mutex_t mutex;

// function that create the list and return the pointer to the liststruct SharedList* createSharedList();                                                   // create a shared list
struct SharedList* createSharedList();                                                   

// function that insert a veicle in the list 
void addVeicleToList(int id, struct VeicleState veicle);  

// function that remove a veicle from the list
void removeVeicleFromList(int id);      

// function that set veicle state in the list 
void setVeicleState(int id, struct VeicleState State);  

// function that return list size
int getListSize();

// function that destroy the list
void destroySharedList();




#endif