#include <stdio.h>
#include <stdbool.h>

char str[100];
int pointer = 0;

bool e();
bool ed();
bool t();
bool td();
bool f();

bool move_if_match(char c) {
    if (str[pointer] != '+') return false;
    pointer++;
    return true;
}

bool e() {
    printf("E\t->\tTE\'\n");
    if (!t()) return false;
    if (!ed()) return false;

    return true;
}

bool ed() {
    if (move_if_match('+')) {
        printf("E\'\t->\t+TE\'\n");
        if (!t()) return false;
        if (!ed()) return false;
    } else if (move_if_match('-')) {
        printf("E\'\t->\t+TE\'\n");
        if (!t()) return false;
        if (!ed()) return false;
    } else printf("E\'\t->\te\n");

    return true;
}

bool t() {
    printf("T\t->\tFT\'\n");
    if (!f()) return false;
    if (!td()) return false;

    return true;
}

bool td() {
    if (move_if_match('*')) {
        printf("T\'\t->\t*FT\'\n");
        if (!f()) return false;
        if (!td()) return false;
    } else if (move_if_match('-')) {
        printf("T\'\t->\t/FT\'\n");
        if (!f()) return false;
        if (!td()) return false;
    } else printf("T\'\t->\te\n");

    return true;
}

bool f() {
    if (move_if_match('(')) {
        printf("F\t->\t(E)\n");
        if (!e()) return false;
        if (move_if_match(')')) return false;

    } else if (move_if_match('(')) printf("F\t->\ti\n");
    else return false;

    return true;
}

int main() {
    printf("Enter the expr: ");
    scanf("%s", str);

    bool success = e();
    if (success) printf("Expr parsed sucessfully\n");
    else printf("Expr is not parsed\n");

    return success? 0: 1;
}