#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define LOADABLE_KEYWORDS_COUNT 32
#define LOADABLE_OPERATORS_COUNT 26

char keywords[LOADABLE_KEYWORDS_COUNT][16];
char operators[LOADABLE_OPERATORS_COUNT][16];

void load_data(char * filename, int arr_size, char arr[arr_size][16]) {
    FILE *file = fopen(filename, "r");

    for (int i = 0; i < arr_size; i++) fscanf(file, "%s", arr[i]);

    printf("Loaded %d items from %s\n", arr_size, filename);
    fclose(file);
}

bool parse(FILE *src_file, char* regex, char* extracted) {
    extracted[0] = '\0'; // Reset output string
    if (src_file == NULL) return false; // Error if file isn't opened;
    
    int pointer = ftell(src_file); // Mark current pointer
    if (fscanf(src_file, regex, extracted) == 1) return true;

    fseek(src_file, pointer, SEEK_SET); // Reset pointer back to previous
    return false;
}

void extract_literals(FILE *src_file, char terminal, char* extracted) {
    extracted[0] = '\0'; // Reset output string
    if (src_file == NULL) return; // Error if file isn't opened;

    int i = 0;
    while (true) {
        char c = fgetc(src_file);

        if (c == '\\') {
            extracted[i++] = '\\';
            c = fgetc(src_file);
        } else if (c == terminal) break;

        extracted[i++] = c;
        extracted[i] = '\0';
    }
}

int index_of(char* value, int arr_size, char arr[arr_size][16]) {
    for (int i = 0; i < arr_size; i++)
        if (strcmp(arr[i], value) == 0) return i;
    
    return -1;
}

void extract_leximes() {
    FILE *src_file = fopen("src.c", "r");
    FILE *dest_file = fopen("dest.txt", "w");

    char token[100];
    while(!feof(src_file)) {
        if (parse(src_file, " %[bx0-9.fd]", token)) fprintf(dest_file, "%s\t\t->\tNumeric Constants\n", token);
        else if (parse(src_file, " %[a-zA-Z0-9]", token)) 
            if (index_of(token, LOADABLE_KEYWORDS_COUNT, keywords) == -1) fprintf(dest_file, "%s\t\t->\tIdentifiers\n", token);
            else fprintf(dest_file, "%s\t\t->\tKeywords\n", token);
        else if (parse(src_file, " %[-+*/%.<>=!&|~^]", token)) 
            if (index_of(token, LOADABLE_OPERATORS_COUNT, operators) != -1) fprintf(dest_file, "%s\t\t->\tOperators\n", token);
            else for (int i = 0; i < strlen(token); i++) fprintf(dest_file, "%c\t\t->\tPunctuators\n", token[i]);
        else {
            char c = getc(src_file);
            if (c == EOF) break;
            
            if (c == '#') fscanf(src_file, " %*[^\n]");
            else if (c == '\'') {
                extract_literals(src_file, '\'', token);
                fprintf(dest_file, "\'%s\'\t\t->\tCharacter Constants\n", token);
            } else if (c == '\"') {
                extract_literals(src_file, '\"', token);
                fprintf(dest_file, "\"%s\"\t->\tString Constants\n", token);
            } else if (c == ' ' || c == '\t' || c == '\n') continue;
            else fprintf(dest_file, "%c\t\t->\tPunctuators\n", c);
        }
    }

    fclose(src_file);
    fclose(dest_file);
}

int main() {
    load_data("keywords.kw", LOADABLE_KEYWORDS_COUNT, keywords);
    load_data("operators.opr", LOADABLE_OPERATORS_COUNT, operators);

    extract_leximes();

    return 0;
}