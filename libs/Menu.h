#ifndef MENU_H
#define MENU_H
#include <allegro.h>
#include <stdio.h>
#include <stdlib.h>


// struct that contains menu configuration
struct MenuConfig{
    int screen_w;      // screen width
    int screen_h;      // screen height
    int width;         // width of the box
    int height;        // height of the box
    int RelativeX;     // relative x position of the box
    int RelativeY;     // relative y position of the box
    int bg_color;      // background color of the box
    int optionNumber;  // number of options
};

// struct that contains option position
struct optionPos{
    int x;
    int y;
};


// struct that contains option configuration
struct option{
    struct optionPos pos;      // position of the option
    int selected;              // 1 if selected 0 otherwise
    int number;                // option number
    char *text;                // text to display
    int value;                 // value to save if necessary
    void (*function)(int);     // function to call to apply the option
};

// function that init menu configuration
void menu_init(int screen_w, int scrren_h, int width, int height, int bg_color, int optNumber, struct MenuConfig *config);

// function that draw menu
void menu_draw_box(struct MenuConfig *config, BITMAP *buffer);

// function that draws menu title
void menu_draw_title(char *title, int x, int y,struct MenuConfig *config, int color, BITMAP *buffer);   // remeber x, y are relative to the box position

// function that draws menu frame
void menu_draw_frame(struct MenuConfig *config, BITMAP *buffer, int color);

// function that draws menu options
void menu_draw_option(struct option *options, BITMAP *buffer);

// function that draw bit map inside the menu box
void menu_draw_bitmap(BITMAP *bitmap, int x, int y, struct MenuConfig *config, BITMAP *buffer);

// function that draw text inside the menu box
void menu_draw_text(char *text, int x, int y, struct MenuConfig *config, BITMAP *buffer);

#endif