#include "Game.h"

// function that initialize allegro
int initAllegro()
{
    // initialize allegro
    if (!al_init())
    {
        fprintf(stderr, "ERROR: failed to initialize allegro!\n");
        return 0;
    }
    // initialize primitives addon
    if (!al_init_primitives_addon())
    {
        fprintf(stderr, "ERROR: failed to initialize primitives!\n");
        return 0;
    }
    // initialize image addon
    if (!al_init_image_addon())
    {
        fprintf(stderr, "ERROR: failed to initialize image addon!\n");
        return 0;
    }
    // initialize font addon
    if (!al_init_font_addon())
    {
        fprintf(stderr, "ERROR: failed to initialize font addon!\n");
        return 0;
    }
    // initialize ttf addon
    if (!al_init_ttf_addon())
    {
        fprintf(stderr, "ERROR: failed to initialize ttf addon!\n");
        return 0;
    }
    // create and check display
    display = al_create_display(SCREEN_W, SCREEN_H);

    // set window title
    al_set_window_title(display, "Highway: Traffic Simulator");

    // make target buffer
    if (!display)
    {
        fprintf(stderr, "ERROR: failed to create display!\n");
        return 0;
    }
    // create and check timer
    timer = al_create_timer(1.0 / SCREEN_FPS);
    if (!timer)
    {
        fprintf(stderr, "ERROR: failed to create timer!\n");
        al_destroy_display(display);
        return 0;
    }
    // create and check event queue
    event_queue = al_create_event_queue();
    if (!event_queue)
    {
        fprintf(stderr, "ERROR: failed to create event_queue!\n");
        al_destroy_timer(timer);
        al_destroy_display(display);
        return 0;
    }

    // install keyboard
    if (!al_install_keyboard())
    {
        fprintf(stderr, "ERROR: failed to initialize the keyboard!\n");
        al_destroy_timer(timer);
        al_destroy_event_queue(event_queue);
        return 0;
    }
    // install mouse
    if (!al_install_mouse())
    {
        fprintf(stderr, "ERROR: failed to initialize the mouse!\n");
        al_destroy_timer(timer);
        al_destroy_event_queue(event_queue);
        return 0;
    }

    // register event sources
    al_register_event_source(event_queue, al_get_display_event_source(display)); // register display from event queue
    al_register_event_source(event_queue, al_get_timer_event_source(timer));     // register timer from event queue
    al_register_event_source(event_queue, al_get_keyboard_event_source());       // register keyboard from event queue
    al_register_event_source(event_queue, al_get_mouse_event_source());          // register mouse from event queue

    return 1;
}

// function that returns allegro event
ALLEGRO_EVENT getEvent()
{
    // get event from event queue
    al_get_next_event(event_queue, &event);
    return event;
}

// function that clear the display
void clearDisplay()
{
    al_set_target_backbuffer(display);
    al_clear_to_color(al_map_rgb(0, 0, 0));
}

// function that flip the display
void flipDisplay()
{
    al_set_target_backbuffer(display);
    al_flip_display();
}

// function that draws the backgroung
void DrawBackground()
{
    al_set_target_backbuffer(display);

    al_draw_filled_rectangle(0, 0, SCREEN_W, (SCREEN_H / (LANE_NUMBER + 1)) * 4, al_map_rgb(188, 188, 188));  // road
    al_draw_filled_rectangle(0, (SCREEN_H / (LANE_NUMBER + 1)) * 4, SCREEN_W, SCREEN_H, al_map_rgb(0, 0, 0)); // terminal

    // draw dashed line
    int x1 = 0;
    int y1 = ((SCREEN_H / (LANE_NUMBER + 1)));
    int x2 = SCREEN_W;
    int y2 = ((SCREEN_H / (LANE_NUMBER + 1)));

    bool draw = true;
    int segment_length = 20; // segment length
    int space_length = 20;   // space length

    while (x1 < x2)
    {
        if (draw)
        {
            al_draw_line(x1, y1, x1 + segment_length, y2, al_map_rgb(255, 255, 255), 5);
            al_draw_line(x1, y1 * 2, x1 + segment_length, y2 * 2, al_map_rgb(255, 255, 255), 5);
            al_draw_line(x1, y1 * 3, x1 + segment_length, y2 * 3, al_map_rgb(255, 255, 255), 5);
        }
        x1 += segment_length;
        x1 += space_length;
        draw = !draw;
    }

    char lane[3];
    for (int i = 0; i < 4; i++)
    {
        sprintf(lane, "%d", i);
        al_draw_text(fonts[0], al_map_rgb(0, 0, 0), 30, ((SCREEN_H / (LANE_NUMBER + 1))) * i + 25, 0, lane);
    }
}

// function that draws info on the screen
void DrawInfo(pthread_mutex_t *mutex, struct SharedList *shared, int id)
{
    char info[50];
    al_set_target_backbuffer(display);

    // print active veicles
    pthread_mutex_lock(mutex);
    sprintf(info, "Active Veicles: %d", shared->size);
    al_draw_text(fonts[0], al_map_rgb(255, 255, 255), 20, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 20, 0, info);
    pthread_mutex_unlock(mutex);

    // print dead line missed
    sprintf(info, "Total Deadline Miss: %d", get_deadline_miss());
    al_draw_text(fonts[0], al_map_rgb(255, 255, 255), 20, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 40, 0, info);

    // print info about the task with id
     sprintf(info, "Task: %d", id);
     al_draw_text(fonts[0], al_map_rgb(255, 255, 255), 450, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 20, 0, info);
     sprintf(info, "Period: %d", task_get_period(id));
     al_draw_text(fonts[0], al_map_rgb(255, 255, 255), 450, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 40, 0, info);
     sprintf(info, "Deadline: %d", task_get_deadline(id));
     al_draw_text(fonts[0], al_map_rgb(255, 255, 255), 450, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 60, 0, info);
     sprintf(info, "Priority: %d", task_get_priority(id));
     al_draw_text(fonts[0], al_map_rgb(255, 255, 255), 450, ((SCREEN_H / (LANE_NUMBER + 1))) * 4 + 80, 0, info);
}

// function that draws the veicle
void DrawVeicle(double x, double y, int type)
{
    al_set_target_backbuffer(display);

    int xg = (int)round(x*SCALE_FACTOR);
    int yg = (int)round(y*SCALE_FACTOR);

    // calculate new width and height
    int new_width = al_get_bitmap_width(veicleBtm[type]) * VEICLE_SCALE;
    int new_height = al_get_bitmap_height(veicleBtm[type]) * VEICLE_SCALE;
    
    al_draw_scaled_bitmap(veicleBtm[type], 0, 0, al_get_bitmap_width(veicleBtm[type]), al_get_bitmap_height(veicleBtm[type]), xg,yg, new_width, new_height, 0);
}

// function that zoom in a specific car
void ZoomIn(pthread_mutex_t *mutex, struct SharedList *shared, int id){
    // find the right car in the screen
    pthread_mutex_lock(mutex);

    // search for right car in the list
    struct Node *current = shared->head;

    while (current != NULL)
    {
        if (current->id == id)
        {
            // create sub bitmap around the car
            int x = (int)round(current->Veicle.pos.x*SCALE_FACTOR);
            break;
        }
        current = current->next;
    }

    pthread_mutex_unlock(mutex);



}

// fucntion that return veicle bitmap
ALLEGRO_BITMAP *getVeicleBitmap(int type)
{
    return veicleBtm[type];
}

// function that closes allegro
void closeAllegro()
{
    // destroy display
    al_destroy_display(display);
    // destroy timer
    al_destroy_timer(timer);
    // destroy event queue
    al_destroy_event_queue(event_queue);
    // unistall keyboard
    al_uninstall_keyboard();
    // unistall mouse
    al_uninstall_mouse();
}

// function that loads graphics assets
int loadGraphicsAssets()
{
    // load all cars bitmaps in folder Sprites
    for (int i = 0; i < MAX_VEICLE_TYPE; i++)
    {
        char path[50];
        sprintf(path, "./Bitmap/veicle%d.png", i);
        veicleBtm[i] = al_load_bitmap(path);
        if (!veicleBtm[i])
        {
            fprintf(stderr, "ERROR: failed to load car bitmap %d!\n", i);
        }
    }
    printf("OK: Loaded all veicle bitmaps\n");
    // load background bitmap
    // background = al_load_bitmap("./Bitmap/background.png");

    // load all fonts in folder Fonts
    for (int i = 0; i < MAX_FONT; i++)
    {
        char path[50];
        sprintf(path, "./Fonts/font%d.ttf", i);
        fonts[i] = al_load_ttf_font(path, 15, 0);
        if (!fonts[i])
        {
            fprintf(stderr, "ERROR: failed to load font%d!\n ", i);
            return 0;
        }
    }
    printf("OK: Loaded all fonts\n");
    return 1;
}

// function that init veicle state
void initVeicleState(struct VeicleState *state){
    state->veicle = rand() % MAX_VEICLE_TYPE;
    state->lane = 1;
    state->pos.x = (SCREEN_W-2)/SCALE_FACTOR;    // in meter
    int margin = ((SCREEN_H / (LANE_NUMBER + 1)) - (al_get_bitmap_height(veicleBtm[state->veicle])*VEICLE_SCALE))/2; // margin in pixel
    state->pos.y =(((SCREEN_H/(LANE_NUMBER+1))*state->lane) + margin)/SCALE_FACTOR; // in meter
    state->speed = 10.0; // speed in ms
    state->acceleration = 0.0; // acceleration in ms^2
}

// function that act as a sensor check if there is some veicle on front of the veicle
int ProximitySensor(struct Position *position, int range){
     int distance = 0;

    ALLEGRO_COLOR initialColor = al_map_rgb(188, 188, 188); // Gray color
    printf("hey");

    // Check each pixel in the range moving towards the left
    for (int i = (position->x)*SCALE_FACTOR - 2; i > ((position->x)*SCALE_FACTOR) - range; i--) {
        ALLEGRO_COLOR color = al_get_pixel(al_get_backbuffer(display), i, (position->y*SCALE_FACTOR)-50);
        // Compare the color with the initial gray color
        if (color.r != initialColor.r || color.g != initialColor.g || color.b != initialColor.b) {
            // Color is different, return the distance
            printf("distance: %d\n", distance);
            return distance;
        }

        // Increment the distance
        distance++; 
    }

    // If no different color is found in the range, return -1 to indicate no object found
    return -1;
}