#include "User.h"

// user task handle:
// - create a new veicle
// - user input

void *userTask(void *arg)
{
    struct argument userTaskArg = get_task_argument(arg);

    int running = 1;

    // selction variables
    int selection = -1;
    int selectedVeicle = -1;
    int selectedButton = -1;

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
                struct argument veicleTaskArg;
                veicleTaskArg.mutex = userTaskArg.mutex;
                veicleTaskArg.shared = userTaskArg.shared;
                int index = get_free_index();
                if (index == -1)
                {
                    printf("ERROR:can not create a new veicle No free index\n");
                }
                else
                {
                    task_create(veicleTask, index, veicleTaskArg, 20, 20, 10, ACT);
                }
            }
        }
        // Handle mouse input

        // SELECTION HANDLER
        /*

        We get what the user selected and we handle it separately

        */

        if (mouse_b & 1)
        {
            selection = getSelection(mouse_x, mouse_y, userTaskArg.mutex, userTaskArg.shared); // get selection
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
    task_deactivate(ti);
    return NULL;
}