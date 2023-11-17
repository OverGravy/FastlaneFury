#ifndef __SUPPORT_LIST_H__
#define __SUPPORT_LIST_H__

#include "List.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct supportList{
    int id;
    int state;
    double acceleration;
    double speed;
    struct supportList *next;
};

extern struct supportList *support;  
extern pthread_mutex_t supportMutex;

// function that create the list and return the pointer to the list
struct supportList* createSupportList();

// function that insert info in the support list
void addVecileInfoToSupport(struct VeicleState *veicle, int index);

// function that return a specific node in the support list
struct supportList* getSupportNode(int index);

// function that free all the element in the support list
void cleanSupportList();

// function that destroy the list
void destroySupportList();

#endif