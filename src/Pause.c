#include "../libs/Pause.h"

// PAUSE AND RESUME FUNCTIONS

// function that pause all the veicles
void pause_veicles(pthread_mutex_t *mutex, struct Shared_List *shared)
{
    clean_support_list();
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
void pause_for_menu(pthread_mutex_t *mutex, struct Shared_List *shared)
{
    clean_support_list();
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
void resume_veicles(pthread_mutex_t *mutex, struct Shared_List *shared)
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
void resume_from_menu(pthread_mutex_t *mutex, struct Shared_List *shared)
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
int check_pause()
{
    return paused[0];
}

// function that checks if menu is open
int check_menu()
{
    return paused[1];
}