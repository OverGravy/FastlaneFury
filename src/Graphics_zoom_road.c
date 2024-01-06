#include "../libs/Graphics_zoom_road.h"

// function that render the zoom road peace on screen_buffer
void render_zoom_road(BITMAP *dest_buffer, BITMAP *scene_surce, struct position *pos, struct Config *config)
{
    int x, y;
    int scale = config->zv_scale_factor * 100;

    x = pos->x;
    y = pos->y;

    // width and height of the cutted scene
    int width = 500 + scale;
    int height = 300 + scale;

    // create a buffer with the cutted scene
    BITMAP *zoom_road = create_bitmap(width, height);

    if (x - (scale / 2) > 0)
        x = x - (scale / 2);
    else
        x = 0;

    if (y - (scale / 2) > 0)
        y = y - (scale / 2);
    else
        y = 0;

    blit(scene_surce, zoom_road, x, y, 0, 0, width, height);

    // calcultate ma new max width and height to fit in the scene and mantain the same aspect ratio
    int n_height = SCENE_H;
    int n_width = (int)round(((double)width / (double)height) * (double)n_height);

    // create a buffer with the zoom veicle scaled to fit in the scene
    BITMAP *zoom_buffer = create_bitmap(n_width, n_height);
    stretch_blit(zoom_road, zoom_buffer, 0, 0, width, height, 0, 0, n_width, n_height);

    // blit the buffer in the middle of the scene
    blit(zoom_buffer, dest_buffer, 0, 0, (SCENE_W / 2) - (n_width / 2), 0, n_width, n_height);
}

void render_info_zoom_road(struct Shared_List *shared, BITMAP *dest_buffer)
{
    char info[50];

    // print active veicles
    sprintf(info, "Active Veicles: %d", shared->size);
    textout_ex(dest_buffer, font, info, 20, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 20, makecol(255, 255, 255), -1);

    // print dead line missed
    sprintf(info, "Total Deadline Miss: %d", get_deadline_miss());
    textout_ex(dest_buffer, font, info, 20, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 40, makecol(255, 255, 255), -1);
}