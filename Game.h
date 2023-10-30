#ifndef __GAME__
#define __GAME__
#include <allegro.h>

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "Ptask.h"
#include "List.h"
#include <math.h>

// allegro costant
#define MY_SCREEN_W 1880
#define MY_SCREEN_H 720
#define SCREEN_FPS 60
#define MAX_FONT 1

// Game costant 
#define MAX_VEICLE_TYPE 94
#define LANE_NUMBER 4
#define VEICLE_SCALE 0.8
#define SCALE_FACTOR 15

// Sensor costant
#define SMAX 1000    // max distance in px
#define SMIN 10      // minimun distance in px
#define SSTEP 5      // step in meter

// Selection constant
#define VEICLE 0
#define BUTTON 1
#define ROAD 2


// Game variables
extern BITMAP* buffer;                    // display buffer bitmap
extern BITMAP* background;                // background bitmap
extern BITMAP* cursor;                    // cursor bitmap
extern BITMAP* Veicles[MAX_VEICLE_TYPE] ; // array of veicles bitmaps
extern FONT* fonts[MAX_FONT];             // array of fonts

// User variables
extern int selectedVeicle;
extern int selectedButton;

// ALLEGRO FUNCTIONS    

// funtion that initialize allegro 4.2
int initAllegro();

// close duntion that take reference to everithing needed to close allegro
void closeAllegro();

// load function that takes references of bitmap and font array
int loadGraphicsAssets();


// DRAWING FUNCTIONS

// function that clear the display
void clearDisplay();

// function that flip the display
void flipDisplay();

// function that draws the backgroung
void DrawBackground();

// fucntion that draws info
void DrawInfo(pthread_mutex_t *mutex, struct SharedList *shared, int id);

// function that draws veicle
void DrawVeicle(double x, double y, int type);

// function that draws mouse
void DrawMouse(int x, int y);


// VEICLE FUNCTIONS

// function that initialize veicle
void initVeicleState(struct VeicleState *state);

// function that returns veicle bitmap
int getVeicleWidth(int id);

// function that returns distance from other veicle
double proximitySensor(int id, double x, double y, double range);


// USER FUNCTIONS

// function that returns the index of the selected veicle
int getSelection(int x, int y, pthread_mutex_t *mutex, struct SharedList *shared);

// function that returns the index of the selected veicle
int getSelectedVeicle();

// function that set the index of the selected veicle
void setSelectedVeicle(int id);

// function that return the index of the selected button
int getSelectedButton();

// function that set the index of the selected button
void setSelectedButton(int id);


#endif