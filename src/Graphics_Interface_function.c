#include "../libs/Graphics_Interface_function.h"

// function that render info part of the screen in a destination bitmap
void render_info(struct Shared_List *shared, struct Support_List *support, pthread_mutex_t *list_mutex, pthread_mutex_t *support_mutex, BITMAP *dest_buffer, int selected_veicle)
{

    char info[50];          // string to print info
    double speedKmH = 0;    // speed in km/h          
    int State, x, y;        // integer rappresenting the state of the veicle

    // veiucle state struct
    struct Veicle_State current;
    struct Support_List temp;

    // print active veicles
    pthread_mutex_lock(list_mutex);
    sprintf(info, "Active Veicles: %d", shared->size);
    textout_ex(dest_buffer, font, info, 20, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 20, makecol(255, 255, 255), -1);
    pthread_mutex_unlock(list_mutex);

    // print dead line missed
    sprintf(info, "Total Deadline Miss: %d", get_deadline_miss());
    textout_ex(dest_buffer, font, info, 20, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 40, makecol(255, 255, 255), -1);

    if (selected_veicle != NONE)
    {
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
        current = get_veicle_state(selected_veicle, shared, list_mutex);

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
            textout_ex(dest_buffer, font, info, 900, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 20, makecol(255, 255, 255), -1);
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
            textout_ex(dest_buffer, font, info, 900, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 20, makecol(255, 255, 255), -1);
            State = temp.state;
        }
        switch (State)
        {
        case SLOWDOWN:
            textout_ex(dest_buffer, font, "State: SLOW_DOWN", 900, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 40, makecol(255, 255, 255), -1);
            break;

        case ACCELERATE:
            textout_ex(dest_buffer, font, "State: ACCELERATE", 900, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 40, makecol(255, 255, 255), -1);
            break;

        case PAUSE:
            textout_ex(dest_buffer, font, "State: PAUSE", 900, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 40, makecol(255, 255, 255), -1);
            break;

        case OVERTAKE:
            textout_ex(dest_buffer, font, "State: OVERTAKE", 900, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 40, makecol(255, 255, 255), -1);
            break;

        case IDLE:
            textout_ex(dest_buffer, font, "State: IDLE", 900, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 40, makecol(255, 255, 255), -1);
            break;

        case ABORTOVERTAKE:
            textout_ex(dest_buffer, font, "State: ABORT_OVERTAKE", 900, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 40, makecol(255, 255, 255), -1);
            break;
        }


        // Front sensor
        x = (current.pos.x) * SCALE_FACTOR - 10;
        y = (current.pos.y * SCALE_FACTOR) + (get_veicle_height(current.veicle) / 2);
        draw_line(x, y, x - RANGE_FRONT, y, FOVCOLOR, dest_buffer);
        draw_point(x, y, SENSORCOLOR, dest_buffer);

        if (current.lane != 0)
        {
            // Right sensor
            x = (current.pos.x * SCALE_FACTOR) + (get_veicle_width(current.veicle) / 2);
            y = (current.pos.y * SCALE_FACTOR) - 10;
            draw_arch(x, y, RANGE_SIDE, 190.0, 350.0, FOVCOLOR, dest_buffer);
            draw_point(x, y, SENSORCOLOR, dest_buffer);
        }

        if (current.lane != LANE_NUMBER - 1)
        {
            // Left sensor
            x = (current.pos.x * SCALE_FACTOR) + (get_veicle_width(current.veicle) / 2);
            y = ((current.pos.y * SCALE_FACTOR) + get_veicle_height(current.veicle)) + 10;
            draw_arch(x, y, RANGE_SIDE, 10.0, 170.0, FOVCOLOR, dest_buffer);
            draw_point(x, y, SENSORCOLOR, dest_buffer);
        }

        // Back sensor
        x = (((current.pos.x) * SCALE_FACTOR) + get_veicle_width(current.veicle)) + 10;
        y = (((current.pos.y) * SCALE_FACTOR) + (get_veicle_height(current.veicle) / 2));
        draw_line(x, y, x + RANGE_BACK, y, FOVCOLOR, dest_buffer);
        draw_point(x, y, SENSORCOLOR, dest_buffer);

        // convert car position in pixel
        x = (int)ceil(current.pos.x * SCALE_FACTOR); // conversion of x in pixel
        y = (int)ceil(current.pos.y * SCALE_FACTOR); // conversion of y in pixel
    }
}

// function that render the pause button on the screen
void render_pause_simbol(BITMAP *dest_buffer)
{
    circlefill(dest_buffer, 40, SCREEN_H - 40, 30, makecol(255, 255, 255));
    // make the pause sign 5 pixel more hight and in the left
    rectfill(dest_buffer, 30, SCREEN_H - 60, 35, SCREEN_H - 20, makecol(0, 0, 0));
    rectfill(dest_buffer, 45, SCREEN_H - 60, 50, SCREEN_H - 20, makecol(0, 0, 0));
}

// function that render the mouse on the screen
void render_mouse(BITMAP *dest_buffer)
{
    circlefill(dest_buffer, mouse_x, mouse_y, 5, makecol(255, 0, 0));
}
