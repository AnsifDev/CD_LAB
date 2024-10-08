#include <stdio.h>
#include <stdlib.h>
#include "../Types/string.h"
#include "../Types/stack.h"

Value string_to_value(String s) {
    Value v = { .object = s };
    return v;
}

int main(int argc, char const *argv[])
{
    int choice;
    Stack stack = stack_new(5);
    while (TRUE) {
        printf(">> ");
        scanf("%d", &choice);

        if (choice == 0) exit(0);
        if (choice == 1) {
            String str = string_new_empty(25);
            scanf("%s", str);
            stack_push(stack, string_to_value(str));
        } else if (choice == 2) {
            String str = stack_pop(stack).object;
            printf("%s\n", str);
            free(str);
        }

        printf("s:\t");
        for (int i = 0; i < stack->top; i++) {
            printf("%s\t", stack->head[i].object);
        }
        printf("\n");
    }
    return 0;
}
