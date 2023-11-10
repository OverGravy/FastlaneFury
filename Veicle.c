#include "Veicle.h"

/**
 * Dentro distance sono contenute tutte le distanze, in metri, che il veicolo ha rilevato.
 *  Front, left right e back sono variabili che vengono riempite con l'index del vettore che ocntiene tutte le distanze nel caso ci sia una distanza valida.
 *  Se non ci sono distanze valide, il valore di front, left, right e back è -1.
 */

// function that handle Veicle driving logic
void DrivingHandling(
    struct VeicleState *State, struct VeicleStatistics *Statistics, int tIndex, double distance[])
{

    int middleLane = 0;
    int margin;
    int i, j, x, y;
    int degree;
    struct supportList *temp = NULL;

    // every possible direction
    int front = -1;
    int left = -1;
    int back = -1;
    int right = -1;

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

    // STATE HANDLING -----------------------------------------------

    switch (State->state)
    {

    case NORMAL: // normal driving
        State->acceleration += 0.5;
        if (State->acceleration > Statistics->maxAcceleration) // check for max acceleration
        {
            State->acceleration = Statistics->maxAcceleration; // set max acceleration
        }
        break;

    case SLOWDOWN:                                                   // slowing down
        State->acceleration = distance[0] - Statistics->minDistance; // set decelation in function of distance
        if (State->acceleration < Statistics->maxDeceleration)       // check for max deceleration
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

    // ANALIZE MISURE -----------------------------------------------------

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
    printf("left: %d\n", left);

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
    

    // check for veicle in front
    if (front != -1 && State->state != PAUSE)
    {
        // switch to crash
        if (distance[front] == SMIN)
        {
            State->state = CRASH;
        }

        // switch to slowing down
        if (distance[front] < Statistics->minDistance && State->state != OVERTAKE && State->state != CRASH)
        {
            State->state = SLOWDOWN;
        }

        // switch to overtaking, the condition is that the distance from the veicle in front is between 0.5 and 1.5 meters and i am not going too fast
        if (distance[front] > 0.5 && distance[front] < Statistics->minDistance && State->speed < 15  && State->state != ABORTOVERTAKE)
        {
            State->state = OVERTAKE;
        }

        // switch to abort overtake if i am overtaking and i detect a veicle on the left
        if (State->state == OVERTAKE && left != -1)
        {
            State->state = ABORTOVERTAKE;
        }
    }
    else if (front == -1 && State->state != OVERTAKE && State->state != CRASH && State->state != PAUSE)
    {
        State->state = NORMAL;
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