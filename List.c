#include "List.h"

// function that create a new list
struct SharedList* createSharedList(){

    struct SharedList* newList = (struct SharedList*)malloc(sizeof(struct SharedList));  // allocate memory for the new list
    newList->head = NULL;                                                                // set head to null
    return newList;

}                                                

// function that insert a veicle in the list
void addVeicleToList(struct SharedList *shared, pthread_mutex_t *mutex, int id, struct VeicleState veicle){

        // create new node
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    
        // set node data
        newNode->id = id;
        newNode->Veicle = veicle;
        // lock mutex
        pthread_mutex_lock(mutex);

        shared->size++;

        // if list is empty
        if(shared->head == NULL){
            shared->head = newNode;
        }
        else{
            // find last node
            struct Node* last = shared->head;
            while(last->next != NULL){
                last = last->next;
            }
    
            // insert new node
            last->next = newNode;
        }
    
        // unlock mutex
        pthread_mutex_unlock(mutex);
}

// function that remove a veicle from the list 
void removeVeicleFromList(struct SharedList *shared, pthread_mutex_t *mutex, int id){
    
        // lock mutex
        pthread_mutex_lock(mutex);  

        shared->size--;

        // if head is the node to remove
        if(shared->head->id == id){
            shared->head = shared->head->next;
            pthread_mutex_unlock(mutex);
            return;
        }
    
        // find node to remove
        struct Node* current = shared->head;
        while(current->next != NULL){
            if(current->next->id == id){
                break;
            }
            current = current->next;
        }
    
        // remove node
        current->next = current->next->next;
    
        // unlock mutex
        pthread_mutex_unlock(mutex);
    
}


// function that set veicle state in the list
void setVeicleState(struct SharedList *shared, pthread_mutex_t *mutex, int id, struct VeicleState State){

    // lock mutex
    pthread_mutex_lock(mutex);

    // search the correct node
    struct Node* current = shared->head;
    while(current->next != NULL){
        if(current->id == id){
            break;
        }
        current = current->next;
    }

    // set node position
    current->Veicle = State;

    // unlock mutex  
    pthread_mutex_unlock(mutex);

}

// a function that destroy the list
void destroySharedList(struct SharedList *shared){

    // free all nodes
    struct Node* current = shared->head;
    while(current != NULL){
        struct Node* next = current->next;
        free(current);
        current = next;
    }

    // free list
    free(shared);

}