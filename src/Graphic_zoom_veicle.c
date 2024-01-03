#include "../libs/Graphics_zoom_veicle.h"

// function that render the zoom veicle screen part
void render_zoom_veicle(BITMAP *dest_buffer, BITMAP *scene_surce, int selected_veicle, struct Shared_List *shared, pthread_mutex_t *list_mutex, struct Config *config){

    int x, y, veicle;
    int scale = config->zv_scale_factor * 100;

    // get the veicle position
    x = (get_veicle_state(selected_veicle, shared, list_mutex).pos.x) * SCALE_FACTOR;
    y = (get_veicle_state(selected_veicle, shared, list_mutex).pos.y) * SCALE_FACTOR;
    veicle = get_veicle_state(selected_veicle, shared, list_mutex).veicle;

    // width and height of the cutted scene
    int width = get_veicle_width(veicle) + scale;
    int height = get_veicle_height(veicle) + scale;

    BITMAP *zoom_buffer = create_bitmap(width, height);

    // cut the scene around the veicle and blit it to the zoom buffer
    BITMAP *zoom_veicle = create_bitmap(SCENE_W, SCENE_H);
    clear_to_color(zoom_veicle, makecol(0, 0, 0));
    blit(scene_surce, zoom_buffer, x - (scale / 2), y - (scale / 2), 0, 0, width, height);

    stretch_blit(zoom_buffer, zoom_veicle, 0, 0, width, height, 0, 0, SCENE_W / 3, SCENE_H / 3);

    // draw the zoom veicle buffer centered in the destination buffer
    blit(zoom_veicle, dest_buffer, 0, 0, (SCENE_W) / 2, 0, SCENE_W / 3, SCENE_H / 3);
}

// function that print the info about the veicle in the zoom veicle screen
void render_info_zoom(struct Shared_List *shared, struct Support_List *support, pthread_mutex_t *list_mutex, pthread_mutex_t *support_mutex, BITMAP *dest_buffer, int selected_veicle)
{

    char info[50];
    double speedKmH = 0;
    int x = 0;
    int y = 0;
    int State;

    // print active veicles
    pthread_mutex_lock(list_mutex);
    sprintf(info, "Active Veicles: %d", shared->size);
    textout_ex(dest_buffer, font, info, 20, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 20, makecol(255, 255, 255), -1);
    pthread_mutex_unlock(list_mutex);

    // print dead line missed
    sprintf(info, "Total Deadline Miss: %d", get_deadline_miss());
    textout_ex(dest_buffer, font, info, 20, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 40, makecol(255, 255, 255), -1);

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
    struct Veicle_State current = get_veicle_state(selected_veicle, shared, list_mutex);

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
        struct Support_List temp = get_support_node(support, support_mutex, selected_veicle);

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