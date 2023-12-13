#include <pthread.h>
#include <allegro.h>
#include <stdio.h>
#include <stdlib.h>
#include "../libs/Graphics_task.h"
#include "../libs/User_task.h"
#include "../libs/Draw.h"
#include "../libs/Allegro_core.h"
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
BITMAP *buffer;                   // display buffer bitmap
BITMAP *background;               // background bitmap
BITMAP *Veicles[CAR_NUMBER+TRUCK_NUMBER+MOTORCYCLE_NUMBER+SUPERCAR_NUMBER]; // array of veicles bitmaps
BITMAP *zoom_viecle_buffer;              // zoom screen bitmaps


// Shared variable things
struct Shared_List *shared;        // shared list
pthread_mutex_t mutex;            // mutex for shared list 
pthread_mutex_t supportMutex;     // suport mutex
struct Support_List *support;      // support list
int paused[MAX_TASKS];            // array of pause

// User task things
int selected_veicle = -1;                   // selected veicle
int selected_button = -1;                   // selected button
int zoom_flag = 0;                          // flag that indicate if the zoom is active
struct Veicle_State selected_veicle_state;  // state of the selected veicle

// statistic things
struct Data_File CarFile;
struct Data_File TruckFile;
struct Data_File MotorcycleFile;
struct Data_File SupercarFile;

// Game Variables
struct Config configuration;            // setting structure

// Pause menu things
struct Option options[OPTION_NUM];  // array of options
struct Menu_Config pause_menu_config;  // pause menu config


int main()
{
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

    // init configuration structure
    if (!init_configuration())
    {
        fprintf(stderr, "ERROR: failed to propertly initialize configuration!\n");
        return -1;
    }


    // init ptask
    ptask_init(SCHED_FIFO); // init ptask with a SCHED policy

    // create a shared list of veicles and support list
    shared = create_shared_list();
    support = create_support_list();

    if (shared == NULL)
    {
        fprintf(stderr, "ERROR:  creating shared list\n");
        return -1;
    }

    if (support == NULL)
    {
        fprintf(stderr, "ERROR:  creating support list\n");
        return -1;
    }

    // init mutex
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&supportMutex, NULL);

    // init menus
    init_config_menu(); 

    // create and start User task
    struct argument userTaskArg;
    userTaskArg.mutex = &mutex;
    userTaskArg.shared = shared;
    userTaskArg.screenH = SCREEN_H;
    userTaskArg.screenW = SCREEN_W;

    if (task_create(user_task, 0, userTaskArg, 15, 15, 90, ACT) != 0)
    {
        printf("ERROR: can not create user task\n");
        return -1;
    }

    // create and start Graphics task
    struct argument GraphicsArg;
    GraphicsArg = userTaskArg;

    if (task_create(graphics_task, 1, GraphicsArg, (int)round((double)1000 / SCREEN_FPS), (int)round((double)1000 / SCREEN_FPS)+1, 80, ACT) != 0)
    {
        printf("ERROR: can not create graphics task\n");
        return -1;
    }

    // Clean up and exit
    wait_for_task_end(0);  // wait for user task to end
    return 0;
}
