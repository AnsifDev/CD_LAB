#include <stdio.h>
#include <string.h>
#include <stdbool.h>

char reg_data[4][6];
int next_reg = 0;

int get_reg(char* var) {
    for (int i = 0; i < 4; i++) 
        if (strcmp(var, reg_data[i]) == 0) return i;
    
    return -1;
}

int get_new_reg() { return next_reg++; }

int main () {
    printf("Reading input.itm\n");
    FILE *itm_file = fopen("input.itm", "r");
    if (itm_file == NULL) {
        printf("ERR: File cannot be opened\n");
    }

    char expr[100], opp[2], result[6], op1[6], op2[6];
    while (fscanf(itm_file, "%s = %[^\n]", result, expr) != -1)
    {
        bool simple_type = sscanf(expr, "%s %1[+-*/%%] %s", op1, opp, op2) != 3;
        int rid1 = get_reg(op1);
        if (rid1 == -1) {
            rid1 = get_new_reg();
            printf("MOV %s R%d\n", op1, rid1);
        }

        int rid2 = get_reg(op2);
        if (!simple_type)
        if (rid2 == -1) switch (opp[0]) {
            case '+': printf("ADD %s R%d\n", op2, rid1); break;
            case '-': printf("SUB %s R%d\n", op2, rid1); break;
            case '*': printf("MUL %s R%d\n", op2, rid1); break;
            case '/': printf("DIV %s R%d\n", op2, rid1); break;
        } else switch (opp[0]) {
            case '+': printf("ADD R%d R%d\n", rid2, rid1); break;
            case '-': printf("SUB R%d R%d\n", rid2, rid1); break;
            case '*': printf("MUL R%d R%d\n", rid2, rid1); break;
            case '/': printf("DIV R%d R%d\n", rid2, rid1); break;
        }

        printf("MOV R%d %s\n", rid1, result);

        strcpy(reg_data[rid1], result);
    }
}