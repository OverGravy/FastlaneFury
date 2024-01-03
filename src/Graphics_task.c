#include "../libs/Graphics_task.h"

// periodic task function that perioducally updates the screen
void *graphics_task(void *arg)
{

    int running = 1;
    struct Node *current;

    // gets the arguments
    struct argument_struct GraphicsArg = get_task_argument(arg);

    int ti = get_task_index(arg);
    wait_for_activation(ti);

    // realize and init all the bitmap needed
    BITMAP *screen_buffer = create_bitmap(MY_SCREEN_W, MY_SCREEN_H);
    BITMAP *scene_buffer = create_bitmap(SCENE_W, SCENE_H);
    BITMAP *bg = create_bitmap(SCENE_W, SCENE_H);
    prerender_background(bg);

    printf("OK: Graphics task activated\n");
    reset_deadline(ti);

    while (running == 1)
    {
        // DRAW SCENE

        // clear the buffer bitmap
        clear_scene_buffer(scene_buffer);

        // draw background
        render_background(scene_buffer, bg);

        // draw veicles
        if (GraphicsArg.shared_list->size > 0)
        {
            pthread_mutex_lock(GraphicsArg.shared_list_mutex);

            if (GraphicsArg.shared_list->size > 0) // draw veicles only if there are veicles in the list
            {
                current = GraphicsArg.shared_list->head;
                while (current != NULL)
                {
                    render_veicle(current->Veicle.pos.x, current->Veicle.pos.y, get_veicle_bitmap(current->Veicle.veicle), scene_buffer);
                    current = current->next;
                }
            }
            pthread_mutex_unlock(GraphicsArg.shared_list_mutex);
        }
        // blit the scene buffer to the shared buffer
        blit(scene_buffer, GraphicsArg.scene, 0, 0, 0, 0, SCENE_W, SCENE_H);

        // DRAW SCREEN
        clear_to_color(screen_buffer, makecol(0, 0, 0));
        switch (GraphicsArg.shared_struct->buffer_id)
        {
        case MAIN_SCENE:
            // draw the scene buffer to the screen buffer
            blit(scene_buffer, screen_buffer, 0, 0, 0, 0, SCENE_W, SCENE_H);
            // draw the info part of the screen
            render_info(GraphicsArg.shared_list, GraphicsArg.support_list, GraphicsArg.shared_list_mutex, GraphicsArg.support_list_mutex, screen_buffer, GraphicsArg.shared_struct->selected_veicle);
            // draw the pause simbol if the game is paused
            if (GraphicsArg.shared_struct->game_state == PAUSE)
            {
                render_pause_simbol(screen_buffer);
            }
            // draw the menu if the game is in config state

            render_mouse(screen_buffer);
            break;

        case ZOOM_SCENE:
            printf("ZOOM\n");
            break;

        case ZOOM_VEICLE:
            // render in scene space the zoom veicle
            render_zoom_veicle(screen_buffer, GraphicsArg.scene, GraphicsArg.shared_struct->selected_veicle, GraphicsArg.shared_list, GraphicsArg.shared_list_mutex, GraphicsArg.config_struct);
            printf("ZOOM VEICLE\n");
            // render the info part of the screen
            render_info_zoom(GraphicsArg.shared_list, GraphicsArg.support_list, GraphicsArg.shared_list_mutex, GraphicsArg.support_list_mutex, screen_buffer, GraphicsArg.shared_struct->selected_veicle);

            break;
        }

        // blit the screen buffer to the screen
        blit(screen_buffer, screen, 0, 0, 0, 0, MY_SCREEN_W, MY_SCREEN_H);

        // check for miss deadline
        if (deadline_miss(ti))
        {
             printf("ERROR: graphics_task deadline missed\n");
        }

        // wait for next activation
        wait_for_period(ti);
    }
    printf("OK: Graphics task terminated\n");
    return NULL;
}
