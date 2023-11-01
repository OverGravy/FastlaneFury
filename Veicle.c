#include "Veicle.h"
// function task veicle
void *veicleTask(void *arg)
{   
    int running = 1;
    int veicleWidth = 0;
    int veicleHeight = 0;
    struct argument veicleArg = get_task_argument(arg);
    int ti = get_task_index(arg);
    wait_for_activation(ti);

    // initialize veicle State
    struct VeicleState State;
    initVeicleState(&State);

    veicleWidth = getVeicleWidth(State.veicle);

    // initialize veicle variables
    double DeltaPosition = 0;             // delta position in m
    double DeltaSpeed = 0;                // delta speed in m/s
    double distance = 0;                  // distance from other veicle in m

    double dmax = 3.0;                    // max distance from other veicle in m
    double SpeedMax = 20.0;               // max speed in m/s
    double amin = -5.0;                   // min acceleration in m/s^2
    double amax = 10.0;                   // max acceleration in m/s^2

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

        // check for speed limit
        if (State.speed < 0)
        {
            State.speed = 0;
        }

        if (State.speed > SpeedMax)
        {
            State.speed = SpeedMax;
        }

        // CHECK OTHER VEICLE

        // check for veicle in front
        distance = proximitySensor(State.veicle, State.pos.x, State.pos.y, SMAX);
        if(distance != -1){
            // if there is a veicle in front i need to ajust acceleration
            if(distance < dmax){
                State.acceleration = distance - dmax;
                if (State.acceleration < amin)
                {
                    State.acceleration = amin;
                }
            }

        }else{
            State.acceleration = amax;
        }

      
        // UPDATE VEICLE STATE

        // set veicle state in the list 
        setVeicleState(veicleArg.shared, veicleArg.mutex, ti, State);


        // check if veicle is out of screen
        if (State.pos.x < -(veicleWidth/SCALE_FACTOR))
        {
            // remove veicle from list
            removeVeicleFromList(veicleArg.shared, veicleArg.mutex, ti);
            running = 0;
        }


        // check for miss deadline
        if (deadline_miss(ti))
        {
            printf("VEICLE: deadline missed\n");
        }
        wait_for_period(ti);

    }
    printf("OK: Veicle task terminated id: %d\n", ti);
    task_deactivate(ti);
    return NULL;

}