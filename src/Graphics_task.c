#include "../libs/Graphics_task.h"

// periodic task function that perioducally updates the screen
void *graphics_task(void *arg)
{

    int running = 1;
    struct Node *current;

    // gets the arguments
    struct argument GraphicsArg = get_task_argument(arg);

    int ti = get_task_index(arg);
    wait_for_activation(ti);

    while (running == 1)
    {
        // DRAW SCENE

        // clear the buffer bitmap
        clear_display();

        if (get_zoom_flag())
        {
            clear_zoom_buffer();
        }

        // draw background
        draw_background();

        if (get_selected_veicle() != NONE)
        {
            update_selected_veicle_state();
        }

        // draw info
        draw_info(GraphicsArg.mutex, GraphicsArg.shared);

        // draw veicles
        pthread_mutex_lock(GraphicsArg.mutex);

        if (GraphicsArg.shared->size > 0) // draw veicles only if there are veicles in the list
        {
            current = GraphicsArg.shared->head;
            while (current != NULL)
            {
                draw_veicle(current->Veicle.pos.x, current->Veicle.pos.y, current->Veicle.veicle);
                current = current->next;
            }
        }
        pthread_mutex_unlock(GraphicsArg.mutex);

        // check for pause menu
        if (check_menu())
        {
            draw_config_menu();
        }

        // drawpause sinbol
        if (check_pause())
        {
            draw_pause_symbol();
        }

        // draw mouse
        draw_mouse(mouse_x, mouse_y);

        // flip the display
        if (get_zoom_flag())
        {
            check_zoom();
            draw_zoom(get_selected_veicle());
            flip_zoom_buffer();
        }
        else
        {
            flip_display();
        }

        // check for miss deadline
        if (deadline_miss(ti))
        {
            printf("GRAPHICS: deadline missed\n");
        }

        // wait for next activation
        wait_for_period(ti);
    }
    printf("OK: Graphics task terminated\n");
    return NULL;
}
