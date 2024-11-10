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

int load_operant(char* operant) {
    int rid = next_reg++%4;
    printf("MOV %s R%d\n", operant, rid);
    strcpy(reg_data[rid], operant);
    return rid;
}

void save_result(int rid, char* result) {
    printf("MOV R%d %s\n", rid, result);
    strcpy(reg_data[rid], result);
}

void get_opcode(char operator, char* opcode) {
    switch (operator) {
        case '+': strcpy(opcode, "ADD"); break;
        case '-': strcpy(opcode, "SUB"); break;
        case '*': strcpy(opcode, "MUL"); break;
        case '/': strcpy(opcode, "DIV"); break;
    }
}

int main () {
    printf("Reading input.itm\n");
    FILE *itm_file = fopen("input.itm", "r");
    if (itm_file == NULL) {
        printf("ERR: File cannot be opened\n");
    }

    while (!feof(itm_file)) {
        char result[8], op1[8], optr[8], op2[8];

        int rt = fscanf(itm_file, " %[a-zA-Z0-9] = %[a-zA-Z0-9] %1[+-*/] %[a-zA-Z0-9]", result, op1, optr, op2);
        if (rt < 2) return 1;

        bool single_operator = rt == 2;

        int rid1 = get_reg(op1);
        if (rid1 == -1) rid1 = load_operant(op1);

        if (!single_operator) {
            int rid2 = get_reg(op2);
            char opcode[8];
            get_opcode(optr[0], opcode);

            printf("%s ", opcode);
            if (rid2 == -1) printf("%s ", op2);
            else printf("R%d ", rid2);
            printf("R%d\n", rid1);
        }

        save_result(rid1, result);
    }
}