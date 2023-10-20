#ifndef __GAME__
#define __GAME__
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include "Ptask.h"
#include "List.h"
#include <math.h>

#define SCREEN_W 1880
#define SCREEN_H 720
#define SCREEN_FPS 60
#define SCALE_FACTOR 15
#define VEICLE_SCALE 0.8
#define LANE_NUMBER 4

#define FOCUS_

#define MAX_VEICLE_TYPE 94
#define MAX_FONT 1

extern ALLEGRO_DISPLAY *display;
extern ALLEGRO_EVENT_QUEUE *event_queue;
extern ALLEGRO_EVENT event;
extern ALLEGRO_TIMER *timer;
extern ALLEGRO_BITMAP *veicleBtm[MAX_VEICLE_TYPE];
extern ALLEGRO_FONT *fonts[MAX_FONT];
extern ALLEGRO_BITMAP *zoomdScreen;
extern int ZoomedId;

// ALLEGRO FUNCTIONS    

// funtion that initialize allegro 
int initAllegro();

// close duntion that take reference to everithing needed to close allegro
void closeAllegro();

// load function that takes references of bitmap and font array
int loadGraphicsAssets();


// DRAWING FUNCTIONS

// function that returns allegro event
ALLEGRO_EVENT getEvent();

// function that clear the display
void clearDisplay();

// function that flip the display
void flipDisplay();

// function that draws the backgroung
void DrawBackground();

// function that get veicle bitmap
ALLEGRO_BITMAP *getVeicleBitmap(int type);

// function that draws the veicle
void DrawVeicle(double x, double y, int type);

// function that draws the info
void DrawInfo(pthread_mutex_t *mutex, struct SharedList *shared, int id);


// ZOOM FUNCTIONS

// function that zoom in a specific car
void ZoomIn(pthread_mutex_t *mutex, struct SharedList *shared, int id);

// function that draw the zoomed screen
void DrawZoomedScreen();

// function that check if there is a zoom
int getZoomId();

// function that return set the zoomId
int setZoomId(int id);

// VEICLE FUNCTIONS

// function that init veicle State
void initVeicleState(struct VeicleState *state);

// function that check if there is some veiche on fornt of the veicle
int ProximitySensor(struct Position *position, int range);

#endif