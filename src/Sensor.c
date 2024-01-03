#include "../libs/Sensor.h"

// function that returns the distance from the other veicle
double proximity_sensor(double x, double y, int range, double alpha, BITMAP *scene)
{

    // the sensor return the distance from the other veicle in meter and obtain position in pixel and range in pixel

    double distance = -1.0;
    int i = 0;
    int color;

    // convert alpha in rad
    alpha = (alpha * M_PI) / 180;

    // check if the range is in the limit
    if (range > SMAX)
    {
        range = SMAX;
    }

    // check for veicle in front
    for (i = SMIN; i < range; i += SSTEP)
    {
        color = getpixel(scene, x + (i * cos(alpha)), y + (i * sin(alpha)));                                                                                                               // get color
        if (color != BGCOLOR && color != -1 && color != FOVCOLOR && color != CURSORCOLOR && color != LINECOLOR && color != LANECOLOR && color != SENSORCOLOR && color != CONFIG_MENU_COLOR) // if there is a veicle
        {
            distance = (double)i / SCALE_FACTOR; // distance in meter
            break;
        }
    }
    return distance;
}