#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Object
typedef void * Object;

// String
typedef char * String;

String string_new(String str) {
    String self = calloc(sizeof(char), strlen(str)+1);
    strcpy(self, str);
    return self;
}

String string_new_empty(int size) { return calloc(sizeof(char), size); }

//LinkedList
struct Node {
    Object object;
    struct Node* next;
};

struct LinkedListClass {
    struct Node* head;
    struct Node* tail;
};
typedef struct LinkedListClass * LinkedList;

LinkedList LinkedList_new() {
    LinkedList self = malloc(sizeof(struct LinkedListClass));
    self->head = self->tail = NULL;
    return self;
}

void LinkedList_append(LinkedList self, Object object) {
    struct Node * node = malloc(sizeof(struct Node));
    node->object = object;
    node->next = NULL;
    
    if (self->head == NULL) self->head = self->tail = node;
    else {
        self->tail->next = node;
        self->tail = node;
    }
}

void LinkedList_free(LinkedList self) {
    while(self->head != NULL) {
        struct Node * node = self->head;
        self->head = node->next;
        free(node);
    }
}

struct StateClass {
    LinkedList* heads;
};
typedef struct StateClass * State;

State State_new(int symbol_count) {
    State self = malloc(sizeof(struct StateClass));
    self->heads = calloc(sizeof(LinkedList), symbol_count);
    return self;
}

void State_free(State self) {
    free(self->heads);
    free(self);
}


int main() {
//     int states_size = 0;
//     printf("Enter number of states (> 0): ");
//     scanf("%d", &states_size);

//     State states[states_size];
//     int symbol_count = 0;
//     printf("Enter number of states (> 0): ");
//     scanf("%d", & symbol_count);
    
//     for (int i = 0; i < symbol_count; i++) printf("\t%c", 'a'+i);
//     printf("\n");

//     for (int i = 0; i < states_size; i++) {
//         String name = string_new_empty(2);
//         sprintf(name, "q%d", i);
//         State state = State_new(name, symbol_count);
//         printf("%s\t");
//         for (int i = 0; i < symbol_count; i++) {
//             int next_int;
//             scanf(" q%d ", &next_int);

//             String next;
//             if (next_int == -1) next = NULL:
//             else {
//                 next = string_new_empty(2);
                
//             }
//         }
//     }

    return 0;
}