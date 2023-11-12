#include <pthread.h>
#include "Game.h"
#include <allegro.h>
#include <stdio.h>
#include <stdlib.h>
#include "List.h"
#include "Ptask.h"
#include "Graphics.h"
#include "User.h"
#include "time.h"
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
BITMAP *Veicles[MAX_VEICLE_TYPE]; // array of veicles bitmaps
BITMAP *otherBmp[MAX_OTHER_BPM]; // array of other bitmaps
FONT *fonts[MAX_FONT];            // array of fonts

// Shared variable things
struct SharedList *shared;        // shared list
pthread_mutex_t mutex;            // mutex for shared list 
pthread_mutex_t supportMutex;     // suport mutex
struct supportList *support;      // support list
int paused[MAX_TASKS];            // array of pause

// User task things
int selectedVeicle = -1;          // selected veicle
int selectedButton = -1;          // selected button

// statistic things
FILE* CarFp, *TruckFp, *MotorcycleFp; // file pointer

int main()
{
    srand(time(NULL));

    // init allegro
    if (!initAllegro())
    {
        fprintf(stderr, "ERROR: failed to initialize allegro!\n");
        return -1;
    }

    // load graphics assets
    if (!loadGraphicsAssets())
    {
        fprintf(stderr, "ERROR: failed to load graphics assets!\n");
        return -1;
    }

    // check statistic file
    if (!CheckStatisticFile())
    {
        fprintf(stderr, "ERROR: failed to load statistic file!\n");
        return -1;
    }

    // init ptask
    ptask_init(SCHED_FIFO); // init ptask with a SCHED policy

    // create a shared list of veicles and support list
    shared = createSharedList();
    support = createSupportList();

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

    // create and start User task
    struct argument userTaskArg;
    userTaskArg.mutex = &mutex;
    userTaskArg.shared = shared;
    userTaskArg.screenH = SCREEN_H;
    userTaskArg.screenW = SCREEN_W;

    if (task_create(userTask, 0, userTaskArg, 15, 15, 90, ACT) != 0)
    {
        printf("ERROR: can not create user task\n");
        return -1;
    }

    // create and start Graphics task
    struct argument GraphicsArg;
    GraphicsArg.mutex = &mutex;
    GraphicsArg.shared = shared;
    GraphicsArg.screenH = SCREEN_H;
    GraphicsArg.screenW = SCREEN_W;

    if (task_create(graphicsTask, 1, GraphicsArg, (int)ceil((double)1000 / SCREEN_FPS), (int)ceil((double)1000 / SCREEN_FPS), 80, ACT) != 0)
    {
        printf("ERROR: can not create graphics task\n");
        return -1;
    }

    // Clean up and exit
    wait_for_task_end(0);  // wait for user task to end
    return 0;
}
