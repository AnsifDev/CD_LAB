#include <stdio.h>
#include <string.h>

typedef struct { char opcode[8]; int bin; } OpcodeEntry;
OpcodeEntry optab[20];
int opcodes_loaded = 0;

int locctr = 0;

int bin_of_opcode(char *opcode) {
    for (int i = 0; i < opcodes_loaded; i++) 
        if (strcmp(optab[i].opcode, opcode) == 0) return optab[i].bin;
    return -1;
}

void on_each_line(char* label, char* opcode, char* operant) {
    printf("%X\t%s\t%s\t%s\n", locctr, label, opcode, operant);
    if (strcmp(opcode, "START") == 0) sscanf(operant, "%X", &locctr);
    else if (bin_of_opcode(opcode) != -1) locctr += 3;
    else if (strcmp(opcode, "WORD") == 0) locctr += 3;
    else if (strcmp(opcode, "BYTE") == 0) locctr += 1;
    else if (strcmp(opcode, "RESW") == 0) {
        int reservation = 0;
        if (operant[0] == 'X') sscanf(operant, "X'%X'", &reservation);
        else sscanf(operant, "%d", &reservation);
        locctr += 3 * reservation;
    }
    else if (strcmp(opcode, "RESB") == 0) {
        int reservation = 0;
        if (operant[0] == 'X') sscanf(operant, "X'%X'", &reservation);
        else sscanf(operant, "%d", &reservation);
        locctr += 1 * reservation;
    }
}

void load_otb() {
    FILE *otb_file = fopen("optab.otb", "r");

    printf("Loading OPTAB from optab.otb\n");
    char line[20];
    while (fgets(line, 19, otb_file) != NULL) {
        sscanf(line, "%s %X", optab[opcodes_loaded].opcode, &optab[opcodes_loaded].bin);
        opcodes_loaded++;
    }

    printf("Loaded %d opcodes\n", opcodes_loaded);
}

void real_main() {
    FILE *pgm = fopen("pgm.src", "r");

    char line[200];
    while (fgets(line, 199, pgm) != NULL) {
        char label[50], opcode[50], operant[100];

        operant[0] = 0;
        sscanf(line, "%s %s %s", label, opcode, operant);
        if (operant[0] == 0) {
            sscanf(line, "%s %s", opcode, operant);
            on_each_line(NULL, opcode, operant);
        }
        else on_each_line(label, opcode, operant);
    }
}

void main() {
    load_otb();
    real_main();
}