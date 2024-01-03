#include "../libs/Assets.h"

// function that load a scaled version of the bitmap
BITMAP *load_scaled_bitmap(char *filename, double factor)
{
    BITMAP *temp = load_bitmap(filename, NULL);
    BITMAP *ret = create_bitmap(temp->w * factor, temp->h * factor);
    stretch_blit(temp, ret, 0, 0, temp->w, temp->h, 0, 0, temp->w * factor, temp->h * factor);
    destroy_bitmap(temp);
    return ret;
}

// function that load all the graphics assets
int load_graphics_assets()
{
    int i;
    // load all cars bitmaps in folder Sprites
    for (i = 0; i <= CAR_NUMBER; i++)
    {
        char path[60];
        sprintf(path, "../Assets/Bitmap/VeicleBitmap/Car/C_bitmap%d.bmp", i);

        // load a scaled version of the bitmap
        Veicles[i] = load_scaled_bitmap(path, VEICLE_SCALE_FACTOR);

        if (!Veicles[i])
        {
            fprintf(stderr, "ERROR: failed to load car bitmap %d!\n", i);
            return 0;
        }
    }

    printf("OK: Loaded all car bitmaps\n");

    // load all trucks bitmaps in folder Sprites
    for (i = 0; i <= TRUCK_NUMBER; i++)
    {
        char path[60];
        sprintf(path, "../Assets/Bitmap/VeicleBitmap/Truck/T_bitmap%d.bmp", i);

        // load a scaled version of the bitmap
        Veicles[i + CAR_NUMBER] = load_scaled_bitmap(path, VEICLE_SCALE_FACTOR);

        if (!Veicles[i + CAR_NUMBER])
        {
            fprintf(stderr, "ERROR: failed to load truck bitmap %d!\n", i);
            return 0;
        }
    }

    printf("OK: Loaded all truck bitmaps\n");

    // load all motorcycles bitmaps in folder Sprites
    for (i = 0; i <= MOTORCYCLE_NUMBER; i++)
    {
        char path[60];
        sprintf(path, "../Assets/Bitmap/VeicleBitmap/Motorcycle/M_bitmap%d.bmp", i);

        // load a scaled version of the bitmap
        Veicles[i + CAR_NUMBER + TRUCK_NUMBER] = load_scaled_bitmap(path, VEICLE_SCALE_FACTOR);

        if (!Veicles[i + CAR_NUMBER + TRUCK_NUMBER])
        {
            fprintf(stderr, "ERROR: failed to load motorcycle bitmap %d!\n", i);
            return 0;
        }
    }

    printf("OK: Loaded all motorcycle bitmaps\n");

    // load all supercars bitmaps in folder Sprites
    for (i = 0; i <= SUPERCAR_NUMBER; i++)
    {
        char path[60];
        sprintf(path, "../Assets/Bitmap/VeicleBitmap/SuperCar/SC_bitmap%d.bmp", i);

        // load a scaled version of the bitmap
        Veicles[i + CAR_NUMBER + TRUCK_NUMBER + MOTORCYCLE_NUMBER] = load_scaled_bitmap(path, VEICLE_SCALE_FACTOR);

        if (!Veicles[i + CAR_NUMBER + TRUCK_NUMBER + MOTORCYCLE_NUMBER])
        {
            fprintf(stderr, "ERROR: failed to load supercar bitmap %d!\n", i);
            return 0;
        }
    }

    return 1;
}

// function that returns the veicle bitmap
BITMAP *get_veicle_bitmap(int type)
{
    return Veicles[type];
}

// function that returns the veicle bitmap height
int get_veicle_height(int type)
{
    return Veicles[type]->h;
}

// function that returns the veicle bitmap width in px
int get_veicle_width(int type)
{
    return Veicles[type]->w;
}

