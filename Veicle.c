#include "Veicle.h"

// function that handle Veicle driving logic
void DrivingHandling(struct VeicleState *State, struct VeicleStatistics *Statistics, int tIndex)
{

    int middleLane = 0;
    int margin;
    double FrontDistance = proximitySensor(State->veicle, State->pos.x, State->pos.y, 15, 0); // distance in m
    double OtherVeicleDistance = 0;

    struct supportList *temp = NULL;

    // STATE HANDLING ********************************************
    switch (State->state)
    {

    case NORMAL: // normal driving
        State->acceleration += 0.5;
        if (State->acceleration > Statistics->maxAcceleration) // check for max acceleration
        {
            State->acceleration = Statistics->maxAcceleration; // set max acceleration
        }
        break;

    case SLOWDOWN:                                                     // slowing down
        State->acceleration = FrontDistance - Statistics->minDistance; // set decelation in function of distance
        if (State->acceleration < Statistics->maxDeceleration)         // check for max deceleration
        {
            State->acceleration = Statistics->maxDeceleration; // set max deceleration
        }
        break;

    case OVERTAKE:                                         // overtaking
        State->acceleration = Statistics->maxAcceleration; // set max acceleration
        State->steeringAngle -= 0.5;                       // increase gradually steering angle
        if (State->steeringAngle > 45)                     // check for max steering angle
        {
            State->steeringAngle = 45; // set max steering angle
        }

        // calculate middle lane position to stop overtaking
        margin = ((MY_SCREEN_H / (LANE_NUMBER + 1)) - (getVeicleHeight(State->veicle))) / 2; // margin in pixel
        middleLane = (((MY_SCREEN_H / (LANE_NUMBER + 1)) * (State->lane + 1)) + margin);     // in meter

        if (((State->pos.y) * SCALE_FACTOR) >= middleLane)
        { // check if i am in the middle lane
            State->lane += 1;
            printf("Veicle: %d, switch to lane: %d\n", State->veicle, State->lane);
            State->steeringAngle = 0;
            State->state = NORMAL;
        }

        break;

    case ABORTOVERTAKE:               // abort overtake
        State->steeringAngle += 0.5;  // increase gradually steering angle
        if (State->steeringAngle > 0) // check for max steering angle
        {
            State->steeringAngle = 0; // set max steering angle
        }

        margin = ((MY_SCREEN_H / (LANE_NUMBER + 1)) - (getVeicleHeight(State->veicle))) / 2; // margin in pixel
        middleLane = (((MY_SCREEN_H / (LANE_NUMBER + 1)) * (State->lane)) + margin);         // in meter

        if (((State->pos.y) * SCALE_FACTOR) <= middleLane)
        { // check if i am in the middle lane
            printf("Veicle: %d, switch to lane: %d\n", State->veicle, State->lane);
            State->steeringAngle = 0;
            State->state = SLOWDOWN;
        }

        break;

    case CRASH: // crash
        State->speed = 0;
        State->acceleration = 0;
        printf("CRASH\n");
        break;

    case PAUSE:
        State->acceleration = 0;
        State->speed = 0;
        break;
    }

    // STATE CHANGING ********************************************

    // check for veicle in front
    if (FrontDistance != -1 && State->state != PAUSE)
    {
        // switch to crash
        if (FrontDistance == SMIN)
        {
            State->state = CRASH;
        }

        // switch to slowing down
        if (FrontDistance < Statistics->minDistance && State->state != OVERTAKE && State->state != CRASH)
        {
            State->state = SLOWDOWN;
        }

        // switch to overtaking, the condition is that the distance from the veicle in front is between 0.5 and 1.5 meters and i am not going too fast
        if (FrontDistance > 0.5 && FrontDistance < Statistics->minDistance && State->speed < 10 && FrontDistance > SMIN && State->state != ABORTOVERTAKE)
        {
            State->state = OVERTAKE;
        }
    }
    else if (FrontDistance == -1 && State->state != OVERTAKE && State->state != CRASH && State->state != PAUSE)
    {
        State->state = NORMAL;
    }

    if (checkPause(tIndex) && State->state != PAUSE)
    {
        addVecileInfoToSupport(State, tIndex);
        State->state = PAUSE;
    }
    else if (State->state == PAUSE && !checkPause(tIndex))
    {
        temp = getSupportNode(tIndex);
        State->acceleration = temp->acceleration;
        State->speed = temp->speed;
        State->state = temp->state;
    }

    // check for veicle in the left lane
    for (int i = 0; i < 360; i++)
    {
        if (proximitySensor(State->veicle, State->pos.x, State->pos.y, 15, i) != -1)
        {
            break;
        }
    }

    // check for speed limit
    if (State->speed < 0)
    {
        State->speed = 0;
    }

    if (State->speed > Statistics->maxSpeed)
    {
        State->speed = Statistics->maxSpeed;
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
    initVeicleState(&State, &Statistics, getListSize());

    veicleWidth = getVeicleWidth(State.veicle);

    // initialize veicle variables
    double DeltaPositionX = 0; // delta position in m
    double DeltaPositionY = 0; // delta position in m
    double DeltaSpeed = 0;     // delta speed in m/s

    // add veicle to list
    addVeicleToList(ti, State);

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

        // set veicle state in the list
        setVeicleState(ti, State);

        // Handle driving logic
        DrivingHandling(&State, &Statistics, ti);

        // CHECK STATUS

        // check if veicle is out of screen
        if (State.pos.x < -(veicleWidth / SCALE_FACTOR))
        {
            // remove veicle from list
            removeVeicleFromList(ti);
            running = 0;
        }

        // check for miss deadline
        if (deadline_miss(ti))
        {
            printf("VEICLE: deadline missed\n");
        }

        // wait for period
        wait_for_period(ti);
    }
    printf("OK: Veicle task terminated id: %d\n", ti);
    task_deactivate(ti);
    return NULL;
}