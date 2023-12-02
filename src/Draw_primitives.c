#include "../libs/Draw_primitives.h"


// function that draws point where we need
void draw_point(int x, int y, int color, BITMAP *dest)
{
    circlefill(dest, x, y, 5, color);
}

// draw arch
void draw_arch(int x, int y, int radius, double startAngle, double endAngle, int color, BITMAP *dest)
{
    // draw arch using line to fill it
    double angle;
    int px, py;

    // convert angle in rad
    startAngle = (startAngle * M_PI) / 180;
    endAngle = (endAngle * M_PI) / 180;

    for (angle = startAngle; angle <= endAngle; angle += 0.01)
    {
        px = x + radius * cos(angle);
        py = y + radius * sin(angle);
        line(dest, x, y, px, py, color);
    }
}

// function that draws line where we need
void draw_line(int x1, int y1, int x2, int y2, int color, BITMAP *dest)
{
    for (int i = 0; i < 5; i++)
    {
        line(dest, x1, y1 + i, x2, y2 + i, color);
        line(dest, x1, y1 - i, x2, y2 - i, color);
    }
}


