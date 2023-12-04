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
struct Veicle_State{
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
    struct Veicle_State Veicle; // veicle state
    struct Node* next;         // pointer to the next node
};

// Define a structure to represent a list
struct Shared_List{
   struct Node* head;
   int size;
};


extern struct Shared_List *shared;
extern pthread_mutex_t mutex;

// function that create the list and return the pointer to the liststruct SharedList* createSharedList();                                                   // create a shared list
struct Shared_List* create_shared_list();                                                   

// function that insert a veicle in the list 
void add_veicle_to_list(int id, struct Veicle_State veicle);  

// function that remove a veicle from the list
void remove_veicle_from_list(int id);      

// function that set veicle state in the list 
void set_veicle_state(int id, struct Veicle_State State);  

// function that return list size
int get_list_size();

// function that return a specific veicle state
struct Veicle_State get_veicle_state(int id);

// function that destroy the list
void destroy_shared_list();

#endif