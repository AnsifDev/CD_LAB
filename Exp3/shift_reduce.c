#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define bool int
#define TRUE 1
#define FALSE 0

typedef struct {
    char left;
    char production[100];
    int pr_len;
} Rule;

Rule grammer[100];
int rules = 0;

bool load_grammer() {
    FILE *gmr_file = fopen("grammer.gmr", "r");
    if (gmr_file == NULL) return FALSE;

    while(TRUE) {
        int rt = fscanf(gmr_file, " %c -> %s ", &grammer[rules].left, grammer[rules].production);
        if (rt == -1) return TRUE;
        if (rt != 2) return FALSE;
        grammer[rules].pr_len = strlen(grammer[rules].production);
        rules++;
    }
}

char stack[100];
int pointer = 0, stack_top = 0;

bool match_production(char *production, int max_len) {
    for (int i = 0; i < stack_top && i < max_len; i++) {
        // printf("%c:%c\n", production[max_len-1-i], stack[stack_top-1-i]);
        if (production[max_len-1-i] != stack[stack_top-1-i]) return FALSE;
    }
    
    return TRUE;
}

int reduce() {
    for (int rule = 0; rule < rules; rule++) { // For each rule
        if (stack_top < grammer[rule].pr_len) continue; // Rule length not matched

        if (match_production(grammer[rule].production, grammer[rule].pr_len)) {
            stack_top -= grammer[rule].pr_len;
            stack[stack_top++] = grammer[rule].left;
            stack[stack_top] = '\0';
            return rule;
        }
    }

    return -1;
}

int main() {
    if (load_grammer()) printf("Grammer Loaded\n");
    else {
        printf("ERR: Grammer Failed to Load\n");
        return 1;
    }

    for (int i = 0; i < rules; i++) 
        printf("%c -> %s\n", grammer[i].left, grammer[i].production);

    char str[100];
    int str_len;
    printf("Enter the expr: ");
    scanf("%s%n", str, &str_len);
    str[str_len] = '$';
    str[str_len+1] = '\0';

    while(TRUE) {
        printf("%s\t%s\t", str+pointer, stack); //Input Character

        int rule = reduce();
        if (rule > -1) printf("Reduced by %c -> %s\n", grammer[rule].left, grammer[rule].production);
        else {
            if (str[pointer] == '$') break;
            printf("Shift %c\n", str[pointer]);
            stack[stack_top++] = str[pointer++];
            stack[stack_top] = '\0';
        }
    }

    if (strcmp(stack, "E") == 0) {
        printf("Accepted\n");
        return 0;
    } else {
        printf("Rejected\n");
        return 2;
    }    
}