#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Boolean
typedef int bool;
#define TRUE 1
#define FALSE 0

// Object
typedef void * Object;

// Stack
struct StackClass { Object* head; int top; int max; };
typedef struct StackClass * Stack;

Stack stack_new(int max) {
    Stack self = malloc(sizeof(struct StackClass));
    self->head = calloc(sizeof(Object), self->max = max);
    self->top = 0;
    return self;
}

bool stack_push(Stack self, Object value) {
    if (self->top >= self->max) return FALSE;
    self->head[self->top++] = value;
    return TRUE;
}

Object stack_pop(Stack self) {
    if (self->top == 0) return NULL;
    return self->head[--self->top];
}

Object stack_peek(Stack self) {
    if (self->top == 0) return NULL;
    return self->head[self->top-1];
}

bool stack_is_empty(Stack self) { return self->top == 0; }

void stack_free(Stack self) {
    free(self->head);
    free(self);
}

// String
typedef char * String;

String string_new(String str) {
    String self = calloc(sizeof(char), strlen(str)+1);
    strcpy(self, str);
    return self;
}

String string_new_empty(int size) { return calloc(sizeof(char), size); }

// Character
String char_to_string(char self) {
    String string = string_new_empty(2);
    string[0] = self;
    return string;
}

//Itm Generation Program
#define PRECEDENCE_UNKNOWN 0
#define PRECEDENCE_OPEN_PARANTHESIS 1
#define PRECEDENCE_CLOSE_PARANTHESIS -1

int get_precedence(String string) {
    if (string == NULL || string[1] != '\0') return PRECEDENCE_UNKNOWN;

    switch (string[0]) {
        case '*':
        case '/':
        case '%': return 3;
        case '+':
        case '-': return 2;
        case '(': return PRECEDENCE_OPEN_PARANTHESIS;
        case ')': return PRECEDENCE_CLOSE_PARANTHESIS;
        default: return PRECEDENCE_UNKNOWN;
    }
}

int counter = 0;
void evaluate(Stack operants, char operator) {
    
}

int main() {
    int input_len = 0;
    String input = string_new_empty(100);
    printf("Enter the input expr: ");
    scanf("%s%n", input, &input_len);

    Stack operators = stack_new(25);
    Stack operants = stack_new(30);

    stack_push(operators, string_new("("));
    for (int i = 0; i < input_len+1; i++) {
        String character = char_to_string(input[i] != '\0'? input[i]: ')');

        int incoming_precedence = get_precedence(character);
        if (incoming_precedence == PRECEDENCE_UNKNOWN) { stack_push(operants, character); continue; } // If not operator

        int existing_precedence = get_precedence(stack_peek(operators));
        bool closed_paren = incoming_precedence == PRECEDENCE_CLOSE_PARANTHESIS;
        if (incoming_precedence != PRECEDENCE_OPEN_PARANTHESIS) while (closed_paren || existing_precedence >= incoming_precedence) {
            String operator_str = stack_pop(operators);
        
            if (operator_str == NULL) return 1;
            char operator = operator_str[0];
            existing_precedence = get_precedence(stack_peek(operators));
            free(operator_str);

            if (operator == '(') break;

            // Pop it and evaluate on the go
            String op2 = stack_pop(operants);
            String op1 = stack_pop(operants);
            String result = string_new_empty(5);

            sprintf(result, "t%d", counter++);
            printf("%s = %s %c %s\n", result, op1, operator, op2);

            stack_push(operants, result);
            free(op1);
            free(op2);
        }

        if (!closed_paren) stack_push(operators, character);
    }

    stack_free(operators);
    stack_free(operants);

    return 0;
}