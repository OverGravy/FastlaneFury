#include "User.h"

// user task handle:
// - create a new veicle
// - user input

void *userTask(void *arg)
{
    struct argument userTaskArg = get_task_argument(arg);

    int running = 1;
    int counter = 0;

    int zoomIn = 0;
    int zoomId = 0;

    ALLEGRO_EVENT event;

    int ti = get_task_index(arg);
    wait_for_activation(ti);

    printf("OK: User task activated\n");

    // user input
    while (running)
    {
        counter++;
        event = getEvent();
        if (event.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            switch (event.keyboard.keycode)
            {
            case ALLEGRO_KEY_ESCAPE:
                running = 0;
                printf("OK: User task deactivated\n");
                break;
            case ALLEGRO_KEY_SPACE:
                if (zoomIn == 1){
                    zoomIn = 0;
                }
                break;
            case ALLEGRO_KEY_UP:
                // increase speed
                break;
            case ALLEGRO_KEY_DOWN:
                // decrease speed
                break;
            case ALLEGRO_KEY_LEFT:
                // change lane
                break;
            case ALLEGRO_KEY_RIGHT:
                // change lane
                break;
            }
        }

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            running = 0;
        }

        // check for mouse event
        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
        {
            printf("OK: mouse button down\n");
            // get mouse position
            int x = event.mouse.x;
            int y = event.mouse.y;
            int bitmapX = 0;
            int bitmapY = 0;
            int bitmapWidthX = 0;
            int bitmapHeightY = 0;

            // check if mouse is on veicle
            pthread_mutex_lock(userTaskArg.mutex);

            struct Node *current = userTaskArg.shared->head;
            while (current != NULL)
            {
                // get bitmap position on screen
                bitmapX = (current->Veicle.pos.x)*SCALE_FACTOR;
                bitmapY = (current->Veicle.pos.y)*SCALE_FACTOR;

                bitmapWidthX =  bitmapX + al_get_bitmap_width(getVeicleBitmap(current->Veicle.veicle)); 
                bitmapHeightY = bitmapY + al_get_bitmap_height(getVeicleBitmap(current->Veicle.veicle));

                if(x > bitmapX && x < bitmapWidthX && y > bitmapY && y < bitmapHeightY){
                    
                    // get the veicle id

                    // say to graphics veicle to zoom in


                    break;
                }

                current = current->next;
            }
            pthread_mutex_unlock(userTaskArg.mutex);
        }



        if (counter == 200)
        {
            counter = 0;
            // create a new veicle using ptask
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