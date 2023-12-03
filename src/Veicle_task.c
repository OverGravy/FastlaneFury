
#include "../libs/Veicle_task.h"

/**
 * Dentro distance sono contenute tutte le distanze, in metri, che il veicolo ha rilevato.
 *  Front, left right e back sono variabili che vengono riempite con l'index del vettore che ocntiene tutte le distanze nel caso ci sia una distanza valida.
 *  Se non ci sono distanze valide, il valore di front, left, right e back è -1.
 */

// function task veicle
void *veicle_task(void *arg)
{
    int running = 1;

    // get task argument and wait for activation
    struct argument veicleArg = get_task_argument(arg);
    int ti = get_task_index(arg);
    wait_for_activation(ti);

    // initialize veicle State
    struct Veicle_State State;           // veicle state
    struct Support_List *temp = NULL;    // support list temp node to retrive veicle state after pause
    struct Veicle_Statistics Statistics; // veicle statistics
    init_veicle_state(&State, &Statistics);

    // initialize veicle variables
    double DeltaPositionX = 0;                       // delta position in m
    double DeltaPositionY = 0;                       // delta position in m
    double DeltaSpeed = 0;                           // delta speed in m/s
    struct Distances distance;                       // veicle distances
    double measurements[(DETECTION_DEGREE * 2) + 2]; // all sensor measurements

    // add veicle to shared list
    add_veicle_to_list(ti, State);

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
        set_veicle_state(ti, State);

        // execute sensor
        if (State.state != PAUSE)
        {
            do_mesurements(&State, measurements, &distance);
        }

        // Handle driving logic
        driving_handling(&State, &Statistics, &distance);

        // CHECK STATUS

        if (check_pause(ti) && State.state != PAUSE)
        {
            add_vecile_info_to_support(&State, ti);
            State.state = PAUSE;
        }
        else if (State.state == PAUSE && !check_pause(ti))
        {
            temp = get_support_node(ti);
            State.acceleration = temp->acceleration;
            State.speed = temp->speed;
            State.state = temp->state;
        }

        // check if veicle is out of screen
        if ((State.pos.x < -(get_veicle_width(State.veicle) / SCALE_FACTOR)))
        {
            // remove veicle from list
            remove_veicle_from_list(ti);
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