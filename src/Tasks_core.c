#include "../libs/Tasks_core.h"

// function that init shared structure
struct argument_struct *init_argument_struct(){

    // structure declaration 
    struct argument_struct *a_struct = (struct argument_struct*)malloc(sizeof(struct argument_struct));
    struct shared_struct *shared_struct = NULL;
    struct Config *config_struct = NULL;

    // init shared list and his mutex
    a_struct->shared_list = create_shared_list();
    pthread_mutex_t *shared_list_mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(shared_list_mutex, NULL);
    a_struct->shared_list_mutex = shared_list_mutex;

    // init support list and his mutex
    a_struct->support_list = create_support_list();
    pthread_mutex_t *support_list_mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(support_list_mutex, NULL);
    a_struct->support_list_mutex = support_list_mutex;

    //init the scene BITMAP 
    a_struct->scene = create_bitmap(SCREEN_W, SCREEN_H);
    pthread_mutex_t *scene_mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(scene_mutex, NULL);
    a_struct->scene_mutex = scene_mutex;

    // init shared struct mutex
    pthread_mutex_t *shared_struct_mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(shared_struct_mutex, NULL);
    a_struct->shared_struct_mutex = shared_struct_mutex;

    // init shared struct
    shared_struct = (struct shared_struct*)malloc(sizeof(struct shared_struct));
    a_struct->shared_struct = shared_struct;

    a_struct->shared_struct->game_state = PLAY;
    a_struct->shared_struct->selection = NONE;
    a_struct->shared_struct->selected_veicle = NONE;
    a_struct->shared_struct->buffer_id = MAIN_SCENE;
    a_struct->shared_struct->mouse_pos.x = 0;
    a_struct->shared_struct->mouse_pos.y = 0;
   
    // init config struct
    config_struct = (struct Config*)malloc(sizeof(struct Config));
    a_struct->config_struct = config_struct;
    a_struct->config_struct->auto_spawn = AUTO;
    a_struct->config_struct->auto_spawn_time = AS_T1;
    a_struct->config_struct->zv_scale_factor = Z1_FACTOR;

    // init the veicle type array
    a_struct->VeicleType = (int*)malloc(sizeof(int) * MAX_TASKS);

    // set the first 2 in the array as NONE cause the first 2 veicle are non existent
    // i chose to keep the same index of the task for this array to avoid confusion
    // the first 2 veicle are non existent cause they are used for the user and graphic task
    for (int i = 0; i < 2; i++)
    {
        a_struct->VeicleType[i] = NONE;
    }

    // init the last lane used
    a_struct->LastLane = (int*)malloc(sizeof(int));

    // set last lane as NONE
   *(a_struct->LastLane) = NONE;

    // set the simulation speed to 1
    a_struct->SimSpeed = (float*)malloc(sizeof(float));
    *(a_struct->SimSpeed) = 1;

    // return the structure
    return a_struct;
}

// funtion that create the user task
int create_user_task(struct argument_struct argument){
    // create the user task
    return task_create(user_task, 0, argument, UT_PERIOD, UT_DEADLINE, UT_PRIORITY, ACT);
}

// function that create the Graphic task
int create_graphic_task(struct argument_struct argument){
    // create the graphic task
    return task_create(graphics_task, 1, argument, GT_PERIOD, GT_DEADLINE, GT_PRIORITY, ACT);
}

// function that create the veicle task
int create_veicle_task(struct argument_struct argument, int index, int predefined){

    // if the veicle is predefined set the veicle type else set a random veicle type
    if (predefined != NONE)
    {
        // set the predefined veicle
        argument.VeicleType[index] = predefined;
    }
    else
    {
        // set the random veicle
        argument.VeicleType[index] = rand() % 3;
    }

    // create the veicle task
    return task_create(veicle_task, index, argument, VT_PERIOD, VT_DEADLINE, VT_PRIORITY, ACT);
}