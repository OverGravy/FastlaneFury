#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

// setting structures
struct Config
{
    int autoSpawn;     // auto spawn veicle
    int autoSpawnTime; // auto spawn time in second
};

extern struct Config configuration;            // setting structure

// function that init to default configuration
void initConfiguration();


// SETTER

// function that set autospawn 
void setAutoSpawn(int value);

// function that set autospawn time
void setAutoSpawnTime(int value);


// GETTER

// function that get autospawn
int getAutoSpawn();

// function that get autosspawntime
int getAutoSpawnTime();


#endif