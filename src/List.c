#include "../libs/List.h"

// function that create a new list
struct Shared_List* create_shared_list(){

    struct Shared_List* newList = (struct Shared_List*)malloc(sizeof(struct Shared_List));  // allocate memory for the new list
    newList->head = NULL;                                                                // set head to null
    return newList;

}                                                

// function that insert a veicle in the list
void add_veicle_to_list(int id, struct Veicle_State veicle){


        // create new node
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));

        // set node data
        newNode->id = id;
        newNode->Veicle = veicle;
        // lock mutex
        pthread_mutex_lock(&mutex);

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
        pthread_mutex_unlock(&mutex);
}

// function that remove a veicle from the list 
void remove_veicle_from_list(int id){
    
        // lock mutex
        pthread_mutex_lock(&mutex);  

        shared->size--;

        // if head is the node to remove
        if(shared->head->id == id){
            shared->head = shared->head->next;
            pthread_mutex_unlock(&mutex);
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
        pthread_mutex_unlock(&mutex);
    
}


// function that set veicle state in the list
void set_veicle_state(int id, struct Veicle_State State){

    // lock mutex
    pthread_mutex_lock(&mutex);

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
    pthread_mutex_unlock(&mutex);

}

// a function that destroy the list
void destroy_shared_list(){

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

// function that return list size
int get_list_size(){
    return shared->size;
}