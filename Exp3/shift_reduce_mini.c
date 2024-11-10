#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    char left;
    char production[100];
    int pr_len;
} Rule;

Rule grammer[100];
int rules = 0;

bool load_grammer() {
    FILE *gmr_file = fopen("grammer.gmr", "r");
    if (gmr_file == NULL) return false;

    printf("Loading rules:\n");
    while(!feof(gmr_file)) {
        Rule *rule = &grammer[rules++];

        int rt = fscanf(gmr_file, " %c -> %s ", &rule->left, rule->production);
        if (rt != 2) {
            printf("ERR: Rule parsing failed\n");
            return false;
        }
        
        rule->pr_len = strlen(rule->production);
        printf("Rule Loaded: %c -> %s\n", rule->left, rule->production);
    }

    return true;
}

char stack[100] = "";
int pointer = 0, stack_top = 0;

void stack_push(char c) {
    stack[stack_top++] = c;
    stack[stack_top] = '\0';
}

char stack_pop() {
    if (stack_top == 0) return '\0';
    char c = stack[--stack_top];
    stack[stack_top] = '\0';
    return c;
}

int reduce() {
    for (int i = 0; i < rules; i++) { 
        Rule *rule = &grammer[i]; // For each rule
        if (stack_top < rule->pr_len) continue; // Rule length is bigger than stack length

        if (strcmp(&stack[stack_top-rule->pr_len], rule->production) == 0) { // If rule matches
            for (int i = 0; i < rule->pr_len; i++) stack_pop();
            stack_push(rule->left);
            return i;
        }
    }

    return -1;
}

void main() {
    if (!load_grammer()) return;

    char str[100];
    printf("\nEnter the expr: ");
    scanf("%s", str);

    while(true) {
        printf("%s$\t", &str[pointer]); // Input sequence
        printf("%s\t", stack); // Stack

        int rule = reduce();
        if (rule > -1) printf("Reduced by %c -> %s\n", grammer[rule].left, grammer[rule].production);
        else {
            if (str[pointer] == '\0') break; // Reached end of string

            printf("Shift %c\n", str[pointer]);
            stack_push(str[pointer++]);
        }
    }

    if (strcmp(stack, "E") == 0) printf("Accepted\n"); 
    else printf("Rejected\n");
}