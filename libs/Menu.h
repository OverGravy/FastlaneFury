#ifndef MENU_H
#define MENU_H

#include <allegro.h>
#include <stdio.h>
#include <stdlib.h>

// struct that contains menu configuration
struct Menu_Config{
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
struct Option_Pos{
    int x;
    int y;
};


// struct that contains option configuration
struct Option{
    struct Option_Pos pos;      // position of the option
    int selected;              // 1 if selected 0 otherwise
    int number;                // option number
    char *text;                // text to display
    int value;                 // value to save if necessary
    void (*function)(int);     // function to call to apply the option
};

// function that init menu configuration
void menu_init(int screen_w, int screen_h, int width, int height, int bg_color, int optNumber, struct Menu_Config *config);

// function that draw menu
void menu_draw_box(struct Menu_Config *config, BITMAP *buffer);

// function that draws menu title
void menu_draw_title(char *title, int x, int y,struct Menu_Config *config, int color, BITMAP *buffer);   // remeber x, y are relative to the box position

// function that draws menu frame
void menu_draw_frame(struct Menu_Config *config, BITMAP *buffer, int color);

// function that draws menu options
void menu_draw_option(struct Option *options, BITMAP *buffer);

// function that draw bit map inside the menu box
void menu_draw_bitmap(BITMAP *bitmap, int x, int y, struct Menu_Config *config, BITMAP *buffer);

// function that draw text inside the menu box
void menu_draw_text(char *text, int x, int y, struct Menu_Config *config, BITMAP *buffer);

#endif