#include "../libs/Graphics_zoom_veicle.h"

// function that render the zoom veicle screen part
void render_zoom_veicle(BITMAP *dest_buffer, BITMAP *scene_surce, int selected_veicle, struct Shared_List *shared, pthread_mutex_t *list_mutex, struct Config *config)
{
    // Position of veicle and veicle type
    int x, y, veicle;                          // coordinates of the veicle and veicle number
    int scale = config->zv_scale_factor * 100; // scale factor

    // width and height of the cutted scene
    int width;
    int height;

    // calcultate ma new max width and height to fit in the scene and mantain the same aspect ratio
    int n_height = SCENE_H;
    int n_width;

    // set variables
    x = (get_veicle_state(selected_veicle, shared, list_mutex).pos.x) * SCALE_FACTOR;
    y = (get_veicle_state(selected_veicle, shared, list_mutex).pos.y) * SCALE_FACTOR;
    veicle = get_veicle_state(selected_veicle, shared, list_mutex).veicle;
    width = get_veicle_width(veicle) + scale;
    height = get_veicle_height(veicle) + scale;
    n_width = (int)round(((double)width / (double)height) * (double)n_height);

    // create a buffer with the cutted scene
    BITMAP *zoom_veicle = create_bitmap(width, height);

    // create a buffer with the cutted scene
    BITMAP *zoom_buffer = create_bitmap(n_width, n_height);

    if( x - (scale / 2) > 0)
        x = x - (scale / 2);
    else
        x = 0;
    
    if ( y - (scale / 2) > 0)
        y = y - (scale / 2);
    else
        y = 0;

    // blit the scene in the buffer
    blit(scene_surce, zoom_veicle, x, y, 0, 0, width, height);
    
    // stretch the buffer to fit in the scene
    stretch_blit(zoom_veicle, zoom_buffer, 0, 0, width, height, 0, 0, n_width, n_height);

    // blit the buffer in the middle of the scene
    blit(zoom_buffer, dest_buffer, 0, 0, (SCENE_W / 2) - (n_width / 2), 0, n_width, n_height);
    
}

// function that print the info about the veicle in the zoom veicle screen
void render_info_zoom(struct Shared_List *shared, struct Support_List *support, pthread_mutex_t *list_mutex, pthread_mutex_t *support_mutex, BITMAP *dest_buffer, int selected_veicle)
{
    char info[50];          // string to print info
    double speedKmH = 0;    // speed in km/h
    int State;              // integer rappresenting the state of the veicle

    // get the veicle state
    struct Veicle_State current;
    struct Support_List temp;

    // print active veicles
    sprintf(info, "Active Veicles: %d", shared->size);
    textout_ex(dest_buffer, font, info, 20, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 20, makecol(255, 255, 255), -1);

    // print dead line missed
    sprintf(info, "Total Deadline Miss: %d", get_deadline_miss());
    textout_ex(dest_buffer, font, info, 20, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 40, makecol(255, 255, 255), -1);

    current = get_veicle_state(selected_veicle, shared, list_mutex);

    // print info about the task with id
    sprintf(info, "Task: %d", selected_veicle);
    textout_ex(dest_buffer, font, info, 450, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 20, makecol(255, 255, 255), -1);
    sprintf(info, "Period: %d", task_get_period(selected_veicle));
    textout_ex(dest_buffer, font, info, 450, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 40, makecol(255, 255, 255), -1);
    sprintf(info, "Deadline: %d", task_get_deadline(selected_veicle));
    textout_ex(dest_buffer, font, info, 450, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 60, makecol(255, 255, 255), -1);
    sprintf(info, "Priority: %d", task_get_priority(selected_veicle));
    textout_ex(dest_buffer, font, info, 450, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 80, makecol(255, 255, 255), -1);

   
    // print info about the veicle with id
    sprintf(info, "Veicle: %d", selected_veicle);
    textout_ex(dest_buffer, font, info, 650, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 20, makecol(255, 255, 255), -1);
    sprintf(info, "Type: %d", current.veicle);
    textout_ex(dest_buffer, font, info, 650, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 40, makecol(255, 255, 255), -1);
    sprintf(info, "Lane: %d", current.lane);
    textout_ex(dest_buffer, font, info, 650, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 60, makecol(255, 255, 255), -1);
    // speed in km/h
    if (current.state != PAUSE)
    {
        speedKmH = (int)round(current.speed * 3.6);
        sprintf(info, "Speed: %.2f", speedKmH);
        textout_ex(dest_buffer, font, info, 650, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 80, makecol(255, 255, 255), -1);
        sprintf(info, "Acceleration: %.2f", current.acceleration);
        textout_ex(dest_buffer, font, info, 650, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 100, makecol(255, 255, 255), -1);
        sprintf(info, "Position: (%.2f, %.2f)", current.pos.x, current.pos.y);
        textout_ex(dest_buffer, font, info, 650, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 120, makecol(255, 255, 255), -1);
        sprintf(info, "Steering Angle: %.2f", current.steeringAngle);
        textout_ex(dest_buffer, font, info, 800, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 20, makecol(255, 255, 255), -1);
        State = current.state;
    }
    else
    {
        // find in support list the veicle with id
        temp = get_support_node(support, support_mutex, selected_veicle);

        speedKmH = (int)round(temp.speed * 3.6);
        sprintf(info, "Speed: %.2f", speedKmH);
        textout_ex(dest_buffer, font, info, 650, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 80, makecol(255, 255, 255), -1);
        sprintf(info, "Acceleration: %.2f", temp.acceleration);
        textout_ex(dest_buffer, font, info, 650, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 100, makecol(255, 255, 255), -1);
        sprintf(info, "Position: (%.2f, %.2f)", current.pos.x, current.pos.y);
        textout_ex(dest_buffer, font, info, 650, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 120, makecol(255, 255, 255), -1);
        sprintf(info, "Steering Angle: %.2f", current.steeringAngle);
        textout_ex(dest_buffer, font, info, 800, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 20, makecol(255, 255, 255), -1);
        State = temp.state;
    }
    switch (State)
    {
    case SLOWDOWN:
        textout_ex(dest_buffer, font, "State: SLOW_DOWN", 800, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 40, makecol(255, 255, 255), -1);
        break;

    case ACCELERATE:
        textout_ex(dest_buffer, font, "State: ACCELERATE", 800, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 40, makecol(255, 255, 255), -1);
        break;

    case PAUSE:
        textout_ex(dest_buffer, font, "State: PAUSE", 800, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 40, makecol(255, 255, 255), -1);
        break;

    case OVERTAKE:
        textout_ex(dest_buffer, font, "State: OVERTAKE", 800, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 40, makecol(255, 255, 255), -1);
        break;

    case IDLE:
        textout_ex(dest_buffer, font, "State: IDLE", 800, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 40, makecol(255, 255, 255), -1);
        break;

    case ABORTOVERTAKE:
        textout_ex(dest_buffer, font, "State: ABORT_OVERTAKE", 800, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 40, makecol(255, 255, 255), -1);
        break;
    }
}