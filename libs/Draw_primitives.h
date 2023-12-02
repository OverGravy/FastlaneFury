#ifndef __DRAW_PRIMITIVES_H__
#define __DRAW_PRIMITIVES_H__

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <allegro.h>
#include <math.h>

// function that draws point where we need
void draw_point(int x, int y, int color, BITMAP *dest);

// function that draws arch where we need
void draw_arch(int x, int y, int radius, double startAngle, double endAngle, int color, BITMAP *dest);

// function that draws line where we need
void draw_line(int x1, int y1, int x2, int y2, int color, BITMAP *dest);

#endif