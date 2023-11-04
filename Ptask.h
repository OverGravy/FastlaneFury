#ifndef __PTASK__
#define __PTASK__
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <signal.h>

#define MAX_TASKS 200
#define MICRO 0
#define MILLI 1
#define ACT 1

struct argument
{
    int screenW;
    int screenH;
    struct SharedList *shared;
    pthread_mutex_t *mutex;
};

struct task_par
{
    int index;                // task id
    struct argument arg;      // task argument
    long wcet;                // task WCET in us
    int period;               // task period in ms
    int deadline;             // relative deadline in ms
    int priority;             // task priority in [0,99]
    int paused;               // task paused
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


// TIME MENAGMENT FUNCTION

// function that copy time from one struct to another
void time_copy(struct timespec *td, struct timespec ts);

// function that compares two times
int time_cmp(struct timespec td, struct timespec ts);

// function that adds time to a struct
void time_add_ms(struct timespec *t, int ms);

// funtion that gets the current time
long get_sys_time(int unit);


// TASK FUNCTION

// function that init ptask
void ptask_init(int policy);

// funxtion that handle the exit from ptask
void ptask_exit();

// function that creates a task
int task_create(void *(*task)(void *), int i, struct argument input, int period, int drel, int prio, int aflag);

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

// function that wait fo a period
void wait_for_period(int i);

// function that wait for a specific task to terminate
void wait_for_task_end(int i);

// function that pause all task
void pause_all_task();

// function that resume all task
void resume_all_task();

// GETTER

// function that retrive a free id
int get_free_index();

// function that return task id
int get_task_index(void *arg);

// function that get task argument
struct argument get_task_argument(void *arg);

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


// PAUSE AND RESUME

// function that set single task pause
void task_set_pause(int i);

// function that pause all the thread
void task_set_all_pause();

// function that resume a single task
void task_set_resume(int i);

// function that resume all the thread
void task_set_all_resume();

// function that check if a task is paused and return 1 if it is
int task_is_paused(int i);

// function that wait for a task to be resumed
void wait_for_resume(int i);


#endif