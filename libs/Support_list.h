#ifndef __SUPPORT_LIST_H__
#define __SUPPORT_LIST_H__

#include "List.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// struct that rappresent a node in the support list
struct Support_List{
    int id;
    int state;
    double acceleration;
    double speed;
    struct Support_List *next;
};


// function that create the list and return the pointer to the list
struct Support_List* create_support_list();

// function that insert info in the support list
void add_vecile_info_to_support(struct Support_List *support, pthread_mutex_t *supportMutex, struct Veicle_State *veicle, int index);

// function that return a specific node in the support list
struct Support_List get_support_node(struct Support_List *support, pthread_mutex_t *support_mutex, int index);

// function that free all the element in the support list
void clean_support_list(struct Support_List *support, pthread_mutex_t *supportMutex);

// function that destroy the list
void destroy_support_list(struct Support_List *support);

#endif