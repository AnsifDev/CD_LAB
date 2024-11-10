#include <stdio.h>
#include <string.h>
#include <stdbool.h>

char operators[32][8];
char operants[32][8];

int operators_top = 0;
int operants_top = 0;
int temp_counter = 0;


int get_precedence(char* token) {
    if (token == NULL) return -2;
    if (strlen(token) != 1) return 0;

    switch (token[0]) {
        case '+':
        case '-': return 2;
        case '*': 
        case '/':
        case '%': return 3;
        case '(': return 1;
        case ')': return -1;
        default: return 0;
    }
}
void push_operator(char* operator) {
    int incoming_precedence = get_precedence(operator);
    
    while (incoming_precedence < 0 || incoming_precedence <= get_precedence(operators[operators_top-1])) {
        char temp[8], op1[8], op2[8], opp[8];
        
        strcpy(opp, operators[--operators_top]); // Pop operator
        if (incoming_precedence == -1 && strcmp(opp, "(") == 0) break;
        
        strcpy(op2, operants[--operants_top]); // Pop operant 2
        strcpy(op1, operants[--operants_top]); // Pop operant 1
        sprintf(temp, "t%d", temp_counter++);

        printf("%s = %s %s %s\n", temp, op1, opp, op2);

        strcpy(operants[operants_top++], temp); // Push temp result
    }

    if (incoming_precedence >= 0) 
        strcpy(operators[operators_top++], operator);
}

int main() {
    printf("Enter the expression: ");

    char result[8], token[8];
    if (scanf(" %[a-zA-Z0-9]", result) != 1) return 1; // Need a result variable;
    if (scanf(" %1[=]", token) != 1) return 1;
    
    strcpy(operators[operators_top++], "("); // Push ( initially

    while(true) {
        while (scanf(" %1[(]", token) == 1) // For all '('
            strcpy(operators[operators_top++], token);

        if (scanf(" %[a-zA-Z0-9]", token) != 1) return 1;
        strcpy(operants[operants_top++], token);

        while (scanf(" %1[)]", token) == 1) // For all ')'
            push_operator(token);

        if (scanf(" %1[+-*/%%]", token) != 1) 
            if (scanf(" %1[;]", token) != 1) return false;
            else break;
        push_operator(token);
    }

    push_operator(")");
    printf("%s = t%d\n", result, temp_counter-1);

    return 0;
}