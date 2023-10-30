#include "Graphics.h"

// periodic task function that perioducally updates the screen
void *graphicsTask(void *arg)
{

    int running = 1;
    int Zoom = 0;
    int ZoomId = 0;

    // gets the arguments
    struct argument GraphicsArg = get_task_argument(arg);

    int ti = get_task_index(arg);
    wait_for_activation(ti);

    printf("OK: Graphics task activated\n");
  
    while (running == 1)
    {
        // DRAW SCENE

        // clear display
        clearDisplay();

        // draw background
        DrawBackground();
        
        // draw info 
        DrawInfo(GraphicsArg.mutex,GraphicsArg.shared, 0);

        pthread_mutex_lock(GraphicsArg.mutex);
        // draw veicles
        struct Node *current = GraphicsArg.shared->head;
        while (current != NULL)
        {
            DrawVeicle(current->Veicle.pos.x, current->Veicle.pos.y, current->Veicle.veicle);
            current = current->next;

        }
        pthread_mutex_unlock(GraphicsArg.mutex);

        // draw mouse
        DrawMouse(mouse_x, mouse_y);

        //flip the display
        flipDisplay();

        // check for miss deadline
        if (deadline_miss(ti))
        {
           printf("GRAPHICS: deadline missed\n");
        }

        // wait for next activation
        wait_for_period(ti);
    }

    task_deactivate(ti);
    return NULL;
}
