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

typedef struct { char op1[6], op2[6], opp, result[6]; int unknowns; bool simple_type; } ExprDefinision;
ExprDefinision expr_definision_table[100];
int expr_definision_table_size = 0;

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
    int n;
    printf("Enter number of expressions: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++)
    {
        char expr[100], opp[2];
        int temp1, temp2;
        ExprDefinision *expr_def = &expr_definision_table[expr_definision_table_size];
        printf("Expr %d: ", i+1);
        scanf("%s = %[^\n]", expr_def->result, expr);

        expr_def->unknowns = 0;
        if (sscanf(expr, "%s %1[+-*/%%] %s", expr_def->op1, opp, expr_def->op2) == 3) { // 2 operants with 1 operation
            expr_def->unknowns |= (sscanf(expr_def->op1, "%d", &temp1) != 1) << 1;
            expr_def->unknowns |= (sscanf(expr_def->op2, "%d", &temp2) != 1);
            expr_def->opp = opp[0];
            expr_def->simple_type = false;

            if ((expr_def->unknowns & 2) != 0) {
                int index = index_of_symbol(expr_def->op1);
                if (index > -1) {
                    temp1 = symbol_table[index].value;
                    sprintf(expr_def->op1, "%d", temp1);
                    expr_def->unknowns &= 1;
                }
            }

            if ((expr_def->unknowns & 1) != 0) {
                int index = index_of_symbol(expr_def->op2);
                if (index > -1) {
                    temp2 = symbol_table[index].value;
                    sprintf(expr_def->op2, "%d", temp2);
                    expr_def->unknowns &= 2;
                }
            }

            if (expr_def->unknowns == 0) {
                strcpy(symbol_table[symbol_table_size].id, expr_def->result);
                symbol_table[symbol_table_size].value = eval(temp1, temp2, opp[0]);
                symbol_table_size++;
            } else expr_definision_table_size++;
        } else if (sscanf(expr, "%s", expr_def->op1) == 1) { // 1 operant without operation
            expr_def->simple_type = true;
            if (sscanf(expr_def->op1, "%d", &temp1) == 1) { //It is a digit
                strcpy(symbol_table[symbol_table_size].id, expr_def->result);
                symbol_table[symbol_table_size].value = temp1;
                symbol_table_size++;
            } else {
                int index = index_of_symbol(expr_def->op1);
                if (index > -1) { // Value is directly available
                    strcpy(symbol_table[symbol_table_size].id, expr_def->result);
                    symbol_table[symbol_table_size].value = symbol_table[index].value;
                    symbol_table_size++;
                } else { // Unknown value
                    expr_def->unknowns = 2;
                    expr_definision_table_size++;
                }
            }
        }
    }

    if (expr_definision_table_size > 0) for (int i = 0; i < expr_definision_table_size; i++) {
        ExprDefinision *expr_def = &expr_definision_table[i];
        if (expr_def->simple_type) printf("%s = %s\n", expr_def->result, expr_def->op1);
        else printf("%s = %s %c %s\n", expr_def->result, expr_def->op1, expr_def->opp, expr_def->op2);
    } else printf("%s = %d\n", symbol_table[symbol_table_size-1].id, symbol_table[symbol_table_size-1].value);
}