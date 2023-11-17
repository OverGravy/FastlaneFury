#include "../libs/Veicle.h"

/**
 * Dentro distance sono contenute tutte le distanze, in metri, che il veicolo ha rilevato.
 *  Front, left right e back sono variabili che vengono riempite con l'index del vettore che ocntiene tutte le distanze nel caso ci sia una distanza valida.
 *  Se non ci sono distanze valide, il valore di front, left, right e back è -1.
 */

// function task veicle
void *veicleTask(void *arg)
{
    int running = 1;

    // get task argument and wait for activation
    struct argument veicleArg = get_task_argument(arg);
    int ti = get_task_index(arg);
    wait_for_activation(ti);

    // initialize veicle State
    struct VeicleState State;           // veicle state
    struct supportList *temp = NULL;    // support list temp node to retrive veicle state after pause
    struct VeicleStatistics Statistics; // veicle statistics
    initVeicleState(&State, &Statistics, getListSize());

    // initialize veicle variables
    double DeltaPositionX = 0;                       // delta position in m
    double DeltaPositionY = 0;                       // delta position in m
    double DeltaSpeed = 0;                           // delta speed in m/s
    struct Distances distance;                       // veicle distances
    double measurements[(DETECTION_DEGREE * 2) + 2]; // all sensor measurements

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

        // execute sensor
        if (State.state != PAUSE)
        {
            DoMesurements(&State, measurements, &distance);
        }

        // Handle driving logic
        DrivingHandling(&State, &Statistics, &distance);

        // CHECK STATUS

        if (checkPause(ti) && State.state != PAUSE)
        {
            addVecileInfoToSupport(&State, ti);
            State.state = PAUSE;
        }
        else if (State.state == PAUSE && !checkPause(ti))
        {
            temp = getSupportNode(ti);
            State.acceleration = temp->acceleration;
            State.speed = temp->speed;
            State.state = temp->state;
        }

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