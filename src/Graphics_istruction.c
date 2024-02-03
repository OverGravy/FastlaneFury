#include "../libs/Graphics_instruction.h"

// function that render the instruction screen
void render_instruction(BITMAP *dest_buffer, struct argument_struct *arg){

    // calculate the width and height of the instruction part of the screen
    int is_width = MY_SCREEN_W/3;
    int is_height = ((SCREEN_H / (LANE_NUMBER + 1)));
    
    // margin between istructions
    const int margin = LABEL_MARGIN;
    const int label_height = LABEL_HEIGHT; 

    // calculate the x and y of the instruction part of the screen
    int relative_x = (is_width*2);
    int relative_y = is_height*4;

    // draw "Press P to pause/resume"
    if(arg->shared_struct->game_state == PLAY){
        render_label(relative_x + margin, relative_y + margin, "Press P to pause", dest_buffer);
    }else{
        render_label(relative_x + margin, relative_y +margin, "Press P to resume", dest_buffer);
    }

    // draw "Press Z to zoom in/out"
    if(arg->config_struct->zv_scale_factor != Z3_FACTOR){  
        render_label(relative_x + margin, relative_y + (margin*2) + label_height, "Press Z to zoom in", dest_buffer);
    }else{
        render_label(relative_x + margin, relative_y + (margin*2) + label_height , "Press Z to zoom out", dest_buffer);
    }

    // draw "Press A to set automatic spawn"
    if(arg->config_struct->auto_spawn == 0){
        render_label(relative_x + margin, relative_y + (margin*3) + (label_height*2), "Press A to set autospawn", dest_buffer);
    }else{
        render_label(relative_x + margin, relative_y + (margin*3) + (label_height*2), "Press A to set manual spawn", dest_buffer);
    }

    // draw press "press SPACE to spawn a veicle"
    render_label(relative_x + margin + (is_width/2), relative_y + margin, "Press SPACE to spawn a veicle", dest_buffer);

    // draw "Press ESC to exit"
    render_label(relative_x + margin + (is_width/2), relative_y + (margin*2) + label_height, "Press ESC to exit", dest_buffer);
    

}
