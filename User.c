#include "User.h"

// user task handle:
// - create a new veicle
// - user input

void *userTask(void *arg)
{
    struct argument userTaskArg = get_task_argument(arg);

    int running = 1;

    // Local selction variables
    int selection = -1;
    int selectedVeicle = -1;
    int selectedButton = -1;
    int pause = 0;

    // car variables
    struct argument veicleTaskArg;
    int index = 0;

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
                veicleTaskArg.mutex = userTaskArg.mutex;
                veicleTaskArg.shared = userTaskArg.shared;
                index = get_free_index();
                if (index == -1)
                {
                    printf("ERROR:can not create a new veicle No free index\n");
                }
                else
                {
                    if (pause == 0)
                    {
                        task_create(veicleTask, index, veicleTaskArg, 20, 20, 10, ACT);
                    }
                    else
                    {
                        printf("ERROR: can not create a new veicle game paused\n");
                    }
                }
                break;
            case KEY_P:
                if (pause == 0)
                {
                    pauseVeicles(userTaskArg.mutex, userTaskArg.shared);
                    printf("OK: game paused\n");
                    pause = 1;
                }
                else
                {
                    pause = 0;
                    resumeVeicles(userTaskArg.mutex, userTaskArg.shared);
                    printf("OK: game resumed\n");
                }

                break;
            }
        }

        // Handle mouse input
        if (mouse_b & 1)
        {
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
                break;
            }
        }

        // check if selected veicle is valid
        if (selectedVeicle != -1)
        {
            // check if the veicle is still active if not deselect it
            if (!task_is_active(selectedVeicle))
            {
                selectedVeicle = -1;
                setSelectedVeicle(selectedVeicle);
            }
        }

        // Handle button selection

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
    closeAllegro();       // close allegro
    CloseStatisticFile(); // close statistic file
    return NULL;
}