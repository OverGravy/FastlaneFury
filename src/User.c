#include "../libs/User.h"

// USER FUNCTIONS

// function that return the index of the selected veicle
int set_selection(int x, int y, pthread_mutex_t *mutex, struct Shared_List *shared)
{
    int selection = -1;

    // 1) check if the mouse is in the screen
    // 2) check if the mouse is on a veicle
    // 3) check if the mouse is on a button

    if (x > 0 && x < MY_SCREEN_W && y > 0 && y < MY_SCREEN_H) // check if mouse is in the screen
    {
        // check if the mouse is on a veicle
        pthread_mutex_lock(mutex);

        // check all the veicles
        struct Node *current = shared->head;
        while (current != NULL)
        {
            int xg = (int)round((current->Veicle.pos.x) * SCALE_FACTOR);
            int yg = (int)round((current->Veicle.pos.y) * SCALE_FACTOR);
            int width = get_veicle_width(current->Veicle.veicle);
            int height = get_veicle_height(current->Veicle.veicle);

            if (x > xg && x < xg + width && y > yg && y < yg + height)
            {
                selection = VEICLE;        
                set_selected_veicle(current->id);
                pthread_mutex_unlock(mutex);
                return selection;                 // if i found what i want i can exit
            }
            current = current->next;
        }

        pthread_mutex_unlock(mutex);

        // check if the mouse is on a button

        // set selected button
    }
    selection = ROAD; // if i don't find anything i return road
    selected_veicle = -1;

    return selection;
}

// function that return the index of the selected veicle
int get_selected_veicle()
{
    return selected_veicle;
}

// function that set the index of the selected veicle
void set_selected_veicle(int id)
{
   selected_veicle = id;
}

// function that return the index of the selected button
int get_selected_button()
{
    return selected_button;
}

// function that set zoomed veicle id
void set_zoomed_veicle(int id)
{
    zoomed_veicle = id;
}

// function that get the index of the zoomed veicle
int get_zoomed_veicle()
{
    return zoomed_veicle;
}

