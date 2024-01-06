#include "../libs/Label_primitives.h"

// function that render a lable considering a given position and a given text and given dest_buffer
void render_label(int x, int y, char *text, BITMAP *dest_buffer)
{
    // get how text is long
    int text_length = strlen(text);

    int width = (text_length * 8) + 10;
    int height = LABEL_HEIGHT;

    // create a bitmap for the label 
    BITMAP *label = create_bitmap(width, height);

    // draw a rectangle adapted to the text length
    rectfill(label, 0, 0, width, height, BGCOLOR);

    // draw a dark grey frame around the rectangle
    rect(label, 0, 0, width, height, LABEL_FRAME_COLOR);
    rect(label, 1, 1, width - 1, height - 1, LABEL_FRAME_COLOR);
    rect(label, 2, 2, width - 2, height - 2, LABEL_FRAME_COLOR);

    // draw the text on the label
    textout_ex(label, font, text, 3, 4, makecol(0, 0, 0), -1);

    // draw the label on the dest_buffer
    draw_sprite(dest_buffer, label, x, y);
}