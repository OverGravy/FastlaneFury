#include "../libs/Veicle.h"

// function that initialize veicle
void init_veicle_state(struct Veicle_State *state, struct Veicle_Statistics *statistics, struct argument_struct argument, int index)
{
    int margin = 0;

    // check if the veicle is predefined in argument struct
    if (argument.VeicleType[index] != -1)
    {
        // randomize veicle type considering the predefined veicle
        switch (argument.VeicleType[index])
        {
        case CAR:
            state->veicle = rand() % CAR_NUMBER;
            break;
        case TRUCK:
            state->veicle = rand() % TRUCK_NUMBER + CAR_NUMBER;
            break;
        case MOTORCYCLE:
            state->veicle = rand() % MOTORCYCLE_NUMBER + CAR_NUMBER + TRUCK_NUMBER;
            break;
        case SUPERCAR:
            state->veicle = rand() % SUPERCAR_NUMBER + CAR_NUMBER + TRUCK_NUMBER + MOTORCYCLE_NUMBER;
            break;
        }
    }
    else
    {
        state->veicle = rand() % (CAR_NUMBER + TRUCK_NUMBER + MOTORCYCLE_NUMBER + SUPERCAR_NUMBER);
    }

    // find the veicle type
    if (state->veicle >= 0 && state->veicle <= CAR_NUMBER)
    {
        get_veicle_staitistics(CAR, statistics);
    }
    else if (state->veicle > CAR_NUMBER && state->veicle <= CAR_NUMBER + TRUCK_NUMBER)
    {
        get_veicle_staitistics(TRUCK, statistics);
    }
    else if (state->veicle > CAR_NUMBER + TRUCK_NUMBER && state->veicle <= CAR_NUMBER + TRUCK_NUMBER + MOTORCYCLE_NUMBER)
    {
        get_veicle_staitistics(MOTORCYCLE, statistics);
    }
    else if (state->veicle > CAR_NUMBER + TRUCK_NUMBER + MOTORCYCLE_NUMBER && state->veicle <= CAR_NUMBER + TRUCK_NUMBER + MOTORCYCLE_NUMBER + SUPERCAR_NUMBER)
    {
        get_veicle_staitistics(SUPERCAR, statistics);
    }

    state->speed = 30.0; // in m/s
    if (*(argument.LastLane) == NONE)
    {
        state->lane = rand() % LANE_NUMBER;
    }
    else
    {
        // randomize lane considering the last lane used in the argument struct to avoid collision
        do
        {
            state->lane = rand() % LANE_NUMBER;
        } while(*(argument.LastLane) == state->lane);
        *(argument.LastLane) = state->lane;           // update last lane used
    }

    state->pos.x = (MY_SCREEN_W - 2) / SCALE_FACTOR;                                            // in meter
    margin = ((MY_SCREEN_H / (LANE_NUMBER + 1)) - (get_veicle_height(state->veicle))) / 2;      // margin in pixel
    state->pos.y = (((MY_SCREEN_H / (LANE_NUMBER + 1)) * state->lane) + margin) / SCALE_FACTOR; // in meter
    state->steeringAngle = 0.0;                                                                 // in degree
    state->acceleration = 0;                                                                    // in m/s^2
}

// STATE FUNCTIONS

// function that handle IDLE state logic and change state
void idle(struct Veicle_State *State, struct Veicle_Statistics *Statistics, struct Distances *distances)
{
    // Handling state logic

    // changing state logic
    if (distances->front == -1 && State->acceleration < Statistics->maxAcceleration) // No car in front
    {
        State->state = ACCELERATE;
    }

    if (distances->front != -1) // Car in front
    {
        State->state = SLOWDOWN;
    }

    if (distances->left == -1 && State->speed > 45 * (LANE_NUMBER + 1) && State->lane != LANE_NUMBER - 1) // No car on left and speed > 45 m/s
    {
        State->state = OVERTAKE;
    }
}

// function that handle NORMAL state logic and change state
void accelerate(struct Veicle_State *State, struct Veicle_Statistics *Statistics, struct Distances *distances)
{
    // Handling state logic

    State->acceleration += 0.5;
    if (State->acceleration > Statistics->maxAcceleration) // check for max acceleration
    {
        State->acceleration = Statistics->maxAcceleration; // set max acceleration
        State->state = IDLE;
    }

    // changing state logic
    if (distances->front != -1) // Car in front
    {
        State->state = SLOWDOWN;
    }
}

// function that handle SLOWDOWN state logic and change state
void slowDown(struct Veicle_State *State, struct Veicle_Statistics *Statistics, struct Distances *distances)
{
    // Handling state logic

    State->acceleration = distances->front - Statistics->minDistance; // set decelation in function of distance
    if (State->acceleration < Statistics->maxDeceleration)            // check for max deceleration
    {
        State->acceleration = Statistics->maxDeceleration; // set max deceleration
    }

    // changing state logic
    if (distances->front == -1) // No car in front
    {
        State->state = ACCELERATE;
    }

    if (distances->front <= Statistics->minDistance && State->lane != LANE_NUMBER - 1) // check for overtaking condition
    {
        State->state = OVERTAKE;
    }
}

// function that handle OVERTAKE state logic and change state
void overtake(struct Veicle_State *State, struct Veicle_Statistics *Statistics, struct Distances *distances)
{
    int margin;
    int middleLane;

    // handling state logic

    State->acceleration += 0.1; // set max acceleration

    if (State->acceleration > Statistics->maxAcceleration)
    {
        State->acceleration = Statistics->maxAcceleration;
    }
    State->steeringAngle -= 0.5; // increase gradually steering angle

    if (State->steeringAngle < -45) // check for max steering angle
    {
        State->steeringAngle = -45; // set max steering angle
    }

    // changing state logic

    // calculate middle lane position to stop overtaking
    margin = ((MY_SCREEN_H / (LANE_NUMBER + 1)) - (get_veicle_height(State->veicle))) / 2; // margin in pixel
    middleLane = (((MY_SCREEN_H / (LANE_NUMBER + 1)) * (State->lane + 1)) + margin);       // in meter

    if (((State->pos.y) * SCALE_FACTOR) >= middleLane)
    { // check if i am in the middle lane
        State->lane += 1;
        State->steeringAngle = 0;
        State->state = IDLE;
    }

    if (distances->left != -1) // check for abort overtake condition
    {
        State->steeringAngle = 0;
        State->state = ABORTOVERTAKE;
    }
}

// function that handle ABORTOVERTAKE state logic and change state
void abortOvertake(struct Veicle_State *State, struct Veicle_Statistics *Statistics, struct Distances *distances)
{
    int margin;
    int middleLane;

    // handling state logic

    State->steeringAngle += 0.5; // increase gradually steering angle

    if (distances->front != -1) // check if i have veicle in front while im aborting overtake
    {
        State->acceleration = distances->front - Statistics->minDistance;
    }
    else // still if i have a veicle i was trying to overtake i need to slow down
    {
        if (distances->right != -1)
        {
            State->acceleration = distances->right - Statistics->minDistance;
        }
        State->acceleration = Statistics->maxAcceleration;
    }

    if (State->acceleration < Statistics->maxDeceleration) // check for max deceleration
    {
        State->acceleration = Statistics->maxDeceleration;
    }

    if (State->steeringAngle > 45) // check for max steering angle
    {
        State->steeringAngle = 45;
    }

    // changing state logic

    margin = ((MY_SCREEN_H / (LANE_NUMBER + 1)) - (get_veicle_height(State->veicle))) / 2; // margin in pixel
    middleLane = (((MY_SCREEN_H / (LANE_NUMBER + 1)) * (State->lane)) + margin);           // in meter

    if (((State->pos.y) * SCALE_FACTOR) <= middleLane)
    { // check if i am in the middle lane
        printf("Veicle: %d, switch to lane: %d\n", State->veicle, State->lane);
        State->steeringAngle = 0;
        State->state = IDLE;
    }
}

// function that handle CRASH state logic and change state
void crash(struct Veicle_State *State)
{
    // handling state logic
    State->speed = 0;
    State->acceleration = 0;
    printf("CRASH\n");
}

// function that handle PAUSE state logic and change state
void pauseState(struct Veicle_State *State)
{
    // handling state logic
    State->speed = 0;
    State->acceleration = 0;
}

// DRIVING FUNCTION

// function that handle driving movement
void driving_handling(struct Veicle_State *State, struct Veicle_Statistics *Statistics, struct Distances *distances)
{
    switch (State->state)
    {
    case IDLE:
        idle(State, Statistics, distances);
        break;
    case ACCELERATE:
        accelerate(State, Statistics, distances);
        break;
    case SLOWDOWN:
        slowDown(State, Statistics, distances);
        break;
    case OVERTAKE:
        overtake(State, Statistics, distances);
        break;
    case ABORTOVERTAKE:
        abortOvertake(State, Statistics, distances);
        break;
    case CRASH:
        crash(State);
        break;
    case PAUSE:
        pauseState(State);
        break;
    default:
        break;
    }

    // check for speed limit
    if (State->speed < 0)
    {
        State->speed = 0;
    }

    // check max speed
    if (State->speed > Statistics->maxSpeed)
    {
        State->speed = Statistics->maxSpeed;
    }
}

// function tha handle sensor measurements
void do_mesurements(struct Veicle_State *State, double measurement[], struct Distances *distances, BITMAP *scene_buffer)
{

    int i, j, x, y; // support index and variables
    int degree;

    // # GET DISTANCE FRONT #

    x = (State->pos.x) * SCALE_FACTOR - 10;
    y = (State->pos.y * SCALE_FACTOR) + (get_veicle_height(State->veicle) / 2);

    measurement[0] = proximity_sensor(x, y, RANGE_FRONT, 180, scene_buffer);

    // if im in first or in the last lane i dont need to check for left or right

    // # GET DISTANCE LEFT #

    if (State->lane != 0)
    {
        x = (((State->pos.x) * SCALE_FACTOR) + (get_veicle_width(State->veicle) / 2));
        y = ((State->pos.y * SCALE_FACTOR) + get_veicle_height(State->veicle)) + 10;

        for (i = 1; i <= DETECTION_DEGREE; i++)
        {
            degree = 10 + i;
            measurement[i] = proximity_sensor(x, y, RANGE_SIDE, degree, scene_buffer);
        }
    }
    else
    {
        for (i = 1; i <= DETECTION_DEGREE; i++)
        {
            measurement[i] = -1;
        }
    }
    // # GET DISTANCE RIGHT #

    if (State->lane != LANE_NUMBER - 1)
    {
        x = (((State->pos.x) * SCALE_FACTOR) + (get_veicle_width(State->veicle) / 2));
        y = ((State->pos.y * SCALE_FACTOR) - 10);

        for (i = DETECTION_DEGREE + 1; i <= (DETECTION_DEGREE * 2); i++)
        {
            j++;
            degree = 190 + j;
            measurement[i] = proximity_sensor(x, y, RANGE_SIDE, degree, scene_buffer);
        }
    }
    else
    {
        for (i = DETECTION_DEGREE + 1; i <= (DETECTION_DEGREE * 2); i++)
        {
            measurement[i] = -1;
        }
    }

    // # GET DISTANCE BACK #

    x = (((State->pos.x) * SCALE_FACTOR) + get_veicle_width(State->veicle)) + 10;
    y = (((State->pos.y) * SCALE_FACTOR) + (get_veicle_height(State->veicle) / 2));

    measurement[(DETECTION_DEGREE * 2) + 1] = proximity_sensor(x, y, RANGE_BACK, 0, scene_buffer);

    // CHECK DISTANCE -----------------------------------------------------

    // # CHECK FRONT #

    if (measurement[0] != -1)
    {
        distances->front = measurement[0];
    }
    else
    {
        distances->front = -1;
    }

    // #CHECK LEFT#

    for (i = 1; i <= DETECTION_DEGREE; i++)
    {
        if (measurement[i] != -1)
        {
            distances->left = measurement[i];
            break;
        }
        else
        {
            distances->left = -1;
        }
    }

    // #CHECK RIGHT#

    for (i = DETECTION_DEGREE; i <= (DETECTION_DEGREE * 2); i++)
    {
        if (measurement[i] != -1)
        {
            distances->right = measurement[i];
            break;
        }
        else
        {
            distances->right = -1;
        }
    }

    // #CHECK BACK#

    if (measurement[(DETECTION_DEGREE * 2) + 1] != -1)
    {
        distances->back = measurement[(DETECTION_DEGREE * 2) + 1];
    }
    else
    {
        distances->back = -1;
    }
}