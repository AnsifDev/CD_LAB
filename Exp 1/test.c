#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define bool int
#define TRUE 1
#define FALSE 0

char keywords[32][16];

void load_keywords() {
    FILE *kw_file = fopen("keywords.kw", "r");

    printf("Loading Keywords from keywords.kw\n");
    for (int i = 0; i < 32; i++) fscanf(kw_file, "%s", keywords[i]);

    printf("Loaded 32 Keywords\n");
    fclose(kw_file);
}

char * load_src(int *buffer_length) {
    FILE *src_file = fopen("src.c", "r");
    int file_size = 0;
    
    if (src_file == NULL) return;

    fseek(src_file, 0, SEEK_END);
    file_size = ftell(src_file);
    fseek(src_file, 0, SEEK_SET);

    int file_buff_size = 0;
    char* file_buff = calloc(sizeof(char *), file_size+1);

    for (int i = 0; i < file_size; i++) file_buff[i] = fgetc(src_file);
    file_buff[file_size] = '\0';
    
    printf("%s\n\n", file_buff);

    fclose(src_file);
    free(file_buff);
}

int main() {
    load_keywords();

    analyse();

    return 0;
}