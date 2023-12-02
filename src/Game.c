#include "../libs/Game.h"

// ALLEGRO FUNCTIONS

// function that initialize allegro 4.2
int init_allegro()
{
    // Initialize Allegro
    if (allegro_init() != 0)
    {
        allegro_message("Failed to initialize Allegro!");
        return 0;
    }

    // set the window title when in a GUI environment
    set_window_title("FastLaneFury");

    // true color mode
    set_color_depth(32); // Set the color depth

    // Set up a graphics mode
    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, MY_SCREEN_W, MY_SCREEN_H, 0, 0) != 0)
    {
        allegro_message("Failed to set graphics mode!");
        return 0;
    }

    printf("OK: Allegro initialized\n");

    // Install the keyboard
    if (install_keyboard() < 0)
    {
        allegro_message("Failed to install keyboard!");
        return 0;
    }

    // Install the mouse
    if (install_mouse() < 0)
    {
        allegro_message("Failed to install mouse!");
        return 0;
    }

    printf("OK: Keyboard and mouse installed\n");

    // This bitmap will be used as the virtual screen
    buffer = create_bitmap(MY_SCREEN_W, MY_SCREEN_H);

    // init bitmap
    clear_to_color(buffer, makecol(0, 0, 0)); // Clear to black

    // init screen bitmap
    clear_to_color(screen, makecol(0, 0, 0)); // Clear to black

    background = create_bitmap(MY_SCREEN_W, MY_SCREEN_H);

    return 1;
}

// function that init the backgroung
void prerender_background()
{
    rectfill(background, 0, 0, MY_SCREEN_W, (MY_SCREEN_H / (LANE_NUMBER + 1)) * 4, makecol(188, 188, 188));     // road
    rectfill(background, 0, (MY_SCREEN_H / (LANE_NUMBER + 1)) * 4, MY_SCREEN_W, MY_SCREEN_H, makecol(0, 0, 0)); // terminal

    // draw dashed line
    int x1 = 0;
    int y1 = ((MY_SCREEN_H / (LANE_NUMBER + 1)));
    int x2 = MY_SCREEN_W;
    int y2 = ((MY_SCREEN_H / (LANE_NUMBER + 1)));

    int draw = 1;
    int segment_length = 20; // segment length
    int space_length = 20;   // space length

    while (x1 < x2)
    {
        if (draw)
        {
            line(background, x1, y1, x1 + segment_length, y2, LANECOLOR);
            line(background, x1, y1 * 2, x1 + segment_length, y2 * 2, LANECOLOR);
            line(background, x1, y1 * 3, x1 + segment_length, y2 * 3, LANECOLOR);
        }
        x1 += segment_length;
        x1 += space_length;
        draw = !draw;
    }

    char lane[3];
    for (int i = 0; i < 4; i++)
    {
        sprintf(lane, "%d", i);
        textout_ex(background, font, lane, 30, ((MY_SCREEN_H / (LANE_NUMBER + 1))) * i + 25, makecol(0, 0, 0), -1);
    }
}

// function to close allegro
void close_allegro()
{
    // Destroy the buffer bitmap
    destroy_bitmap(buffer);

    // Destroy the background bitmap
    destroy_bitmap(background);

    // destroy all veicles bitmaps
    for (int i = 0; i < CAR_NUMBER + TRUCK_NUMBER + MOTORCYCLE_NUMBER + SUPERCAR_NUMBER; i++)
    {
        destroy_bitmap(Veicles[i]);
    }

    // unistall display
    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);

    // Uninstall the mouse
    remove_mouse();

    // Uninstall the keyboard
    remove_keyboard();

    // destroy mutex
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&supportMutex);

    // destroy shared list
    destroySharedList();

    // destroy support list
    destroySupportList();

    // Exit program
    allegro_exit();
}



// VEICLE FUNCTIONS

// function that return the bitmap width of a veicle
int getVeicleWidth(int veicle)
{
    return Veicles[veicle]->w;
}

// function that return the bitmap height of a veicle
int getVeicleHeight(int veicle)
{
    return Veicles[veicle]->h;
}

// function that initialize veicle
void initVeicleState(struct VeicleState *state, struct VeicleStatistics *statistics)
{
    int margin = 0;
    state->veicle = rand() % VEICLE_NUMBER; // random veicle

    // find the veicle type 
    if(state->veicle >= 0 && state->veicle <= CAR_NUMBER){
        GetVeicleStaitistics(CAR, statistics);
    }else if(state->veicle > CAR_NUMBER && state->veicle <= CAR_NUMBER + TRUCK_NUMBER){
        GetVeicleStaitistics(TRUCK, statistics);
    }else if(state->veicle > CAR_NUMBER + TRUCK_NUMBER && state->veicle <= CAR_NUMBER + TRUCK_NUMBER + MOTORCYCLE_NUMBER){
        GetVeicleStaitistics(MOTORCYCLE, statistics);
    }else if(state->veicle > CAR_NUMBER + TRUCK_NUMBER + MOTORCYCLE_NUMBER && state->veicle <= CAR_NUMBER + TRUCK_NUMBER + MOTORCYCLE_NUMBER + SUPERCAR_NUMBER){
        GetVeicleStaitistics(SUPERCAR, statistics);
    }


    state->speed = 30.0;                                                                        // in m/s
    state->lane = rand() % LANE_NUMBER;
    state->pos.x = (MY_SCREEN_W - 2) / SCALE_FACTOR;                                            // in meter
    margin = ((MY_SCREEN_H / (LANE_NUMBER + 1)) - ((Veicles[state->veicle]->h))) / 2;           // margin in pixel
    state->pos.y = (((MY_SCREEN_H / (LANE_NUMBER + 1)) * state->lane) + margin) / SCALE_FACTOR; // in meter
    state->steeringAngle = 0.0;                                                                 // in degree
    state->acceleration = 0;                                                                    // in m/s^2
   
}

// function that returns the distance from the other veicle
double proximitySensor(double x, double y, int range, double alpha)
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
        color = getpixel(screen, x + (i * cos(alpha)), y + (i * sin(alpha)));                                                                                                               // get color
        if (color != BGCOLOR && color != -1 && color != FOVCOLOR && color != CURSORCOLOR && color != LINECOLOR && color != LANECOLOR && color != SENSORCOLOR && color != CONFIG_MENU_COLOR) // if there is a veicle
        {
            distance = (double)i / SCALE_FACTOR; // distance in meter
            break;
        }
    }
    return distance;
}



// USER FUNCTIONS

// function that return the index of the selected veicle
int getSelection(int x, int y, pthread_mutex_t *mutex, struct SharedList *shared)
{
    int selection = -1;

    // 1) check if the mouse is in the screen
    // 2) check if the mouse is on a veicle
    // 3) check if the mouse is on a button

    if (x > 0 && x < MY_SCREEN_W && y > 0 && y < MY_SCREEN_H) // check if mouse is in the screen
    {
        // check if the mouse is on a veicle
        pthread_mutex_lock(mutex);

        // check all the veicles
        struct Node *current = shared->head;
        while (current != NULL)
        {
            int xg = (int)round((current->Veicle.pos.x) * SCALE_FACTOR);
            int yg = (int)round((current->Veicle.pos.y) * SCALE_FACTOR);
            int width = ((Veicles[current->Veicle.veicle]->w));
            int height = (Veicles[current->Veicle.veicle]->h);

            if (x > xg && x < xg + width && y > yg && y < yg + height)
            {
                selection = VEICLE;        
                setSelectedVeicle(current->id);
                pthread_mutex_unlock(mutex);
                return selection;                 // if i found what i want i can exit
            }
            current = current->next;
        }

        pthread_mutex_unlock(mutex);

        // check if the mouse is on a button

        // set selected button
    }

    selection = ROAD; // if i don't find anything i return road

    return selection;
}

// function that return the index of the selected veicle
int getSelectedVeicle()
{
    return selectedVeicle;
}

// function that set the index of the selected veicle
void setSelectedVeicle(int id)
{
    selectedVeicle = id;
}

// function that return the index of the selected button
int getSelectedButton()
{
    return selectedButton;
}



// PAUSE AND RESUME FUNCTIONS

// function that pause all the veicles
void pauseVeicles(pthread_mutex_t *mutex, struct SharedList *shared)
{
    cleanSupportList();
    paused[0] = 1; // to inform grapfic task to draw pause icon
    pthread_mutex_lock(mutex);
    struct Node *current = shared->head;
    while (current != NULL)
    {
        paused[current->id] = 1; // set pause to 1
        printf("OK: Veicle %d paused\n", current->id);
        current = current->next;
    }
    pthread_mutex_unlock(mutex);
}

// function that pause and open the menu
void pauseForMenu(pthread_mutex_t *mutex, struct SharedList *shared)
{
    cleanSupportList();
    paused[0] = 1; // to inform grapfic task to draw pause icon
    paused[1] = 1; // to inform grapfic task to draw menu
    pthread_mutex_lock(mutex);
    struct Node *current = shared->head;
    while (current != NULL)
    {
        paused[current->id] = 1; // set pause to 1
        printf("OK: Veicle %d paused\n", current->id);
        current = current->next;
    }
    pthread_mutex_unlock(mutex);
}

// function that resume all the veicles
void resumeVeicles(pthread_mutex_t *mutex, struct SharedList *shared)
{
    pthread_mutex_lock(mutex);
    paused[0] = 0;
    struct Node *current = shared->head; // current node
    while (current != NULL)
    {
        paused[current->id] = 0; // set pause to 0
        current = current->next;
    }
    pthread_mutex_unlock(mutex);
}

// function that resume and close the menu
void resumeFromMenu(pthread_mutex_t *mutex, struct SharedList *shared)
{
    pthread_mutex_lock(mutex);
    paused[0] = 0;
    paused[1] = 0;
    struct Node *current = shared->head; // current node
    while (current != NULL)
    {
        paused[current->id] = 0; // set pause to 0
        current = current->next;
    }
    pthread_mutex_unlock(mutex);
}

// function that check if the veicle is paused
int checkPause()
{
    return paused[0];
}

// function that checks if menu is open
int checkMenu()
{
    return paused[1];
}