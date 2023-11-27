#include "../libs/Configuration.h"

// function that init configuration
void initConfiguration()
{
    configuration.autoSpawn = 0;
    configuration.autoSpawnTime = 5;
}

// Setter

// function that set autospawn
void setAutoSpawn(int value)
{
    configuration.autoSpawn = !configuration.autoSpawn;
}

// function that set autospawn time
void setAutoSpawnTime(int value)
{
    configuration.autoSpawnTime = value;
}

// Getter

// function that get autospawns
int getAutoSpawn()
{
    return configuration.autoSpawn;
}

// function that get autospawn time
int getAutoSpawnTime()
{
    return configuration.autoSpawnTime;
}