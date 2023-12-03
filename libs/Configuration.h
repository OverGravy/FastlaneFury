#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

#include "Game_core.h"

// setting structures
struct Config
{
    int auto_spawn;     // auto spawn veicle
    int auto_spawn_time; // auto spawn time in second
};

extern struct Config configuration;            // setting structure

// function that init to default configuration
int init_configuration();


// SETTER

// function that set autospawn 
void set_auto_spawn(int value);

// function that set autospawn time
void set_auto_spawn_time(int value);


// GETTER

// function that get autospawn
int get_auto_spawn();

// function that get autosspawntime
int get_auto_spawn_time();


#endif