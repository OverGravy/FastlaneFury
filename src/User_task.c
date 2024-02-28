#include "../libs/User_task.h"

// user task handle:
// - create a new veicle
// - user input

void *user_task(void *arg)
{
    struct argument_struct userTaskArg = get_task_argument(arg);
    int running = 1;

    // Local selction variables
    int selection = NONE;       // what the user clicked
    int selected_veicle = NONE; // id of the selected veicle
    int game_state = PLAY;      // game state
    int mouse = 0;              // mouse click flag to avoid multiple click

    // car variables
    int index = 0;

    // autospawn variables
    time_t nextAutoSpawn; // will contain the time that pass between activation and deactivation
    int autoSpawn = 0;    // autospawn flag

    // input variables
    int key = 0;

    // time variables
    struct tm *tm;
    char s[64];

    // get task index
    int ti = get_task_index(arg);
    wait_for_activation(ti);

    printf("OK: User task activated\n");
    reset_deadline(ti);

    // user input
    while (running)
    {
        // get selected veicle from shared struct
        selected_veicle = userTaskArg.shared_struct->selected_veicle;

        // check for user input
        if (keypressed())
        {
            // check keyboard

            key = readkey() >> 8;

            switch (key)
            {
            // esc key
            case KEY_ESC: // esc key
                running = 0;
                printf("OK: User task deactivated\n");
                break;
            // spawn key
            case KEY_SPACE: // spawn a veicle

                index = get_free_index();
                if (index == -1)
                {
                    printf("ERROR: Can not create a new veicle No free index\n");
                }
                else
                {
                    if (game_state == PLAY)
                    {
                        if (create_veicle_task(userTaskArg, index, NONE) != 0)
                        {
                            printf("ERROR: error creating a new veicle_task\n");
                        }
                    }
                    else
                    {
                        printf("ERROR: Can not create a new veicle game paused\n");
                    }
                }

                break;
            // pause key
            case KEY_P: // pause game to cheng config
                if (game_state == PLAY)
                {
                    printf("OK: Game paused\n");
                    game_state = PAUSE_G;
                    userTaskArg.shared_struct->game_state = PAUSE_G;
                }
                else if (game_state == PAUSE_G)
                {
                    printf("OK: Game resumed\n");
                    game_state = PLAY;
                    userTaskArg.shared_struct->game_state = PLAY;
                }
                break;
            // zoom key
            case KEY_Z:
                // zoom the veicle if a veicle is selected
                if (selected_veicle != NONE && userTaskArg.shared_struct->buffer_id == MAIN_SCENE)
                {
                    userTaskArg.shared_struct->buffer_id = ZOOM_VEICLE;
                    userTaskArg.config_struct->zv_scale_factor = Z1_FACTOR;
                }
                // add zoom to the veicle if the veicle is already zoomed
                else if (userTaskArg.shared_struct->buffer_id == ZOOM_VEICLE)
                {
                    if (userTaskArg.config_struct->zv_scale_factor == Z1_FACTOR)
                    {
                        userTaskArg.config_struct->zv_scale_factor = Z2_FACTOR;
                    }
                    else if (userTaskArg.config_struct->zv_scale_factor == Z2_FACTOR)
                    {
                        userTaskArg.config_struct->zv_scale_factor = Z3_FACTOR;
                    }
                    else if (userTaskArg.config_struct->zv_scale_factor == Z3_FACTOR)
                    {
                        userTaskArg.config_struct->zv_scale_factor = Z1_FACTOR;
                    }
                }
                // zoom the scene if no veicle is selected
                if (userTaskArg.shared_struct->buffer_id == MAIN_SCENE && selected_veicle == NONE && userTaskArg.shared_struct->selection != NONE)
                {
                    userTaskArg.shared_struct->buffer_id = ZOOM_SCENE;
                }
                // add zoom to the scene if the scene is already zoomed
                else if (userTaskArg.shared_struct->buffer_id == ZOOM_SCENE)
                {
                    if (userTaskArg.config_struct->zv_scale_factor == Z1_FACTOR)
                    {
                        userTaskArg.config_struct->zv_scale_factor = Z2_FACTOR;
                    }
                    else if (userTaskArg.config_struct->zv_scale_factor == Z2_FACTOR)
                    {
                        userTaskArg.config_struct->zv_scale_factor = Z3_FACTOR;
                    }
                    else if (userTaskArg.config_struct->zv_scale_factor == Z3_FACTOR)
                    {
                        userTaskArg.config_struct->zv_scale_factor = Z1_FACTOR;
                    }
                }
                break;
            // zoom out key
            case KEY_X:
                if (userTaskArg.shared_struct->buffer_id == ZOOM_VEICLE || userTaskArg.shared_struct->buffer_id == ZOOM_SCENE)
                {
                    userTaskArg.shared_struct->buffer_id = MAIN_SCENE;
                }
                break;
            // auto spawn key
            case KEY_A:
                if (userTaskArg.config_struct->auto_spawn == AUTO)
                {
                    userTaskArg.config_struct->auto_spawn = MANUAL;
                    printf("OK: Auto spawn disabled\n");
                }
                else
                {
                    userTaskArg.config_struct->auto_spawn = AUTO;
                    printf("OK: Auto spawn enabled\n");
                }
                break;
            // car spawn key 
            case KEY_1:
                if (game_state == PLAY) // check if the game is paused
                {
                    index = get_free_index(); // get a free index
                    if (index == -1)
                    {
                        printf("ERROR: Can not create a new veicle No free index\n");
                    }
                    else
                    {
                        if (create_veicle_task(userTaskArg, index, CAR) != 0) // create a car 
                        {
                            printf("ERROR: error creating a new veicle_task\n");
                        }
                    }
                }
                printf("OK: Car selected\n");
                break;
            // truck spawn key
            case KEY_2:
                if (game_state == PLAY) // check if the game is paused
                {
                    index = get_free_index(); // get a free index
                    if (index == -1)
                    {
                        printf("ERROR: Can not create a new veicle No free index\n");
                    }
                    else
                    {
                        if (create_veicle_task(userTaskArg, index, TRUCK) != 0) // create a car 
                        {
                            printf("ERROR: error creating a new veicle_task\n");
                        }
                    }
                }
                printf("OK: Car selected\n");
                break;
            // motorcycle spawn key
            case KEY_3:
                if (game_state == PLAY) // check if the game is paused
                {
                    index = get_free_index(); // get a free index
                    if (index == -1)
                    {
                        printf("ERROR: Can not create a new veicle No free index\n");
                    }
                    else
                    {
                        if (create_veicle_task(userTaskArg, index, MOTORCYCLE) != 0) // create a car 
                        {
                            printf("ERROR: error creating a new veicle_task\n");
                        }
                    }
                }
                printf("OK: Car selected\n");
                break;
            // supercar spawn key
            case KEY_4:
                if (game_state == PLAY) // check if the game is paused
                {
                    index = get_free_index(); // get a free index
                    if (index == -1)
                    {
                        printf("ERROR: Can not create a new veicle No free index\n");
                    }
                    else
                    {
                        if (create_veicle_task(userTaskArg, index, SUPERCAR) != 0) // create a car 
                        {
                            printf("ERROR: error creating a new veicle_task\n");
                        }
                    }
                }
                printf("OK: Car selected\n");
                break;
            // use the arrow to increase the speed of the sim 
            case KEY_UP:
                if (userTaskArg.shared_struct->game_state == PLAY )
                {
                    *(userTaskArg.SimSpeed) += 0.1;
                }
                break;
            // use the arrow to decrease the speed of the sim
            case KEY_DOWN:
                if (userTaskArg.shared_struct->game_state == PLAY && *(userTaskArg.SimSpeed)>=0)
                {
                    *(userTaskArg.SimSpeed) -= 0.1;
                }
                break;

            }
        }

        // Handle mouse input
        if (mouse_b & 1)
        {
            if (mouse == 0)
            {
                // to avoid multiple click
                mouse = 1;
                selection = set_selection(mouse_x, mouse_y, userTaskArg.shared_list, userTaskArg.shared_struct_mutex, userTaskArg.shared_struct); // handle on what the user clicked
                switch (selection)
                {
                case VEICLE:
                    printf("OK: Veicle selected, %d\n", selected_veicle);
                    break;

                case BUTTON:
                    printf("OK: Button selected\n");
                    break;

                case ROAD:
                    printf("OK: Road selected\n");
                    break;
                }
            }
        }

        // avoid mosue multiple click
        if (mouse != 0)
        {
            mouse++;
            if (mouse == 15)
            {
                mouse = 0;
            }
        }

        // handle auto spawn veicle
        if (userTaskArg.config_struct->auto_spawn == AUTO && *(userTaskArg.SimSpeed) >= 0)
        {
            if (autoSpawn == 0)
            { // calculate next time car will spawn
                nextAutoSpawn = time(NULL) + (userTaskArg.config_struct->auto_spawn_time);

                // print nextAutoSpawn
                tm = localtime(&nextAutoSpawn);
                strftime(s, sizeof(s), "%c", tm);

                autoSpawn = 1;
            }
            if (time(NULL) >= nextAutoSpawn)
            {
                // spawn car
                index = get_free_index();
                if (index == NONE)
                {
                    printf("ERROR: Can not create a new veicle No free index\n");
                }
                else
                {
                    if (game_state == PLAY)
                    {
                        if (create_veicle_task(userTaskArg, index, NONE) != 0)
                        {
                            printf("ERROR: error creating a new veicle_task\n");
                        }
                    }
                }
                autoSpawn = 0;
            }
        }

        if (deadline_miss(ti))
        {
            printf("ERROR: user_task deadline missed\n");
        }

        // wait for next activation
        wait_for_period(ti);
    }

    // close the game
    task_deactivate(1); // close graphics task

    ptask_exit();           // close ramining task and exit
    close_allegro();        // close allegro
    close_statistic_file(); // close statistic file
    return NULL;
}