#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define bool int
#define TRUE 1
#define FALSE 0

// Stack
struct StackClass {
    char *head;
    int max, top;
};
typedef struct StackClass * Stack;

Stack stack_new(int max) {
    Stack self = malloc(sizeof(struct StackClass));
    self->head = calloc(self->max = max, sizeof(char));
    self->top = 0;
    return self;
} 

char stack_peek(Stack self) { return self->top == 0? '\0': self->head[self->top-1]; }

char stack_pop(Stack self) { return self->top == 0? '\0': self->head[--self->top]; }

bool stack_push(Stack self, char c) {
    if (self->top >= self->max) return FALSE;
    self->head[self->top++] = c;
    return TRUE;
}

void stack_free(Stack self) {
    free(self->head);
    free(self);
}

int get_precedence(char c) {
    switch (c) {
        case '\0': return 0;
        case '+':
        case '-': return 2;
        case '*': 
        case '/':
        case '%': return 3;
        case '(': return 1;
        case ')': return -2;
        default: return -1;
    }
}

int main() {
    char input[100];
    int input_len;
    printf("Enter the expression: ");
    scanf("%s%n", input, &input_len);

    Stack operators = stack_new(64);
    Stack operants = stack_new(32);
    
    char temp_variable = 'z';
    for (int i = 0; i < input_len; i++) {
        char c = input[i];

        int incoming_precedence = get_precedence(c);
        if (incoming_precedence == -1) {
            stack_push(operants, c);
            continue;
        }

        int existing_precedence = get_precedence(stack_peek(operators));

        while(TRUE) {
            if (incoming_precedence == -2) {
                if (stack_peek(operators) == '(') {
                    stack_pop(operators);
                    break;
                }
            } else if (incoming_precedence == 1) break;
            else if (incoming_precedence > existing_precedence) break;

            char result = temp_variable--;
            char op2 = stack_pop(operants);
            char op1 = stack_pop(operants);
            char opp = stack_pop(operators);

            printf("%c = %c %c %c\n", result, op1, opp, op2);
            stack_push(operants, result);
            existing_precedence = get_precedence(stack_peek(operators));
        }

        if (incoming_precedence >= 0) stack_push(operators, c);
    }
    
    while(stack_peek(operators) != '\0') {
        char result = temp_variable--;
        char op2 = stack_pop(operants);
        char op1 = stack_pop(operants);
        char opp = stack_pop(operators);

        printf("%c = %c %c %c\n", result, op1, opp, op2);
        stack_push(operants, result);
    }

    stack_free(operants);
    stack_free(operators);

    return 0;
}