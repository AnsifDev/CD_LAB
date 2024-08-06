#include <stdio.h>

int main() {
    FILE *src = fopen("src.c", "r");
    FILE *dest = fopen("dest.c", "w");

    char buffer[100];

    while(fgets(buffer, 99, src)) fprintf(dest, "%s", buffer);
    
    printf("\n");

    fclose(src);
    fclose(dest);

    return 0;
}