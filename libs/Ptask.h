#ifndef __PTASK__
#define __PTASK__
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <signal.h>
#include <allegro.h>
#include "List.h"
#include "Task_time.h"
#include "Game_core.h"

// setting structures
struct Config
{
    int auto_spawn;      // auto spawn veicle
    int auto_spawn_time; // auto spawn time in second
    int zv_scale_factor; // zoom veicle scale factor
};

// strcut that contain the shared variables between tasks
struct shared_struct{
    int game_state;                   // integer that rappresent in witch state the game is
    int selection;                    // integer that rappresent the selection on the screen
    int selected_veicle;              // index of the selected veicle
    int buffer_id;                    // id rappresenting what screen is going to be drawn 
};

// task argument with variable shared between tasks
struct argument_struct
{
    struct Shared_List *shared_list;          // shared list cantaining the veicles position
    pthread_mutex_t *shared_list_mutex;       // mutex needed to access to the shared list
    struct Support_List *support_list;        // support list containing the veicles info
    pthread_mutex_t *support_list_mutex;      // mutex needed to access to the support list
    BITMAP *scene;                            // buffer containing the scene that is rendered by the G_task and accessed by the V_task
    pthread_mutex_t *scene_mutex;             // mutex needed to access to the scene buffer
    pthread_mutex_t *shared_struct_mutex;     // mutex needed to access to the shared struct
    struct shared_struct *shared_struct;      // struct containing the shared variables
    struct Config *config_struct;             // struct containing the configuration parameters
};

// struct that contain all the tasks argument and info
struct task_par
{
    int index;                // task id
    struct argument_struct arg;      // task argument
    long wcet;                // task WCET in us
    int period;               // task period in ms
    int deadline;             // relative deadline in ms
    int priority;             // task priority in [0,99]
    pthread_mutex_t pauseMut; // mutex for pause
    pthread_cond_t pauseCond; // condition variable for pause
    int dmiss;                // # of deadline misses
    struct timespec at;       // next activation time
    struct timespec dl;       // current abs. deadline
    pthread_t tid;            // thread id
    sem_t tsem;               // activation semaphore
};

// global variables of ptask
extern pthread_t tid[MAX_TASKS];
extern struct task_par tp[MAX_TASKS];
extern struct timespec ptask_t0;
extern int ptaskPolicy;
extern int ptaskActivated;
extern int freeIndex[MAX_TASKS];


// TASK FUNCTION

// function that init ptask
void ptask_init(int policy);

// funxtion that handle the exit from ptask
void ptask_exit();

// function that creates a task
int task_create(void *(*task)(void *), int i, struct argument_struct input, int period, int drel, int prio, int aflag);

// function that activate a task
void task_activate(int i);

// function that decrease actived task
void task_deactivate(int i);

// function that whit for activation
void wait_for_activation(int i);

// function that check if a task is active
int task_is_active(int i);

// function that check if deadline were missed
int deadline_miss(int i);

// function that reset the deadline 
void reset_deadline(int i);

// function that wait fo a period
void wait_for_period(int i);

// function that wait for a specific task to terminate
void wait_for_task_end(int i);

// funtion that clean task resources
void task_clean(int i);

// GETTER

// function that retrive a free id
int get_free_index();

// function that return task id
int get_task_index(void *arg);

// function that get task argument
struct argument_struct get_task_argument(void *arg);

// function that get period
int task_get_period(int i);

// function that get deadline
int task_get_deadline(int i);

// function that return number of deadline misses for all active task
int get_deadline_miss();

// function that return priority
int task_get_priority(int i);


// SETTER

// function that set period
void task_set_period(int i, int period);

// function that set a deadline
void task_set_deadline(int i, int deadline);




#endif