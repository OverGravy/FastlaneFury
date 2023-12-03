#include "../libs/User_task.h"

// user task handle:
// - create a new veicle
// - user input

void *user_task(void *arg)
{
    struct argument userTaskArg = get_task_argument(arg);

    int running = 1;

    // Local selction variables
    int selection = -1;      // what the user clicked
    int selectedButton = -1; // selected button id
    int pause = 0;           // pause flag
    int menu = 0;            // menu flag

    int mouse = 0;           // mouse click flag to avoid multiple click

    // car variables
    struct argument veicleTaskArg;
    int index = 0;

    // autospawn variables
    time_t nextAutoSpawn;  // will contain the time that pass between activation and deactivation
    int autoSpawn = 0; // autospawn flag

    // get task index
    int ti = get_task_index(arg);
    wait_for_activation(ti);

    printf("OK: User task activated\n");

    // user input
    while (running)
    {   
        // check for user input
        if (keypressed())
        {
            // check keyboard
            int key = readkey() >> 8;

            switch (key)
            {

            case KEY_ESC: // esc key
                running = 0;
                printf("OK: User task deactivated\n");
                break;

            case KEY_SPACE: // spawn a veicle
                if (!get_auto_spawn())
                {
                    veicleTaskArg.mutex = userTaskArg.mutex;
                    veicleTaskArg.shared = userTaskArg.shared;
                    index = get_free_index();
                    if (index == -1)
                    {
                        printf("ERROR: Can not create a new veicle No free index\n");
                    }
                    else
                    {
                        if (pause == 0 && menu == 0)
                        {
                            task_create(veicle_task, index, veicleTaskArg, 20, 20, 10, ACT);
                        }
                        else
                        {
                            printf("ERROR: Can not create a new veicle game paused\n");
                        }
                    }
                }
                break;

            case KEY_M: // open config menu
                if (menu == 0 && pause == 0)
                {
                    pause_for_menu(userTaskArg.mutex, userTaskArg.shared);
                    printf("OK: Config menu open\n");
                    menu = 1;
                }
                else if (menu == 1 && pause == 0)
                {
                    menu = 0;
                    resume_from_menu(userTaskArg.mutex, userTaskArg.shared);
                    printf("OK: Config menu closed\n");
                }

                break;

            case KEY_P: // pause game to cheng config
                if (pause == 0 && menu == 0)
                {
                    pause_veicles(userTaskArg.mutex, userTaskArg.shared);
                    printf("OK: Game paused\n");
                    pause = 1;
                }
                else if (pause == 1 && menu == 0)
                {
                    pause = 0;
                    resume_veicles(userTaskArg.mutex, userTaskArg.shared);
                    printf("OK: Game resumed\n");
                }
                break;
            case KEY_Z:
                if(get_selected_veicle() != NONE && pause == 0 && menu == 0){
                    set_zoomed_veicle(get_selected_veicle());
                }
            }
        }

        // Handle mouse input
        if (mouse_b & 1)
        {
            if (mouse == 0)
            {
                mouse = 1;
                if (menu == 1)
                {
                    handle_input_config_menu(mouse_x, mouse_y);
                }

                selection = set_selection(mouse_x, mouse_y, userTaskArg.mutex, userTaskArg.shared); // handle on what the user clicked
                switch (selection)
                {
                case VEICLE:
                    printf("OK: Veicle selected, %d\n", get_selected_veicle());
                    break;

                case BUTTON:
                    printf("OK: Button selected, %d\n", get_selected_button());
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
        if (get_auto_spawn()){
            if(autoSpawn == 0){ // calculate next time car will spawn
                nextAutoSpawn = time(NULL) + (get_auto_spawn_time());

                // print nextAutoSpawn
                struct tm *tm = localtime(&nextAutoSpawn);
                char s[64];
                strftime(s, sizeof(s), "%c", tm);


                autoSpawn = 1;
            }
            if(time(NULL) >= nextAutoSpawn){ // spawn car
                veicleTaskArg.mutex = userTaskArg.mutex;
                veicleTaskArg.shared = userTaskArg.shared;
                index = get_free_index();
                if (index == NONE)
                {
                    printf("ERROR: Can not create a new veicle No free index\n");
                }
                else
                {
                    if (pause == 0 && menu == 0)
                    {
                        task_create(veicle_task, index, veicleTaskArg, 20, 20, 10, ACT);
                    }
                }
                autoSpawn = 0;
            }

        }


        // check if selected veicle is valid
        if (get_selected_veicle() != NONE)
        {
            // check if the veicle is still active if not deselect it
            if (!task_is_active(get_selected_veicle()))
            {
                set_selected_veicle(NONE);   // comunicate to graphics task that the veicle is deselected
                if(get_zoomed_veicle() == get_selected_veicle()){
                    set_zoomed_veicle(NONE);
                }
            } 
        }

        if (deadline_miss(ti))
        {
            printf("USER: deadline missed\n");
        }

        // wait for next activation
        wait_for_period(ti);
    }

    // close the game
    task_deactivate(1); // close graphics task

    ptask_exit();         // close ramining task and exit
    close_allegro();       // close allegro
    close_statistic_file(); // close statistic file
    return NULL;
}