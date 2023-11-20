#include "../libs/Drive.h"

// STATE FUNCTIONS

// function that handle IDLE state logic and change state
void idle(struct VeicleState *State, struct VeicleStatistics *Statistics, struct Distances *distances)
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

    if (distances->left == -1 && State->speed > 45) // No car on left and speed > 45 m/s
    {
        State->state = OVERTAKE;
    }
}

// function that handle NORMAL state logic and change state
void accelerate(struct VeicleState *State, struct VeicleStatistics *Statistics, struct Distances *distances)
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
void slowDown(struct VeicleState *State, struct VeicleStatistics *Statistics, struct Distances *distances)
{
    // Handling state logic

    State->acceleration = distances->front - Statistics->minDistance; // set decelation in function of distance
    if (State->acceleration < Statistics->maxDeceleration)              // check for max deceleration
    {
        State->acceleration = Statistics->maxDeceleration; // set max deceleration
    }

    // changing state logic
    if (distances->front == -1) // No car in front
    {
        State->state = ACCELERATE;
    }
    if (distances->front > 3 && distances->front < Statistics->minDistance) // check for overtaking condition
    {
        State->state = OVERTAKE;
    }
    if (distances->front < 0.01) // check for crash condition
    {
        State->state = CRASH;
    }
}

// function that handle OVERTAKE state logic and change state
void overtake(struct VeicleState *State, struct VeicleStatistics *Statistics, struct Distances *distances)
{
    int margin;
    int middleLane;

    // handling state logic

    State->acceleration += 0.5; // set max acceleration
    if (State->acceleration > Statistics->maxAcceleration)
    {
        State->acceleration = Statistics->maxAcceleration;
    }
    State->steeringAngle -= 0.5;                       // increase gradually steering angle

    if (State->steeringAngle < -45) // check for max steering angle
    {
        State->steeringAngle = -45; // set max steering angle
    }

    // changing state logic

    // calculate middle lane position to stop overtaking
    margin = ((MY_SCREEN_H / (LANE_NUMBER + 1)) - (getVeicleHeight(State->veicle))) / 2; // margin in pixel
    middleLane = (((MY_SCREEN_H / (LANE_NUMBER + 1)) * (State->lane + 1)) + margin);     // in meter

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
void abortOvertake(struct VeicleState *State, struct VeicleStatistics *Statistics, struct Distances *distances)
{
    int margin;
    int middleLane;

    // handling state logic

    State->steeringAngle += 0.5; // increase gradually steering angle

    if (distances->front != -1) // check if i have veicle in front while im aborting overtake
    {
        State->acceleration = distances->front - Statistics->minDistance;
    }
    else
    {
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

    margin = ((MY_SCREEN_H / (LANE_NUMBER + 1)) - (getVeicleHeight(State->veicle))) / 2; // margin in pixel
    middleLane = (((MY_SCREEN_H / (LANE_NUMBER + 1)) * (State->lane)) + margin);         // in meter

    if (((State->pos.y) * SCALE_FACTOR) <= middleLane)
    { // check if i am in the middle lane
        printf("Veicle: %d, switch to lane: %d\n", State->veicle, State->lane);
        State->steeringAngle = 0;
        State->state = IDLE;
    }
}

// function that handle CRASH state logic and change state
void crash(struct VeicleState *State)
{
    // handling state logic
    State->speed = 0;
    State->acceleration = 0;
    printf("CRASH\n");
}

// function that handle PAUSE state logic and change state
void pauseState(struct VeicleState *State)
{
    // handling state logic
    State->speed = 0;
    State->acceleration = 0;
}

// DRIVING FUNCTION

// function that handle driving movement
void DrivingHandling(struct VeicleState *State, struct VeicleStatistics *Statistics, struct Distances *distances)
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

    if (State->speed > Statistics->maxSpeed)
    {
        State->speed = Statistics->maxSpeed;
    }
}

// function tha handle sensor measurements
void DoMesurements(struct VeicleState *State, double measurement[], struct Distances *distances){
    
    int i, j, x, y;                  // support index and variables 
    int degree;
   

    // # GET DISTANCE FRONT #

    x = (State->pos.x) * SCALE_FACTOR - 10;
    y = (State->pos.y * SCALE_FACTOR) + (getVeicleHeight(State->veicle) / 2);

    measurement[0] = proximitySensor(x, y, RANGE, 180);

    // # GET DISTANCE LEFT #

    x = (((State->pos.x) * SCALE_FACTOR) + (getVeicleWidth(State->veicle) / 2));
    y = ((State->pos.y * SCALE_FACTOR) + getVeicleHeight(State->veicle)) + 10;

    for (i = 1; i <= DETECTION_DEGREE; i++)
    {
        degree = 10 + i;
        measurement[i] = proximitySensor(x, y, RANGE, degree);
    }

    // # GET DISTANCE RIGHT #

    x = (State->pos.x * SCALE_FACTOR) + (getVeicleWidth(State->veicle) / 2);
    y = (State->pos.y * SCALE_FACTOR) - 10;

    j = 0;
    for (i = DETECTION_DEGREE + 1; i <= (DETECTION_DEGREE * 2); i++)
    {
        j++;
        degree = 190 + j;
        measurement[i] = proximitySensor(x, y, RANGE, degree);
    }

    // # GET DISTANCE BACK #

    x = (((State->pos.x) * SCALE_FACTOR) + getVeicleWidth(State->veicle)) + 10;
    y = (((State->pos.y) * SCALE_FACTOR) + (getVeicleHeight(State->veicle) / 2));

    measurement[(DETECTION_DEGREE * 2) + 1] = proximitySensor(x, y, 50, 0);

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