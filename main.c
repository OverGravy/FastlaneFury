#include <pthread.h>
#include "Game.h"
#include "List.h"
#include "Ptask.h"
#include "Graphics.h"
#include "User.h"
#include "time.h"
#include <math.h>

// declare things
pthread_t tid[MAX_TASKS];
struct task_par tp[MAX_TASKS];
struct timespec ptask_t0;
int ptaskPolicy;
int ptaskActivated;
int freeIndex[MAX_TASKS];

// declare allegro things
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_EVENT event;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_BITMAP *veicleBtm[MAX_VEICLE_TYPE];
ALLEGRO_FONT *fonts[MAX_FONT];
ALLEGRO_BITMAP *zoomdScreen = NULL;
int ZoomedId = -1;

int main(){

    srand(time(NULL));

    // init allegro
    if(!initAllegro()){
        fprintf(stderr, "ERROR: failed to initialize allegro!\n");
        return -1;
    }

    // load graphics assets
    if(!loadGraphicsAssets()){
        fprintf(stderr, "ERROR: failed to load graphics assets!\n");
        return -1;
    }

    // init ptask
    ptask_init(SCHED_FIFO); // init ptask with a SCHED policy

    // create a shared list of veicles
    struct SharedList* shared = createSharedList();
    
    if(shared == NULL){
        fprintf(stderr, "ERROR:  creating shared list\n");
        return -1;
    }

    // init mutex
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    // create and start User task
    struct argument userTaskArg;
    userTaskArg.mutex = &mutex;
    userTaskArg.shared = shared;
    userTaskArg.screenH = SCREEN_H;
    userTaskArg.screenW = SCREEN_W;

    if(task_create(userTask, 0, userTaskArg, 15 , 15 , 90, ACT) != 0){
        printf("ERROR: can not create user task\n");
        return -1;
    }


    // create and start Graphics task
    struct argument GraphicsArg;
    GraphicsArg.mutex = &mutex;
    GraphicsArg.shared = shared;
    GraphicsArg.screenH = SCREEN_H;
    GraphicsArg.screenW = SCREEN_W;


    if(task_create(graphicsTask, 1, GraphicsArg, (int)ceil((double)1000/SCREEN_FPS) , (int)ceil((double)1000/SCREEN_FPS) , 80, ACT) != 0){
        printf("ERROR: can not create graphics task\n");
        return -1;
    }


    // wait for graphics task to finish
    wait_for_task_end(0);
    closeAllegro();
    ptask_exit();

    return 0;
}