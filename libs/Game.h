#ifndef __GAME__
#define __GAME__

// define M_PI if not defined
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <allegro.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "Ptask.h"
#include "List.h"
#include "SupportList.h"
#include "VeicleStat.h"
#include "Configuration.h"
#include "ConfigMenu.h"

// allegro costant
#define MY_SCREEN_W 1880
#define MY_SCREEN_H 720
#define SCREEN_FPS 60
#define MAX_FONT 1

// Game costant 
#define MAX_VEICLE_TYPE 158  // max veicle type
#define LANE_NUMBER 4        // number of lane
#define SCALE_FACTOR 15      // scale factor for car for meter to px

// Sensor costant
#define SMAX 200    // max distance in px
#define SMIN 0      // minimun distance in px
#define SSTEP 5     // step in px
#define RANGE 250   // range in px

// color constant
#define BGCOLOR makecol(188, 188, 188)
#define FOVCOLOR makecol(255, 0, 0)
#define CURSORCOLOR makecol(255, 0, 0)
#define LINECOLOR makecol(0, 0, 0)
#define LANECOLOR makecol(255, 255, 255)
#define SENSORCOLOR makecol(0, 255, 0)
#define CONFIG_MENU_COLOR makecol(50, 50, 50)
#define CONFIG_MENU_TEXT_COLOR makecol(255, 255, 255)

// veicle state constant
#define IDLE 0
#define ACCELERATE 1
#define SLOWDOWN 2
#define OVERTAKE 3
#define CRASH 4
#define ABORTOVERTAKE 5
#define PAUSE 6

// Selection constant
#define VEICLE 0
#define BUTTON 1
#define ROAD 2


// Graphics variables
extern BITMAP* buffer;                    // display buffer bitmap
extern BITMAP* background;                // background bitmap
extern BITMAP* Veicles[MAX_VEICLE_TYPE] ; // array of veicles bitmaps
extern FONT* fonts[MAX_FONT];             // array of fonts


// Game Variables
extern int paused[MAX_TASKS];             // array of pause


// User variables
extern int selectedVeicle;                // index of selected veicle
extern int selectedButton;                // index of selected button


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
void DrawInfo(pthread_mutex_t *mutex, struct SharedList *shared);

// function that draws veicle
void DrawVeicle(double x, double y, int type);

// function that draws mouse
void DrawMouse(int x, int y);

// function that draws background in buffer
void DrawBackgroundInBitmap();

// function that draws pause 
void DrawPauseSymbol();

// function that draws point where we need
void DrawPoint(int x, int y, int color);

// function that draws arch where we need
void DrawArch(int x, int y, int radius, double startAngle, double endAngle, int color);

// function that draws line where we need
void DrawLine(int x1, int y1, int x2, int y2, int color);




// VEICLE FUNCTIONS

// function that initialize veicle
void initVeicleState(struct VeicleState *state, struct VeicleStatistics *statistics, int veicle);

// function that returns veicle bitmap
int getVeicleWidth(int veicle);

// function that returns veicle bitmap
int getVeicleHeight(int veicle);

// function that returns distance from other veicle
double proximitySensor(double x, double y, int range, double alpha);


// USER FUNCTIONS

// function that return the selection on the screen and set automatically the id of the selected type of thing
int getSelection(int x, int y, pthread_mutex_t *mutex, struct SharedList *shared);

// function that returns the index of the selected veicle
int getSelectedVeicle();

// function that set the index of the selected veicle
void setSelectedVeicle(int id);

// function that return the index of the selected button
int getSelectedButton();

// function that set the index of the selected button
void setSelectedButton(int id);



// PAUSE AND RESUME FUNCTIONS

// function that pause all the veicles
void pauseVeicles(pthread_mutex_t *mutex, struct SharedList *shared);

// function that pause veicle and open the menu 
void pauseForMenu(pthread_mutex_t *mutex, struct SharedList *shared);

// function that resume all the veicles
void resumeVeicles(pthread_mutex_t *mutex, struct SharedList *shared);

// function that resume veicle and close the menu
void resumeFromMenu(pthread_mutex_t *mutex, struct SharedList *shared);

// function that cheks if a veicle is paused
int checkPause(int id);



#endif