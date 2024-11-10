#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct { char id[6]; int value; } SymbolValue;
SymbolValue symbol_table[100];
int symbol_table_size = 0;

int index_of_symbol(char* id) {
    for (int i = 0; i < symbol_table_size; i++)
        if (strcmp(symbol_table[i].id, id) == 0) return i;
    
    return -1;
}

void assign_symbol(char *id, int value) {
    strcpy(symbol_table[symbol_table_size].id, id);
    symbol_table[symbol_table_size].value = value;
    symbol_table_size++;
}

int eval(int a, int b, char opp) {
    switch (opp) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        case '%': return a % b;
    }
}

int main() {
    FILE *src_file = fopen("src.itm", "r");

    while (!feof(src_file)) {
        char result[8], op1[8], optr[8], op2[8];

        int rt = fscanf(src_file, " %[a-zA-Z0-9] = %[a-zA-Z0-9] %1[+-*/%%] %[a-zA-Z0-9]", result, op1, optr, op2);
        if (rt < 2) return 1;

        int op1_value = -1, op2_value = -1;
        bool single_operator = rt == 2;

        if (sscanf(op1, "%d", &op1_value) != 1) {
            int index = index_of_symbol(op1);
            if (index > -1) {
                op1_value = symbol_table[index].value;
                sprintf(op1, "%d", op1_value);
            }
        }
 
        if (!single_operator && sscanf(op2, "%d", &op2_value) != 1) {
            int index = index_of_symbol(op2);
            if (index > -1) {
                op2_value = symbol_table[index].value;
                sprintf(op2, "%d", op2_value);
            }
        }

        if (op1_value >= 0 && (op2_value >= 0 || single_operator)) {
            int value = rt == 2? op1_value: eval(op1_value, op2_value, optr[0]);
            assign_symbol(result, value);
            single_operator = true;
            sprintf(op1, "%d", value);
        }

        if (single_operator) printf("%s = %s\n", result, op1);
        else printf("%s = %s %s %s\n", result, op1, optr, op2);
    }

    return 0;
}