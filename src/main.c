#include <pthread.h>
#include <allegro.h>
#include <stdio.h>
#include <stdlib.h>
#include "../libs/Allegro_core.h"
#include "../libs/Game_core.h"
#include "../libs/Tasks_core.h"
#include <time.h>
#include <math.h>

// declare ptask things
pthread_t tid[MAX_TASKS];
struct task_par tp[MAX_TASKS];
struct timespec ptask_t0;
int ptaskPolicy;
int ptaskActivated;
int freeIndex[MAX_TASKS];

// declare allegro things
BITMAP *Veicles[CAR_NUMBER+TRUCK_NUMBER+MOTORCYCLE_NUMBER+SUPERCAR_NUMBER]; // array of veicles bitmaps

// statistic things
struct Data_File CarFile;
struct Data_File TruckFile;
struct Data_File MotorcycleFile;
struct Data_File SupercarFile;

int main()
{

    // variable declaration 
    struct argument_struct *argument = NULL;


    srand(time(NULL));

    // init allegro
    if (!init_allegro())
    {
        fprintf(stderr, "ERROR: failed to initialize allegro!\n");
        return -1;
    }

    // load graphics assets
    if (!load_graphics_assets())
    {
        fprintf(stderr, "ERROR: failed to load graphics assets!\n");
        return -1;
    }

    // check statistic file
    if (!check_statistic_file())
    {
        fprintf(stderr, "ERROR: failed to load statistic file!\n");
        return -1;
    }

    // init ptask
    ptask_init(SCHED_FIFO); // init ptask with a SCHED policy

    // init argument struct
    argument = init_argument_struct();
    
    // create user task to menage I/O
    if (create_user_task(*argument) != 0){
        printf("ERROR: can not create user task\n");
        return -1;
    }

    // create graphics task to menage graphics
    if (create_graphic_task(*argument) != 0)
    {
        printf("ERROR: can not create graphics task\n");
        return -1;
    }

    // Clean up and exit
    wait_for_task_end(0);  // wait for user task to end
    return 0;
}
