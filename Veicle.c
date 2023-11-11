#include "Veicle.h"

/**
 * Dentro distance sono contenute tutte le distanze, in metri, che il veicolo ha rilevato.
 *  Front, left right e back sono variabili che vengono riempite con l'index del vettore che ocntiene tutte le distanze nel caso ci sia una distanza valida.
 *  Se non ci sono distanze valide, il valore di front, left, right e back è -1.
 */

int front = -1;
int left = -1;
int back = -1;
int right = -1;

// function that handle IDLE state logic and change state
void idle(struct VeicleState *State, struct VeicleStatistics *Statistics, int tIndex, double distance[])
{
    // Handling state logic

    // changing state logic
    if (front == -1 && State->acceleration < Statistics->maxAcceleration) // No car in front
    {
        State->state = ACCELERATE;
    }

    if (front != -1) // Car in front
    {
        State->state = SLOWDOWN;
    }

    if (left == -1 && State->speed > 45) // No car on left and speed > 45 m/s
    {
        State->state = OVERTAKE;
    }
}

// function that handle NORMAL state logic and change state
void accelerate(struct VeicleState *State, struct VeicleStatistics *Statistics, int tIndex, double distance[])
{
    // Handling state logic

    State->acceleration += 0.5;
    if (State->acceleration > Statistics->maxAcceleration) // check for max acceleration
    {
        State->acceleration = Statistics->maxAcceleration; // set max acceleration
        State->state = IDLE;
    }

    // changing state logic
    if (front != -1) // Car in front
    {
        State->state = SLOWDOWN;
    }
}

// function that handle SLOWDOWN state logic and change state
void slowDown(struct VeicleState *State, struct VeicleStatistics *Statistics, int tIndex, double distance[])
{
    // Handling state logic

    State->acceleration = distance[front] - Statistics->minDistance; // set decelation in function of distance
    if (State->acceleration < Statistics->maxDeceleration)           // check for max deceleration
    {
        State->acceleration = Statistics->maxDeceleration; // set max deceleration
    }

    // changing state logic
    if (front == -1) // No car in front
    {
        State->state = ACCELERATE;
    }
    if (distance[front] > 0.5 && distance[front] < Statistics->minDistance && State->speed < 15) // check for overtaking condition
    {
        State->state = OVERTAKE;
    }
    if (distance[front] < 0.2) // check for crash condition
    {
        State->state = CRASH;
    }
}

// function that handle OVERTAKE state logic and change state
void overtake(struct VeicleState *State, struct VeicleStatistics *Statistics, int tIndex, double distance[])
{
    int margin;
    int middleLane;

    // handling state logic

    State->acceleration = Statistics->maxAcceleration; // set max acceleration
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
        printf("Veicle: %d, switch to lane: %d\n", State->veicle, State->lane);
        State->steeringAngle = 0;
        State->state = IDLE;
    }

    if (left != -1) // check for abort overtake condition
    {
        State->steeringAngle = 0;
        State->state = ABORTOVERTAKE;
    }
}

// function that handle ABORTOVERTAKE state logic and change state
void abortOvertake(struct VeicleState *State, struct VeicleStatistics *Statistics, int tIndex, double distance[])
{
    int margin;
    int middleLane;

    // handling state logic

    State->steeringAngle += 0.5; // increase gradually steering angle

    if (front != -1) // check if i have veicle in front while im aborting overtake
    {
        State->acceleration = distance[front] - Statistics->minDistance;
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
void crash(struct VeicleState *State, struct VeicleStatistics *Statistics, int tIndex, double distance[])
{
    // handling state logic
    State->speed = 0;
    State->acceleration = 0;
    printf("CRASH\n");
}

// function that handle PAUSE state logic and change state
void pauseState(struct VeicleState *State, struct VeicleStatistics *Statistics, int tIndex, double distance[])
{
    // handling state logic
    State->speed = 0;
    State->acceleration = 0;
}




// function that handle Veicle driving logic
void DrivingHandling(struct VeicleState *State, struct VeicleStatistics *Statistics, int tIndex, double distance[])
{

    int middleLane = 0;
    int margin;
    int i, j, x, y;
    int degree;
    struct supportList *temp = NULL;

    // MISURE -----------------------------------------------------

    // # GET DISTANCE FRONT #

    x = (State->pos.x) * SCALE_FACTOR - 10;
    y = (State->pos.y * SCALE_FACTOR) + (getVeicleHeight(State->veicle) / 2);

    distance[0] = proximitySensor(x, y, 150, 180);

    // # GET DISTANCE LEFT #

    x = (((State->pos.x) * SCALE_FACTOR) + (getVeicleWidth(State->veicle) / 2));
    y = ((State->pos.y * SCALE_FACTOR) + getVeicleHeight(State->veicle)) + 10;

    for (i = 1; i <= DETECTION_DEGREE; i++)
    {
        degree = 10 + i;
        distance[i] = proximitySensor(x, y, 150, degree);
    }

    // # GET DISTANCE RIGHT #

    x = (State->pos.x * SCALE_FACTOR) + (getVeicleWidth(State->veicle) / 2);
    y = (State->pos.y * SCALE_FACTOR) - 10;

    j = 0;
    for (i = DETECTION_DEGREE + 1; i <= (DETECTION_DEGREE * 2); i++)
    {
        j++;
        degree = 190 + j;
        distance[i] = proximitySensor(x, y, 150, degree);
    }

    // # GET DISTANCE BACK #

    x = (((State->pos.x) * SCALE_FACTOR) + getVeicleWidth(State->veicle)) + 10;
    y = (((State->pos.y) * SCALE_FACTOR) + (getVeicleHeight(State->veicle) / 2));

    distance[(DETECTION_DEGREE * 2) + 1] = proximitySensor(x, y, 50, 0);

    // CHECK DISTANCE -----------------------------------------------------

    // # CHECK FRONT #

    if (distance[0] != -1)
    {
        front = 0;
    }
    else
    {
        front = -1;
    }

    // #CHECK LEFT#

    for (i = 1; i <= DETECTION_DEGREE; i++)
    {
        if (distance[i] != -1)
        {
            left = i;
            break;
        }
        else
        {
            left = -1;
        }
    }

    // #CHECK RIGHT#

    for (i = DETECTION_DEGREE; i <= (DETECTION_DEGREE * 2); i++)
    {
        if (distance[i] != -1)
        {
            right = i;
            break;
        }
        else
        {
            right = -1;
        }
    }

    // #CHECK BACK#

    if (distance[(DETECTION_DEGREE * 2) + 1] != -1)
    {
        back = (DETECTION_DEGREE * 2) + 1;
    }
    else
    {
        back = -1;
    }

    // STATE HANDLING -----------------------------------------------

    switch (State->state)
    {

    case IDLE: // normal driving
        idle(State, Statistics, tIndex, distance);
        break;

    case ACCELERATE: // accelerating
        accelerate(State, Statistics, tIndex, distance);
        break;

    case SLOWDOWN: // slowing down
        slowDown(State, Statistics, tIndex, distance);
        break;

    case OVERTAKE: // overtaking
        overtake(State, Statistics, tIndex, distance);
        break;

    case ABORTOVERTAKE: // abort overtake
        abortOvertake(State, Statistics, tIndex, distance);
        break;

    case CRASH: // crash
        crash(State, Statistics, tIndex, distance);
        break;

    case PAUSE:
        pauseState(State, Statistics, tIndex, distance);
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

    // handling PAUSE state
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
}

// function task veicle
void *veicleTask(void *arg)
{
    int running = 1;

    // get task argument and wait for activation
    struct argument veicleArg = get_task_argument(arg);
    int ti = get_task_index(arg);
    wait_for_activation(ti);

    // initialize veicle State
    struct VeicleState State;
    struct VeicleStatistics Statistics;
    initVeicleState(&State, &Statistics, getListSize());

    // initialize veicle variables
    double DeltaPositionX = 0;                   // delta position in m
    double DeltaPositionY = 0;                   // delta position in m
    double DeltaSpeed = 0;                       // delta speed in m/s
    double distance[(DETECTION_DEGREE * 2) + 2]; // distance on m

    // add veicle to shared list
    addVeicleToList(ti, State);

    printf("OK: Veicle task activated id: %d\n", ti);

    while (running)
    {
        // UPDATE VEICLE STATE

        printf("Veicle %d, state: %d\n", ti, State.state);

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
        DrivingHandling(&State, &Statistics, ti, distance);

        // CHECK STATUS

        // check if veicle is out of screen
        if ((State.pos.x < -(getVeicleWidth(State.veicle) / SCALE_FACTOR)))
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
    task_clean(ti);
    return NULL;
}