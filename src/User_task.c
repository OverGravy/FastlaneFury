#include "../libs/User_task.h"

// user task handle:
// - create a new veicle
// - user input

void *userTask(void *arg)
{
    struct argument userTaskArg = get_task_argument(arg);

    int running = 1;

    // Local selction variables
    int selection = -1;      // what the user clicked
    int selectedVeicle = -1; // selected veicle id
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
                if (!getAutoSpawn())
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
                            task_create(veicleTask, index, veicleTaskArg, 20, 20, 10, ACT);
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
                    pauseForMenu(userTaskArg.mutex, userTaskArg.shared);
                    printf("OK: Config menu open\n");
                    menu = 1;
                }
                else if (menu == 1 && pause == 0)
                {
                    menu = 0;
                    resumeFromMenu(userTaskArg.mutex, userTaskArg.shared);
                    printf("OK: Config menu closed\n");
                }

                break;

            case KEY_P: // pause game to cheng config
                if (pause == 0 && menu == 0)
                {
                    pauseVeicles(userTaskArg.mutex, userTaskArg.shared);
                    printf("OK: Game paused\n");
                    pause = 1;
                }
                else if (pause == 1 && menu == 0)
                {
                    pause = 0;
                    resumeVeicles(userTaskArg.mutex, userTaskArg.shared);
                    printf("OK: Game resumed\n");
                }
                break;
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
                    handleInputConfigMenu(mouse_x, mouse_y);
                }

                selection = getSelection(mouse_x, mouse_y, userTaskArg.mutex, userTaskArg.shared); // handle on what the user clicked
                switch (selection)
                {
                case VEICLE:
                    selectedVeicle = getSelectedVeicle(); // get veicle id
                    printf("OK: Veicle selected, %d\n", selectedVeicle);
                    break;

                case BUTTON:
                    selectedButton = getSelectedButton(); // get button id
                    printf("OK: Button selected, %d\n", selectedButton);
                    break;

                case ROAD:
                    printf("OK: Road selected\n");
                    selectedVeicle = -1;
                    setSelectedVeicle(selectedVeicle);
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
        if (getAutoSpawn()){
            if(autoSpawn == 0){ // calculate next time car will spawn
                nextAutoSpawn = time(NULL) + (getAutoSpawnTime());

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
                if (index == -1)
                {
                    printf("ERROR: Can not create a new veicle No free index\n");
                }
                else
                {
                    if (pause == 0 && menu == 0)
                    {
                        task_create(veicleTask, index, veicleTaskArg, 20, 20, 10, ACT);
                    }
                }
                autoSpawn = 0;
            }

        }


        // check if selected veicle is valid
        if (selectedVeicle != -1)
        {
            // check if the veicle is still active if not deselect it
            if (!task_is_active(selectedVeicle))
            {
                selectedVeicle = -1;                 // deselect veicle
                setSelectedVeicle(selectedVeicle);   // comunicate to graphics task that the veicle is deselected
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
    CloseStatisticFile(); // close statistic file
    return NULL;
}