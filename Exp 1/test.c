#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define bool int
#define TRUE 1
#define FALSE 0

typedef struct {
    int start, length;
} Block;

Block *blocks;
int blocks_size = 32, blocks_length = 0;

int match_header_file_inclusion(FILE *file, int pos) {
    int rt = -1;
    char header_file[32];

    fseek(file, pos, SEEK_SET);
    rt = fscanf(file, " # include < %[a-zA-Z.] >\n", header_file);
    if (rt == -1) return -1;
    if (rt != 1) return 1;

    printf("%s header file inclusion found [IGNORED]\n", header_file);
    
    return 0;
}

int match_functions_without_params(FILE *file, int pos) {
    int rt = -1;

    char datatype[16], fn_name[32];

    fseek(file, pos, SEEK_SET);
    rt = fscanf(file, " %s %s ( )", datatype, fn_name);
    if (rt == -1) return -1;
    if (rt != 2) return 1;

    printf("Declaration of function %s of type %s without parameters found\n", fn_name, datatype);
}

int main() {
    FILE *src_file = fopen("src.c", "r");

    int current_pos = ftell(src_file), rt = -1;
    while (TRUE) {
        // Case 1: Header file inclusion
        rt = match_header_file_inclusion(src_file, current_pos);
        if (rt == -1) break;
        if (rt == 0) {
            current_pos = ftell(src_file);
            continue;
        }

        // Case 2: Function declaration without params
        rt = match_functions_without_params(src_file, current_pos);
        if (rt == -1) break;
        if (rt == 0) {
            current_pos = ftell(src_file);
            continue;
        }

        printf("ERR: No rules matched\n");
        return 1;
    }

    fclose(src_file);

    return 0;
}