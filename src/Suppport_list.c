#include "../libs/Support_list.h"

// fucntion that create support list 
struct Support_List* create_support_list(){
    struct Support_List* newList = (struct Support_List*)malloc(sizeof(struct Support_List));   // allocate memory for the new list
    newList->next = NULL;                                                                    // set next to null
    return newList;

}

// function that insert info in the support list
void add_vecile_info_to_support(struct Veicle_State *veicle, int index){

    // create new node
    struct Support_List* newNode = (struct Support_List*)malloc(sizeof(struct Support_List));

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
        struct Support_List* last = support->next;
        while(last->next != NULL){
            last = last->next;
        }
        // insert new node
        last->next = newNode;
    }

    pthread_mutex_unlock(&supportMutex);

}

// function that return a specific node in the support list
struct Support_List* get_support_node(int index){

    pthread_mutex_lock(&supportMutex);

    // find node
    struct Support_List* temp = support->next;
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
void clean_support_list(){

    pthread_mutex_lock(&supportMutex);
    // if list is empty return
    if(support->next == NULL){
        pthread_mutex_unlock(&supportMutex);
        return;
    }

    // free all nodes
    struct Support_List* current = support->next;
    while(current != NULL){
        struct Support_List* next = current->next;
        free(current);
        current = next;
    }
    support->next = NULL;
    pthread_mutex_unlock(&supportMutex);

}

// function that destroy support list
void  destroy_support_list(){
    // if list is empty return
    if(support->next == NULL){
        return;
    }

    // free all nodes
    struct Support_List* current = support->next;
    while(current != NULL){
        struct Support_List* next = current->next;
        free(current);
        current = next;
    }
    free(support);
}