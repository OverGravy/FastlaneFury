#include "../libs/Config_menu.h"

// function that init pause menu
void init_config_menu()
{
    // init menu configuration
    menu_init(MY_SCREEN_W, MY_SCREEN_H, MY_SCREEN_W / 2, SCREEN_H / 2, CONFIG_MENU_COLOR, OPTION_NUM, &pause_menu_config);

    // init menu options

    // autospwan option
    options[0].pos.x = pause_menu_config.RelativeX + 10;
    options[0].number = 0;
    options[0].pos.y = pause_menu_config.RelativeY + 50 + (options[0].number * 40);
    options[0].text = "AutoSpawn";
    options[0].value = 1;
    options[0].function = &set_auto_spawn;
    options[0].selected = 0;

    // spawn time option 2 second
    options[1].number = 1;
    options[1].pos.x = pause_menu_config.RelativeX + 10 + (options[1].number * 140);
    options[1].pos.y = options[0].pos.y+70; 
    options[1].text = "5 sec";
    options[1].value = 5;
    options[1].function = &set_auto_spawn_time;
    options[1].selected = 1;

    // spawn time option 5 second
    options[2].number = 2;
    options[2].pos.x = pause_menu_config.RelativeX + 10 + (options[2].number * 140);
    options[2].pos.y = options[0].pos.y+70;
    options[2].text = "10 sec";
    options[2].value = 10;
    options[2].function = &set_auto_spawn_time;
    options[2].selected = 0;



}

// function that draws pause menu
void draw_config_menu()
{
    // draw box
    menu_draw_box(&pause_menu_config, buffer);

    // draw title
    menu_draw_title("CONFIG MENU", 10, 10, &pause_menu_config, makecol(255, 255, 255), buffer);

    // draw frame
    menu_draw_frame(&pause_menu_config, buffer, makecol(255, 255, 255));

    // draw options
    menu_draw_option(&options[0], buffer);

    if (options[0].selected == 1)
    {
        menu_draw_text("Spawn Time -->", 10, 120, &pause_menu_config, buffer);
        menu_draw_option(&options[1], buffer);
        menu_draw_option(&options[2], buffer);
    }

    // draw istruction to get out the menu
}

// function that handle menu input when mouse get click
void handle_input_config_menu(int mouse_x, int mouse_y)
{
    int i;

    // check for every option if i press there
    for (i=0; i < OPTION_NUM; i++)
    {
        if (mouse_x > options[i].pos.x + 80 && mouse_x < options[i].pos.x + 120 && mouse_y > options[i].pos.y - 15 && mouse_y < options[i].pos.y + 25)
        {
            // call option function
            options[i].function(options[i].value);
            options[i].selected = options[i].selected == 0 ? 1 : 0;

            if (i == 1)
            {
                options[2].selected = 0;
            }
            else if (i == 2)
            {
                options[1].selected = 0;
            }
        }
    }
}