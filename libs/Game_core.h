#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__


// define M_PI if not defined
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ptask costant
#define MAX_TASKS 200
#define MICRO 0
#define MILLI 1
#define ACT 1

// allegro costant
#define MY_SCREEN_W 1880
#define MY_SCREEN_H 720
#define SCREEN_FPS 100

// Game costant 
#define LANE_NUMBER 4             // number of lane
#define SCALE_FACTOR 15           // scale factor for car for meter to px
#define VEICLE_SCALE_FACTOR 0.8   // scale factor for veicle

// Sensor costant
#define SMAX 200                               // max distance in px
#define SMIN 0                                 // minimun distance in px
#define SSTEP 1                                // step in px
#define RANGE_FRONT 200 * VEICLE_SCALE_FACTOR  // range in px
#define RANGE_BACK 50  * VEICLE_SCALE_FACTOR   // range in px
#define RANGE_SIDE 150 * VEICLE_SCALE_FACTOR   // range in px

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

// veicle id constant
#define CAR 0
#define TRUCK 1
#define MOTORCYCLE 2
#define SUPERCAR 3

// veicle number constatnt 
#define CAR_NUMBER 89
#define TRUCK_NUMBER 40
#define MOTORCYCLE_NUMBER 8
#define SUPERCAR_NUMBER 16
#define VEICLE_NUMBER CAR_NUMBER+TRUCK_NUMBER+MOTORCYCLE_NUMBER+SUPERCAR_NUMBER

// Selection constant
#define VEICLE 0
#define BUTTON 1
#define ROAD 2

// config menu constant 
#define OPTION_NUM 3
#define D_AUTO_SPAWN 0
#define D_AUTO_SPAWN_TIME 5
#define NONE -1

#endif