#include "../libs/Ptask.h"

// PTASK FUNCTION

// function that init ptask libary and set the scheduling policy
void ptask_init(int policy)
{
    int i;
    ptaskPolicy = policy;
    clock_gettime(CLOCK_MONOTONIC, &ptask_t0);

    // init semaphores
    for (i = 0; i < MAX_TASKS; i++)
    {
        sem_init(&tp[i].tsem, 0, 0);
    }

    ptaskActivated = 0;

    for (i = 0; i < MAX_TASKS; i++)
    {
        pthread_mutex_init(&tp[i].pauseMut, NULL);
        pthread_cond_init(&tp[i].pauseCond, NULL);
    }

    // set all index to 0
    for (i = 0; i < MAX_TASKS; i++)
    {
        freeIndex[i] = 0;
    }
}

// function that return the current time in desired unit
long get_sys_time(int unit)
{
    struct timespec t;
    long tu, mul, div;

    switch (unit)
    {
    case MICRO:
        mul = 1000000;
        div = 1000;
        break;
    case MILLI:
        mul = 1000;
        div = 1000000;
        break;
    default:
        mul = 1000;
        div = 1000000;
        break;
    }

    clock_gettime(CLOCK_MONOTONIC, &t);         // get current time
    tu = (t.tv_sec - ptask_t0.tv_sec) * mul;    // compute the elapsed time
    tu += (t.tv_nsec - ptask_t0.tv_nsec) / div; // in the desired unit
    return tu;
}

// function that create a task
int task_create(void *(*task)(void *), int i, struct argument_struct input, int period, int drel, int prio, int aflag)
{
    pthread_attr_t myatt;     // thread attributes
    struct sched_param mypar; // scheduling parameters for the thread
    int tret;

    freeIndex[i] = 1;     // set the index to 1 to indicate that the index is used

    // set the task parameters<<<
    tp[i].index = i;       // set the task index
    tp[i].period = period; // set the task period
    tp[i].deadline = drel; // set the task deadline
    tp[i].priority = prio; // set the task priority
    tp[i].dmiss = 0;       // set the number of deadline misses to 0
    tp[i].arg = input;     // set the task argument

    pthread_attr_init(&myatt); // initialize the thread attributes

    pthread_attr_setinheritsched(&myatt, PTHREAD_EXPLICIT_SCHED); // set the scheduling parameters
    pthread_attr_setschedpolicy(&myatt, ptaskPolicy);             // set the scheduling policy
    mypar.sched_priority = tp[i].priority;                        // set the priority
    pthread_attr_setschedparam(&myatt, &mypar);                   // set the scheduling parameters
    pthread_attr_setstacksize(&myatt, 1024 * 1024 * 2);           // set the stack size

    tret = pthread_create(&tid[i], &myatt, task, (void *)(&tp[i]));

    if (tret)
    {
        errno = tret; // Imposta errno con il valore restituito da pthread_create
        perror("pthread_create");
    }

    if (aflag == ACT)
        task_activate(i);
    return tret;
}

// function that activate a task
void task_activate(int i)
{
    sem_post(&tp[i].tsem);
    ptaskActivated++;
}

// function that deactivate a task
void task_deactivate(int i)
{
    pthread_cancel(tid[i]);
    ptaskActivated--;
    freeIndex[i] = 0;
}

// function that handle the exit from ptask
void ptask_exit()
{
    int i;

    for (i = 1; i < MAX_TASKS; i++) // clsoe all task from 1 to MAX_TASKS
    {
        sem_destroy(&tp[i].tsem);
        if (freeIndex[i] == 1)
        {
            task_deactivate(i);
        }
    }
}

// function that check if a task is active
int task_is_active(int i)
{
    return freeIndex[i];
}

// function that return the task index
int get_task_index(void *arg)
{
    struct task_par *tpar = (struct task_par *)arg;
    return tpar->index;
}

// function that returns free index
int get_free_index()
{
    int i;
    for (i = 0; i < MAX_TASKS; i++) // check if there is space for the task
    {
        if (freeIndex[i] == 0)
        {
            return i;
        }
    }
    return -1;
}

// funtion that return the task argument
struct argument_struct get_task_argument(void *arg)
{
    struct task_par *tpar = (struct task_par *)arg;
    return tpar->arg;
}

// function that wait for activation
void wait_for_activation(int i)
{
    struct timespec t;
    sem_wait(&tp[i].tsem);
    clock_gettime(CLOCK_MONOTONIC, &t);
    time_copy(&(tp[i].at), t);
    time_copy(&(tp[i].dl), t);
    time_add_ms(&(tp[i].at), tp[i].period);
    time_add_ms(&(tp[i].dl), tp[i].deadline);
}

// function that check if deadline were missed
int deadline_miss(int i)
{
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    if (time_cmp(now, tp[i].dl) > 0)
    {
        tp[i].dmiss++;
        return 1;
    }
    return 0;
}

// function that return number of all deadline misses of all task
int get_deadline_miss()
{
    int i;
    int sum = 0;
    for (i = 0; i < MAX_TASKS; i++)
    {
        if (freeIndex[i] == 1)
        {
            sum += tp[i].dmiss;
        }
    }
    return sum;
}

// function that reset the deadline of a single task
void reset_deadline(int i)
{
    time_copy(&(tp[i].dl), tp[i].at);
    time_add_ms(&(tp[i].dl), tp[i].deadline);
}

// function that wait fo a period
void wait_for_period(int i)
{
    clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &(tp[i].at), NULL);
    time_add_ms(&(tp[i].at), tp[i].period);
    time_add_ms(&(tp[i].dl), tp[i].period);
}

// function that set period
void task_set_period(int i, int period)
{
    tp[i].period = period;
}

// function that set a deadline
void task_set_deadline(int i, int deadline)
{
    tp[i].deadline = deadline;
}

// function that get period
int task_get_period(int i)
{
    return tp[i].period;
}

// function that get deadline
int task_get_deadline(int i)
{
    return tp[i].deadline;
}

// function that get priority
int task_get_priority(int i)
{
    return tp[i].priority;
}

// funtion that wait for the end of a spefic task to terminate
void wait_for_task_end(int i)
{
    pthread_join(tid[i], NULL);
}

// function that clean task resources
void task_clean(int i){
    ptaskActivated--;
    freeIndex[i] = 0;
}