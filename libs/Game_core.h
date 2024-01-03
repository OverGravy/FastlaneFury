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
#define SCREEN_FPS 60

// scene costant
#define SCENE_H (MY_SCREEN_H / (LANE_NUMBER + 1)) * LANE_NUMBER
#define SCENE_W MY_SCREEN_W

// User task constant (in millisecond)
#define UT_PERIOD 15
#define UT_DEADLINE 15
#define UT_PRIORITY 90

// Graphic task constant (in millisecond)
#define GT_PERIOD (int)round((double)1000 / SCREEN_FPS)
#define GT_DEADLINE (int)round((double)1000 / SCREEN_FPS) +1
#define GT_PRIORITY 80

// Veicle task constant (in millisecond)
#define VT_PERIOD 20
#define VT_DEADLINE 20
#define VT_PRIORITY 30

// Game costant 
#define LANE_NUMBER 4             // number of lane
#define SCALE_FACTOR 15           // scale factor for car for meter to px
#define VEICLE_SCALE_FACTOR 0.6   // scale factor for veicle

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

// game state constant
#define PAUSE_G 1
#define PLAY 0

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
#define VEICLE_NUMBER 144

// Selection constant
#define VEICLE 0
#define BUTTON 1
#define ROAD 2

#define NONE -1

// constat id for buffer 
#define MAIN_SCENE 0
#define ZOOM_SCENE 1
#define ZOOM_VEICLE 2

// configuration constant
#define AUTO 1              // auto spawn veicle active
#define MANUAL 0            // manual spawn veicle active
#define AS_T1 2             // autospawn time of 2 second
#define AS_T2 3             // autospawn time of 3 second
#define AS_T3 4             // autospawn time of 4 second

// zoom factor constant
#define Z1_FACTOR 4         // zoom factor 1
#define Z2_FACTOR 3         // zoom factor 2
#define Z3_FACTOR 2         // zoom factor 3

#endif