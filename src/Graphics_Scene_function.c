#include "../libs/Graphics_Scene_function.h"

// function that init the backgroung
void prerender_background(BITMAP *background)
{
    rectfill(background, 0, 0, SCENE_W, SCENE_H, makecol(188, 188, 188));     // road

    // draw dashed line
    int x1 = 0;
    int y1 = ((MY_SCREEN_H / (LANE_NUMBER + 1)));
    int x2 = SCENE_W;
    int y2 = ((MY_SCREEN_H / (LANE_NUMBER + 1)));

    int draw = 1;
    int segment_length = 20; // segment length
    int space_length = 20;   // space length

    while (x1 < x2)
    {
        if (draw)
        {
            line(background, x1, y1, x1 + segment_length, y2, LANECOLOR);
            line(background, x1, y1 * 2, x1 + segment_length, y2 * 2, LANECOLOR);
            line(background, x1, y1 * 3, x1 + segment_length, y2 * 3, LANECOLOR);
        }
        x1 += segment_length;
        x1 += space_length;
        draw = !draw;
    }

    char lane[3];
    for (int i = 0; i < 4; i++)
    {
        sprintf(lane, "%d", i);
        textout_ex(background, font, lane, 30, ((MY_SCREEN_H / (LANE_NUMBER + 1))) * i + 25, makecol(0, 0, 0), -1);
    }
}

void clear_scene_buffer(BITMAP *dest_buffer)
{
    rectfill(dest_buffer, 0, 0, SCENE_W, SCENE_H, makecol(188, 188, 188)); // road
}

// function that draws veicle
void render_veicle(double x, double y, BITMAP *veicle, BITMAP *dest_buffer)
{
    int xg = (int)ceil(x * SCALE_FACTOR); // conversion of x in pixel
    int yg = (int)ceil(y * SCALE_FACTOR); // conversion of y in pixel

    draw_sprite(dest_buffer, veicle, xg, yg);
}

// function that draws background
void render_background(BITMAP *dest_buffer, BITMAP *bg)
{
    blit(bg, dest_buffer, 0, 0, 0, 0, SCENE_W, SCENE_H);
}
