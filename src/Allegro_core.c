#include "../libs/Allegro_core.h"

// function that initialize allegro 4.2
int init_allegro()
{
    // Initialize Allegro
    if (allegro_init() != 0)
    {
        allegro_message("Failed to initialize Allegro!");
        return 0;
    }

    // set the window title when in a GUI environment
    set_window_title("FastLaneFury");

    // true color mode
    set_color_depth(32); // Set the color depth

    // Set up a graphics mode
    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, MY_SCREEN_W, MY_SCREEN_H, 0, 0) != 0)
    {
        allegro_message("Failed to set graphics mode!");
        return 0;
    }

    printf("OK: Allegro initialized\n");

    // Install the keyboard
    if (install_keyboard() < 0)
    {
        allegro_message("Failed to install keyboard!");
        return 0;
    }

    // Install the mouse
    if (install_mouse() < 0)
    {
        allegro_message("Failed to install mouse!");
        return 0;
    }

    printf("OK: Keyboard and mouse installed\n");

    // This bitmap will be used as the virtual screen
    buffer = create_bitmap(MY_SCREEN_W, MY_SCREEN_H);

    // init bitmap
    clear_to_color(buffer, makecol(0, 0, 0)); // Clear to black

    // init screen bitmap
    clear_to_color(screen, makecol(0, 0, 0)); // Clear to black

    background = create_bitmap(MY_SCREEN_W, MY_SCREEN_H);
    
    zoom_buffer = create_bitmap(MY_SCREEN_W, MY_SCREEN_H);

    return 1;
}

// function that init the backgroung
void prerender_background()
{
    rectfill(background, 0, 0, MY_SCREEN_W, (MY_SCREEN_H / (LANE_NUMBER + 1)) * 4, makecol(188, 188, 188));     // road
    rectfill(background, 0, (MY_SCREEN_H / (LANE_NUMBER + 1)) * 4, MY_SCREEN_W, MY_SCREEN_H, makecol(0, 0, 0)); // terminal

    // draw dashed line
    int x1 = 0;
    int y1 = ((MY_SCREEN_H / (LANE_NUMBER + 1)));
    int x2 = MY_SCREEN_W;
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

// function to close allegro
void close_allegro()
{
    // Destroy the buffer bitmap
    destroy_bitmap(buffer);

    // Destroy the background bitmap
    destroy_bitmap(background);

    // destroy all veicles bitmaps
    for (int i = 0; i < CAR_NUMBER + TRUCK_NUMBER + MOTORCYCLE_NUMBER + SUPERCAR_NUMBER; i++)
    {
        destroy_bitmap(Veicles[i]);
    }

    // unistall display
    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);

    // Uninstall the mouse
    remove_mouse();

    // Uninstall the keyboard
    remove_keyboard();

    // destroy mutex
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&supportMutex);

    // destroy shared list
    destroy_shared_list();

    // destroy support list
     destroy_support_list();

    // Exit program
    allegro_exit();
}
