#include "../libs/Support_list.h"

// fucntion that create support list 
struct supportList* createSupportList(){
    struct supportList* newList = (struct supportList*)malloc(sizeof(struct supportList));   // allocate memory for the new list
    newList->next = NULL;                                                                    // set next to null
    return newList;

}

// function that insert info in the support list
void addVecileInfoToSupport(struct VeicleState *veicle, int index){

    // create new node
    struct supportList* newNode = (struct supportList*)malloc(sizeof(struct supportList));

    pthread_mutex_lock(&supportMutex);

    // set node data
    newNode->state = veicle->state;
    newNode->id = index;
    newNode->acceleration = veicle->acceleration;
    newNode->speed = veicle->speed;

    // if list is empty
    if(support->next == NULL){
        support->next = newNode;
    }
    else{
        // find last node
        struct supportList* last = support->next;
        while(last->next != NULL){
            last = last->next;
        }
        // insert new node
        last->next = newNode;
    }

    pthread_mutex_unlock(&supportMutex);

}

// function that return a specific node in the support list
struct supportList* getSupportNode(int index){

    pthread_mutex_lock(&supportMutex);

    // find node
    struct supportList* temp = support->next;
    while (temp != NULL)
    {
        if (temp->id == index)
        {
            break;
        }
        temp = temp->next;
        
    }
    pthread_mutex_unlock(&supportMutex);

    return temp;
}

// function that clean all element in support list
void cleanSupportList(){

    pthread_mutex_lock(&supportMutex);
    // if list is empty return
    if(support->next == NULL){
        pthread_mutex_unlock(&supportMutex);
        return;
    }

    // free all nodes
    struct supportList* current = support->next;
    while(current != NULL){
        struct supportList* next = current->next;
        free(current);
        current = next;
    }
    support->next = NULL;
    pthread_mutex_unlock(&supportMutex);

}

// function that destroy support list
void destroySupportList(){
    // if list is empty return
    if(support->next == NULL){
        return;
    }

    // free all nodes
    struct supportList* current = support->next;
    while(current != NULL){
        struct supportList* next = current->next;
        free(current);
        current = next;
    }
    free(support);
}