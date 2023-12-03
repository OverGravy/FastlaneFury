#include "../libs/Draw.h"

// function that load a scaled version of the bitmap
BITMAP *load_scaled_bitmap(char *filename, double factor)
{
    BITMAP *temp = load_bitmap(filename, NULL);
    BITMAP *ret = create_bitmap(temp->w * factor, temp->h * factor);
    stretch_blit(temp, ret, 0, 0, temp->w, temp->h, 0, 0, temp->w * factor, temp->h * factor);
    destroy_bitmap(temp);
    return ret;
}

// function that load all the graphics assets
int load_graphics_assets(){
    int i;
    // load all cars bitmaps in folder Sprites
    for (i = 0; i <= CAR_NUMBER; i++)
    {
        char path[60];
        sprintf(path, "../Assets/Bitmap/VeicleBitmap/Car/C_bitmap%d.bmp", i);

        // load a scaled version of the bitmap
        Veicles[i] = load_scaled_bitmap(path, VEICLE_SCALE_FACTOR);

        if (!Veicles[i])
        {
            fprintf(stderr, "ERROR: failed to load car bitmap %d!\n", i);
            return 0;
        }
    }

    printf("OK: Loaded all car bitmaps\n");

    // load all trucks bitmaps in folder Sprites
    for (i = 0; i <= TRUCK_NUMBER; i++)
    {
        char path[60];
        sprintf(path, "../Assets/Bitmap/VeicleBitmap/Truck/T_bitmap%d.bmp", i);

        // load a scaled version of the bitmap
        Veicles[i+CAR_NUMBER] = load_scaled_bitmap(path, VEICLE_SCALE_FACTOR);

        if (!Veicles[i + CAR_NUMBER])
        {
            fprintf(stderr, "ERROR: failed to load truck bitmap %d!\n", i);
            return 0;
        }
    }

    printf("OK: Loaded all truck bitmaps\n");

    // load all motorcycles bitmaps in folder Sprites
    for (i = 0; i <= MOTORCYCLE_NUMBER; i++)
    {
        char path[60];
        sprintf(path, "../Assets/Bitmap/VeicleBitmap/Motorcycle/M_bitmap%d.bmp", i);

        // load a scaled version of the bitmap
        Veicles[i + CAR_NUMBER + TRUCK_NUMBER] = load_scaled_bitmap(path, VEICLE_SCALE_FACTOR);

        if (!Veicles[i + CAR_NUMBER + TRUCK_NUMBER])
        {
            fprintf(stderr, "ERROR: failed to load motorcycle bitmap %d!\n", i);
            return 0;
        }
    }

    printf("OK: Loaded all motorcycle bitmaps\n");

    // load all supercars bitmaps in folder Sprites
    for (i = 0; i <= SUPERCAR_NUMBER; i++)
    {
        char path[60];
        sprintf(path, "../Assets/Bitmap/VeicleBitmap/SuperCar/SC_bitmap%d.bmp", i);

        // load a scaled version of the bitmap
        Veicles[i + CAR_NUMBER + TRUCK_NUMBER + MOTORCYCLE_NUMBER] = load_scaled_bitmap(path, VEICLE_SCALE_FACTOR);

        if (!Veicles[i + CAR_NUMBER + TRUCK_NUMBER + MOTORCYCLE_NUMBER])
        {
            fprintf(stderr, "ERROR: failed to load supercar bitmap %d!\n", i);
            return 0;
        }
    }

    // create backgroud bitmap
    prerender_background();
    
    return 1;
}


// funtion that clear the display
void clear_display()
{
    clear_to_color(buffer, makecol(0, 0, 0));
}

// function that flip the display
void flip_display()
{
    acquire_screen();
    blit(buffer, screen, 0, 0, 0, 0, MY_SCREEN_W, MY_SCREEN_H);
    release_screen();
}

// fucntion that draws info
void draw_info(pthread_mutex_t *mutex, struct Shared_List *shared)
{
    char info[50];
    double speedKmH = 0;
    int x = 0;
    int y = 0;
    int State;

    // print active veicles
    pthread_mutex_lock(mutex);
    sprintf(info, "Active Veicles: %d", shared->size);
    textout_ex(buffer, font, info, 20, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 20, makecol(255, 255, 255), -1);
    pthread_mutex_unlock(mutex);

    // print dead line missed
    sprintf(info, "Total Deadline Miss: %d", get_deadline_miss());
    textout_ex(buffer, font, info, 20, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 40, makecol(255, 255, 255), -1);

    if (get_selected_veicle() != -1)
    {
        // print info about the task with id
        sprintf(info, "Task: %d", selected_veicle);
        textout_ex(buffer, font, info, 450, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 20, makecol(255, 255, 255), -1);
        sprintf(info, "Period: %d", task_get_period(selected_veicle));
        textout_ex(buffer, font, info, 450, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 40, makecol(255, 255, 255), -1);
        sprintf(info, "Deadline: %d", task_get_deadline(selected_veicle));
        textout_ex(buffer, font, info, 450, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 60, makecol(255, 255, 255), -1);
        sprintf(info, "Priority: %d", task_get_priority(selected_veicle));
        textout_ex(buffer, font, info, 450, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 80, makecol(255, 255, 255), -1);

        // print info about the veicle with id
        pthread_mutex_lock(mutex);
        struct Node *current = shared->head;
        while (current != NULL)
        {
            if (current->id == selected_veicle)
            {
                sprintf(info, "Veicle: %d", current->id);
                textout_ex(buffer, font, info, 650, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 20, makecol(255, 255, 255), -1);
                sprintf(info, "Type: %d", current->Veicle.veicle);
                textout_ex(buffer, font, info, 650, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 40, makecol(255, 255, 255), -1);
                sprintf(info, "Lane: %d", current->Veicle.lane);
                textout_ex(buffer, font, info, 650, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 60, makecol(255, 255, 255), -1);
                // speed in km/h
                if (current->Veicle.state != PAUSE)
                {
                    speedKmH = (int)round(current->Veicle.speed * 3.6);
                    sprintf(info, "Speed: %.2f", speedKmH);
                    textout_ex(buffer, font, info, 650, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 80, makecol(255, 255, 255), -1);
                    sprintf(info, "Acceleration: %.2f", current->Veicle.acceleration);
                    textout_ex(buffer, font, info, 650, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 100, makecol(255, 255, 255), -1);
                    sprintf(info, "Position: (%.2f, %.2f)", current->Veicle.pos.x, current->Veicle.pos.y);
                    textout_ex(buffer, font, info, 650, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 120, makecol(255, 255, 255), -1);
                    sprintf(info, "Steering Angle: %.2f", current->Veicle.steeringAngle);
                    textout_ex(buffer, font, info, 800, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 20, makecol(255, 255, 255), -1);
                    State = current->Veicle.state;
                }
                else
                {
                    // find in support list the veicle with id
                    struct Support_List *temp = get_support_node(current->id);
                    if (temp != NULL)
                    {
                        speedKmH = (int)round(temp->speed * 3.6);
                        sprintf(info, "Speed: %.2f", speedKmH);
                        textout_ex(buffer, font, info, 650, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 80, makecol(255, 255, 255), -1);
                        sprintf(info, "Acceleration: %.2f", temp->acceleration);
                        textout_ex(buffer, font, info, 650, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 100, makecol(255, 255, 255), -1);
                        sprintf(info, "Position: (%.2f, %.2f)", current->Veicle.pos.x, current->Veicle.pos.y);
                        textout_ex(buffer, font, info, 650, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 120, makecol(255, 255, 255), -1);
                        sprintf(info, "Steering Angle: %.2f", current->Veicle.steeringAngle);
                        textout_ex(buffer, font, info, 800, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 20, makecol(255, 255, 255), -1);
                        State = temp->state;
                    }
                }

                switch (State)
                {
                case SLOWDOWN:
                    textout_ex(buffer, font, "State: SLOW_DOWN", 800, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 40, makecol(255, 255, 255), -1);
                    break;

                case ACCELERATE:
                    textout_ex(buffer, font, "State: ACCELERATE", 800, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 40, makecol(255, 255, 255), -1);
                    break;

                case PAUSE:
                    textout_ex(buffer, font, "State: PAUSE", 800, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 40, makecol(255, 255, 255), -1);
                    break;

                case OVERTAKE:
                    textout_ex(buffer, font, "State: OVERTAKE", 800, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 40, makecol(255, 255, 255), -1);
                    break;

                case IDLE:
                    textout_ex(buffer, font, "State: IDLE", 800, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 40, makecol(255, 255, 255), -1);
                    break;

                case ABORTOVERTAKE:
                    textout_ex(buffer, font, "State: ABORT_OVERTAKE", 800, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 40, makecol(255, 255, 255), -1);
                    break;

                }




                // DrawFOV(current->Veicle.pos.x, current->Veicle.pos.y, SMAX, current->Veicle.veicle);

                // Front sensor
                x = (current->Veicle.pos.x) * SCALE_FACTOR - 10;
                y = (current->Veicle.pos.y * SCALE_FACTOR) + (Veicles[current->Veicle.veicle]->h / 2);
                draw_line(x, y, x - RANGE_FRONT, y, FOVCOLOR, buffer);
                draw_point(x, y, SENSORCOLOR, buffer);

                if(current->Veicle.lane != 0){
                    // Right sensor
                    x = (current->Veicle.pos.x * SCALE_FACTOR) + (Veicles[current->Veicle.veicle]->w / 2);
                    y = (current->Veicle.pos.y * SCALE_FACTOR) - 10;
                    draw_arch(x, y, RANGE_SIDE, 190.0, 350.0, FOVCOLOR, buffer);
                    draw_point(x, y, SENSORCOLOR, buffer);
                }

                
                if(current->Veicle.lane != LANE_NUMBER - 1){
                    // Left sensor
                    x = (current->Veicle.pos.x * SCALE_FACTOR) + (Veicles[current->Veicle.veicle]->w  / 2);
                    y = ((current->Veicle.pos.y * SCALE_FACTOR) + Veicles[current->Veicle.veicle]->h) + 10;
                    draw_arch(x, y, RANGE_SIDE, 10.0, 170.0, FOVCOLOR, buffer);
                    draw_point(x, y, SENSORCOLOR, buffer);
                }

                // Back sensor
                x = (((current->Veicle.pos.x) * SCALE_FACTOR) + Veicles[current->Veicle.veicle]->w ) + 10;
                y = (((current->Veicle.pos.y) * SCALE_FACTOR) + (Veicles[current->Veicle.veicle]->h / 2));
                draw_line(x, y, x + RANGE_BACK, y, FOVCOLOR, buffer);
                draw_point(x, y, SENSORCOLOR, buffer);

                break;
            }
            current = current->next;
        }
        pthread_mutex_unlock(mutex);
    }
}

// function that draws veicle
void draw_veicle(double x, double y, int type)
{
    int xg = (int)ceil(x * SCALE_FACTOR); // conversion of x in pixel
    int yg = (int)ceil(y * SCALE_FACTOR); // conversion of y in pixel

    draw_sprite(buffer, Veicles[type], xg, yg);
}

// function that draws background
void draw_background()
{
    blit(background, buffer, 0, 0, 0, 0, MY_SCREEN_W, MY_SCREEN_H);
}

// function that draws mouse
void draw_mouse(int x, int y)
{
    circlefill(buffer, x, y, 5, makecol(255, 0, 0));
}

// function that draws pause if the veicle is paused
void draw_pause_symbol()
{
    circlefill(buffer, 40, SCREEN_H - 40, 30, makecol(255, 255, 255));
    // make the pause sign 5 pixel more hight and in the left
    rectfill(buffer, 30, SCREEN_H - 60, 35, SCREEN_H - 20, makecol(0, 0, 0));
    rectfill(buffer, 45, SCREEN_H - 60, 50, SCREEN_H - 20, makecol(0, 0, 0));
}

// function that returns the veicle bitmap height
int get_veicle_height(int type)
{
    return Veicles[type]->h;
}

// function that returns the veicle bitmap width
int get_veicle_width(int type)
{
    return Veicles[type]->w;
}

// function that returns the pointer to the buffer
BITMAP *get_buffer()
{
    return buffer;
}

// function that returns the pointer to the background
BITMAP *get_background()
{
    return background;
}

// function that clear the zoom screen
void clear_zoom_screen()
{
    clear_to_color(zoom_screen, makecol(0, 0, 0));
}

// function that flips the zoom screen
void flip_zoom_screen()
{
    acquire_screen();

    // center the zoom screen
    int x = (MY_SCREEN_W - zoom_screen->w) / 2;
    int y = (MY_SCREEN_H - zoom_screen->h) / 2;

    blit(zoom_screen, screen, 0, 0, x, y, zoom_screen->w, zoom_screen->h);

    release_screen();
}

// function that draws zoom screen around a selected veicle
void draw_zoom_screen(int x, int y, int type)
{
    // convert car position in pixel
    int xg = (int)ceil(x * SCALE_FACTOR); // conversion of x in pixel
    int yg = (int)ceil(y * SCALE_FACTOR); // conversion of y in pixel

    // cut buffer and insert it in zoom_screen
    zoom_screen = create_sub_bitmap(buffer, xg - 100, yg - 100, 300, 300);

}