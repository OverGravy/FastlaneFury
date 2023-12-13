#include "../libs/Zoom_veicle.h"

// function that clear the zoom screen
void clear_zoom_buffer()
{
    zoom_buffer = create_bitmap(MY_SCREEN_W, MY_SCREEN_H);
    clear_to_color(zoom_buffer, makecol(0, 0, 0));
}

// function that flips the zoom screen
void flip_zoom_buffer()
{
    acquire_screen();
    blit(zoom_buffer, screen, 0, 0, 0, 0, MY_SCREEN_W, MY_SCREEN_H);
    release_screen();
}

// function that catch the veicle to zoom
void draw_zoom(int veicle)
{
    int x = 0;
    int y = 0;
    BITMAP *zoom;

    // search the veicle in the list
    struct Veicle_State *current = get_selected_veicle_state();

    if (current->lane == 0)
    {
        zoom = create_sub_bitmap(buffer, (current->pos.x * SCALE_FACTOR) - 200, 0, get_veicle_width(veicle) + 400, get_veicle_height(veicle) + 200);
    }
    else if (current->lane == LANE_NUMBER - 1)
    {
        zoom = create_sub_bitmap(buffer, (current->pos.x * SCALE_FACTOR) - 200, (MY_SCREEN_H / (LANE_NUMBER + 1))* current->lane - get_veicle_width(veicle) - 100, get_veicle_width(veicle), get_veicle_height(veicle)+100);
    }
    else
    {
        zoom = create_sub_bitmap(buffer, (current->pos.x * SCALE_FACTOR) - 200, (current->pos.y * SCALE_FACTOR) - 150, get_veicle_width(veicle) + 400, get_veicle_height(veicle) + 400);
    }
    // put zoom_veicle in the center of the zoom_buffer
    x = (MY_SCREEN_W / 2) - (zoom->w / 2);
    y = (MY_SCREEN_H / 2) - (zoom->h / 2);

    blit(zoom, zoom_buffer, 0, 0, x, y, zoom->w, zoom->h);
}

// function that check if the veicle is still in the screen
void check_zoom()
{
    struct Veicle_State *current = get_selected_veicle_state();

    if(current->pos.x < 0){
        set_zoom_flag();
    }
}