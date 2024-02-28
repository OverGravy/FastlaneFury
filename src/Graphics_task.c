#include "../libs/Graphics_task.h"


/*
screen division in 2 part, the scene have its own dedicated buffer
_________________________________
|_______________________________|
|_________   SCENE   ___________|
|_______________________________
|_______________________________|
|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|
|\\\\\\\      INFO     \\\\\\\\\|
---------------------------------

*/

// periodic task function that perioducally updates the screen
void *graphics_task(void *arg)
{

    int running = 1;        // flag to check if the task is running
    struct Node *current;   // current node of the list

    // gets the arguments
    struct argument_struct GraphicsArg = get_task_argument(arg);

    int ti = get_task_index(arg);   // get the task index
    wait_for_activation(ti);

    // realize and init all the bitmap needed
    BITMAP *screen_buffer = create_bitmap(MY_SCREEN_W, MY_SCREEN_H);      // screen buffer
    BITMAP *scene_buffer = create_bitmap(SCENE_W, SCENE_H);               // scene buffer
    BITMAP *bg = create_bitmap(SCENE_W, SCENE_H);                         // background buffer
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

        // draw spawn lane point
        render_spawn_lane(scene_buffer, *(GraphicsArg.LastLane));

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
            break;

        case ZOOM_SCENE:
            // render in scene space the zoom scene
            render_zoom_road(screen_buffer, GraphicsArg.scene, &GraphicsArg.shared_struct->mouse_pos, GraphicsArg.config_struct);

            // render the info part of the screen
            render_info_zoom_road(GraphicsArg.shared_list, screen_buffer);
            break;

        case ZOOM_VEICLE:
            // render in scene space the zoom veicle
            render_zoom_veicle(screen_buffer, GraphicsArg.scene, GraphicsArg.shared_struct->selected_veicle, GraphicsArg.shared_list, GraphicsArg.shared_list_mutex, GraphicsArg.config_struct);
            // render the info part of the screen
            render_info_zoom(GraphicsArg.shared_list, GraphicsArg.support_list, GraphicsArg.shared_list_mutex, GraphicsArg.support_list_mutex, screen_buffer, GraphicsArg.shared_struct->selected_veicle);

            break;
        }
        // render mouse position
        render_mouse(screen_buffer);

        // render the instruction part of the screen
        render_instruction(screen_buffer, &GraphicsArg);

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
