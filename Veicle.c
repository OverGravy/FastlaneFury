#include "Veicle.h"
// function task veicle
void *veicleTask(void *arg)
{   
    int running = 1;
    struct argument veicleArg = get_task_argument(arg);
    int ti = get_task_index(arg);
    wait_for_activation(ti);

    // initialize veicle State
    struct VeicleState State;
    initVeicleState(&State);

    // initialize veicle variables
    double DeltaPosition = 0;             // delta position in m
    double DeltaSpeed = 0;                // delta speed in m/s
    int distance = 0;                     // distance from other veicle in m

    // add veicle to list
    addVeicleToList(veicleArg.shared, veicleArg.mutex, ti, State);

    printf("OK: Veicle task activated id: %d\n", ti);

    while (running)
    {
        // UPDATE VEICLE STATE

        // calculate veiele speed and position using MKS unit
        DeltaSpeed = State.acceleration * ((double)task_get_period(ti)/1000.0);     // delta speed in m/s0
        State.speed += DeltaSpeed;                                                  // speed in m/s
        DeltaPosition = State.speed*((double)task_get_period(ti)/1000.0);           // delta position in m
        State.pos.x -= DeltaPosition;                                               // position in m

       
        // check other veicle
       // distance = ProximitySensor(&State.pos, 300); // distance in meter

      
        // set veicle position
        setVeicleState(veicleArg.shared, veicleArg.mutex, ti, State);


        // check if veicle is out of screen
        if (State.pos.x < -(al_get_bitmap_width(getVeicleBitmap(State.veicle))/SCALE_FACTOR))
        {
            // remove veicle from list
            removeVeicleFromList(veicleArg.shared, veicleArg.mutex, ti);
            running = 0;
        }

        // check for miss deadline
        if (deadline_miss(ti))
        {
            
        }
        wait_for_period(ti);

    }
    printf("OK: Veicle task terminated id: %d\n", ti);
    task_deactivate(ti);
    return NULL;

}