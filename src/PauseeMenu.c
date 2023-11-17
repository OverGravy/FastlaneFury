#include "../libs/PauseMenu.h"

// function that init pause menu
void initPauseMenu(){
    // init menu configuration
    menu_init(MY_SCREEN_W, MY_SCREEN_H, MY_SCREEN_W/2, SCREEN_H/2, PAUSE_MENU_COLOR, OPTION_NUM, &pauseMenuConfig);
  
    // init menu options

    // autospwan option
    options[0].pos.x = pauseMenuConfig.RelativeX+10;
    options[0].number = 0;
    options[0].pos.y = pauseMenuConfig.RelativeY+100 + (options[0].number*20);
    options[0].type = CHECK;
    options[0].text = "AutoSpawn";
    options[0].value = getAutoSpawn();
    options[0].function = &setAutoSpawn;

}


// function that draws pause menu
void DrawPauseMenu(){
    // draw box
    menu_draw_box(&pauseMenuConfig, buffer);
    
    // draw title
    menu_draw_title("PAUSE MENU", 10, 10, &pauseMenuConfig, makecol(255, 255, 255), buffer);

    // draw frame
    menu_draw_frame(&pauseMenuConfig, buffer, makecol(255, 255, 255));
    
    // draw options
    menu_draw_option(&options[0], buffer);
   
}


// function that handle menu input when mouse get click
void handleInputPauseMenu(int mouse_x, int mouse_y, int key){
    int i = 0;

    // check for every option if i press there
    for (i;i<OPTION_NUM;i++){
        if (mouse_x > options[i].pos.x && mouse_x < options[i].pos.x+100 && mouse_y > options[i].pos.y && mouse_y < options[i].pos.y+20){
        }
    }
}