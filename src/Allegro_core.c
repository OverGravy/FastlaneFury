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

    return 1;
}



// function to close allegro
void close_allegro()
{

    // unistall display
    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);

    // Uninstall the mouse
    remove_mouse();

    // Uninstall the keyboard
    remove_keyboard();

    // Exit program
    allegro_exit();
}
