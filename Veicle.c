#include "Veicle.h"

// function that handle Veicle driving logic
void DrivingHandling(struct VeicleState *State, struct VeicleStatistics *Statistics)
{
    int middleLane = 0;
    double distance = proximitySensor(State->veicle, State->pos.x, State->pos.y, 15, 0); // distance in m

    // STATE HANDLING
    switch (State->state)
    {
    case NORMAL: // normal driving
        State->acceleration += 0.5;
        if (State->acceleration > Statistics->maxAcceleration)
        {
            State->acceleration = Statistics->maxAcceleration;
        }
        break;
    case SLOWDOWN: // slowing down
        State->acceleration = distance - Statistics->minDistance;
        if (State->acceleration < Statistics->maxDeceleration)
        {
            State->acceleration = Statistics->maxDeceleration;
        }
        break;
    case OVERTAKE: // overtaking
        State->acceleration = Statistics->maxAcceleration;
        State->steeringAngle += 0.5;
        if (State->steeringAngle > 45)
        {
            State->steeringAngle = 45;
        }
        middleLane = ((MY_SCREEN_H / (LANE_NUMBER + 1)) * (State->lane))/2;
        printf("Y: %f\n", (State->pos.y)*SCALE_FACTOR);

        if (((State->pos.y)*SCALE_FACTOR) > middleLane){
            State->lane += 1;
            printf("LANE: %d\n", State->lane);
            State->steeringAngle = 0;
        }

        break;
    case CRASH: // crash
        printf("CRASH\n");
        break;
    }

    // STATE CHANGING

    // check for veicle in front
    if (distance != -1)
    {
        // switch to slowing down
        if (distance < Statistics->minDistance)
        {
            State->state = 1;
        }

        // switch to overtaking

        // the condition is that the distance from the veicle in front is between 0.5 and 1.5 meters and i am not going too fast
        if (distance > 0.5 && distance < Statistics->minDistance && State->speed < 10) 
        {
            State->state = 2;
        }


        // switch to crash
        if (distance < 0.2)
        {
            State->state = 3;
        }
    }else if (distance == -1 && State->state != OVERTAKE){
        State->state = 0;
    }
}

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
    struct VeicleStatistics Statistics;
    initVeicleState(&State, &Statistics, veicleArg.shared->size);

    veicleWidth = getVeicleWidth(State.veicle);

    // initialize veicle variables
    double DeltaPositionX = 0; // delta position in m
    double DeltaPositionY = 0; // delta position in m
    double DeltaSpeed = 0;     // delta speed in m/s
    double distance = 0;       // distance from other veicle in m

    // add veicle to list
    addVeicleToList(veicleArg.shared, veicleArg.mutex, ti, State);

    printf("OK: Veicle task activated id: %d\n", ti);

    while (running)
    {
        // UPDATE VEICLE STATE

        // calculate veiele speed and position using MKS unit
        DeltaSpeed = State.acceleration * ((double)task_get_period(ti) / 1000.0); // delta speed in m/s0
        State.speed += DeltaSpeed;                                                // speed in m/s
        DeltaPositionX = State.speed * ((double)task_get_period(ti) / 1000.0);    // delta position in m
        State.pos.x -= DeltaPositionX;                                            // position in m

        // change in function of sterin angle
        DeltaPositionY = DeltaPositionX * tan(State.steeringAngle * (M_PI / 180.0)); // delta position in m
        State.pos.y -= DeltaPositionY;                                               // position in m

        // check for speed limit
        if (State.speed < 0)
        {
            State.speed = 0;
        }

        if (State.speed > Statistics.maxSpeed)
        {
            State.speed = Statistics.maxSpeed;
        }

        // Handle driving logic

        // check for veicle in front
        DrivingHandling(&State, &Statistics);

        // UPDATE VEICLE STATE

        // set veicle state in the list
        setVeicleState(veicleArg.shared, veicleArg.mutex, ti, State);

        // check if veicle is out of screen
        if (State.pos.x < -(veicleWidth / SCALE_FACTOR))
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