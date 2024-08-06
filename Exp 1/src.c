#include <stdio.h>
#include <stdlib.h>

void main() {
    int a, b;

    printf("Enter Number 1: " );
    scanf("%d", &a);

    printf("Enter Number 2: ");
    scanf("%d", &b);

    int sum = (a + b) % 6;
    printf("The sum is %d\n", sum);
}