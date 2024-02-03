#include "../libs/User.h"

// USER FUNCTIONS

// function that return the index of the selected veicle
int set_selection(int x, int y, struct Shared_List *shared, pthread_mutex_t *shared_struct_mutex, struct shared_struct *shared_struct)
{
    // 1) check if the mouse is in the screen
    // 2) check if the mouse is on a veicle
    // 3) check if the mouse is on a button

    // variable initialization
    int xg, yg, width, height;

    pthread_mutex_lock(shared_struct_mutex);
    if (x > 0 && x < MY_SCREEN_W && y > 0 && y < MY_SCREEN_H) // check if mouse is in the screen
    {
        // check if the mouse is on a veicle
        struct Node *current = shared->head;
        while (current != NULL)
        {
            xg = (int)round((current->Veicle.pos.x) * SCALE_FACTOR);
            yg = (int)round((current->Veicle.pos.y) * SCALE_FACTOR);
            width = get_veicle_width(current->Veicle.veicle);
            height = get_veicle_height(current->Veicle.veicle);

            if (x > xg && x < xg + width && y > yg && y < yg + height)
            {
                // set in shared struct the selection and the selected veicle
                shared_struct->selection = VEICLE;
                shared_struct->selected_veicle = current->id;
                pthread_mutex_unlock(shared_struct_mutex);
                return VEICLE;
            }
            current = current->next;
        }
        pthread_mutex_unlock(shared_struct_mutex);

        // check if the mouse is on a button

        // set selected button

        // if nothing have been selected set selection on road if inside teh screen
        shared_struct->selection = ROAD;
        shared_struct->selected_veicle = NONE;
        shared_struct->mouse_pos.x = x;
        shared_struct->mouse_pos.y = y;
        return ROAD;
    }
    else
    {
        // set selection on none
        shared_struct->selection = NONE;
        return NONE;
    }
}
