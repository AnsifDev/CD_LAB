#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define bool int
#define TRUE 1
#define FALSE 0

#define LOADABLE_KEYWORDS_COUNT 32
#define LOADABLE_OPERATORS_COUNT 26

char keywords[LOADABLE_KEYWORDS_COUNT][16];
char operators[LOADABLE_OPERATORS_COUNT][4];
// char paunctuators

void load_keywords() {
    FILE *kw_file = fopen("keywords.kw", "r");

    for (int i = 0; i < LOADABLE_KEYWORDS_COUNT; i++) fscanf(kw_file, "%s", keywords[i]);

    printf("Loaded %d Keywords from keywords.kw\n", LOADABLE_KEYWORDS_COUNT);
    fclose(kw_file);
}

void load_operators() {
    FILE *opr_file = fopen("operators.opr", "r");

    for (int i = 0; i < LOADABLE_OPERATORS_COUNT; i++) fscanf(opr_file, "%s", operators[i]);

    printf("Loaded %d Operators from operators.opr\n", LOADABLE_OPERATORS_COUNT);
    fclose(opr_file);
}

void extract_leximes() {
    FILE *src_file = fopen("src.c", "r");
    FILE *dest_file = fopen("dest.txt", "w");

    int numeric = 0;
    int literalString = 0;
    int literalChar = 0;
    int paunctuators = 0;
    int operators_count = 0;
    int keywords_count = 0;
    int identifiers = 0;
    
    char token[64];
    int current_pos = ftell(src_file), rt = -1;
    while (TRUE) {
        // Detect Numeric Constants
        fseek(src_file, current_pos, SEEK_SET);
        rt = fscanf(src_file, " %[bx0-9.fd]", token);
        if (rt == -1) break;
        if (rt == 1) {
            int len = strlen(token), integer = -1, floating = -1, floating2 = -1;

            sscanf(token, "%*i%n", &integer);
            sscanf(token, "%*f%n", &floating);
            sscanf(token, "%*f%*[fd]%n", &floating2);
            
            if (floating2 == len || floating == len || integer == len) {
                fprintf(dest_file, "%s => NumericConstant [%s]\n", token, integer == len? "Integer": "Float" );
                numeric++;
                current_pos = ftell(src_file);
                continue;
            }
        }

        // Detect Keywords or Identifiers
        fseek(src_file, current_pos, SEEK_SET);
        rt = fscanf(src_file, " %[a-zA-Z0-9] ", token);
        if (rt == 1) {
            bool keyword_found = FALSE;
            for (int i = 0; i < LOADABLE_KEYWORDS_COUNT; i++) if (strcmp(token, keywords[i]) == 0) {
                keyword_found = TRUE;
                break;
            }

            fprintf(dest_file, "%s => %s\n", token, keyword_found? "Keyword": "Identifier");
            if (keyword_found) keywords_count++;
            else identifiers++;
            current_pos = ftell(src_file);
            continue;
        }

        // Detect Operators
        fseek(src_file, current_pos, SEEK_SET);
        rt = fscanf(src_file, " %[-+*/%.<>=!&|~^] ", token);
        if (rt == 1) {
            bool operator_found = FALSE;
            for (int i = 0; i < LOADABLE_OPERATORS_COUNT; i++) if (strcmp(token, operators[i]) == 0) {
                operator_found = TRUE;
                break;
            }

            if (operator_found) {
                fprintf(dest_file, "%s => Operator\n", token);
                current_pos = ftell(src_file);
                operators_count++;
                continue;
            }
        }

        fseek(src_file, current_pos, SEEK_SET);
        char c = getc(src_file);

        // Detect Header files
        if (c == '#') {
            fscanf(src_file, "%*[^\n]");
            current_pos = ftell(src_file);
            continue;
        } 

        // Detect String Constants
        if (c == '\"') {
            fprintf(dest_file, "\"");

            while (TRUE) {
                c = fgetc(src_file);

                if (c == '\\') {
                    fprintf(dest_file, "\\");
                    c = fgetc(src_file);
                } else if (c == '\"') break;

                fprintf(dest_file, "%c", c);
            }

            fprintf(dest_file, "\" => Literal [String]\n");
            current_pos = ftell(src_file);
            literalString++;
            continue;
        }

        // Detect Character Constants
        if (c == '\'') {
            fscanf(src_file, "%c \'", &c);
            fprintf(dest_file, "\'%c\' => Literal [Character]\n", c);

            current_pos = ftell(src_file);
            literalChar++;
            continue;
        }
        
        // Ignoring Invisible Characters
        if (c == ' ' || c == '\t' || c == '\n') {
            current_pos++;
            continue;
        }
        
        fprintf(dest_file, "%c => Paunctuator\n", c);
        paunctuators++;
        current_pos++;
    }

    printf("Extracted\n\t%d Numeric Constants\n\t%d String Contants\n\t%d Character Constants\n\t%d Keywords\n\t%d Identifiers\n\t%d Operators\n\t%d Paunctuators\n", numeric, literalString, literalChar, keywords_count, identifiers, operators_count, paunctuators);
    
    fclose(src_file);
    fclose(dest_file);
}

int main() {
    load_keywords();
    load_operators();

    extract_leximes();

    return 0;
}