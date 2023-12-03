#include "../libs/Configuration.h"

// function that init configuration
int init_configuration()
{
    configuration.auto_spawn = D_AUTO_SPAWN;
    configuration.auto_spawn_time = D_AUTO_SPAWN_TIME;
    return 1;
}

// Setter

// function that set autospawn
void set_auto_spawn(int value)
{
    configuration.auto_spawn = !configuration.auto_spawn;
}

// function that set autospawn time
void set_auto_spawn_time(int value)
{
    configuration.auto_spawn_time = value;
}

// Getter

// function that get autospawns
int get_auto_spawn()
{
    return configuration.auto_spawn;
}

// function that get autospawn time
int get_auto_spawn_time()
{
    return configuration.auto_spawn_time;
}