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

int main() {
    char str1[100], str2[100], str3[100];
    int n, r;

    printf("%d\n", sscanf("6f", " %ff%n", &r, &n));
    printf("%d\n", n);
}