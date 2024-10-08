#include <stdio.h>

#define bool int
#define TRUE 1
#define FALSE 0

char str[100];
int pointer = 0;

bool e();
bool ed();
bool t();
bool td();
bool f();

bool e() {
    printf("E\t->\tTE\'\n");
    if (!t()) return FALSE;
    if (!ed()) return FALSE;

    return TRUE;
}

bool ed() {
    if (str[pointer] == '+' || str[pointer] == '-') {
        printf("E\'\t->\t%cTE\'\n", str[pointer]);
        pointer++;
        if (!t()) return FALSE;
        if (!ed()) return FALSE;

    } else printf("E\'\t->\te\n");

    return TRUE;
}

bool t() {
    printf("T\t->\tFT\'\n");
    if (!f()) return FALSE;
    if (!td()) return FALSE;

    return TRUE;
}

bool td() {
    if (str[pointer] == '*' || str[pointer] == '/') {
        printf("T\'\t->\t%cFT\'\n", str[pointer]);
        pointer++;
        if (!f()) return FALSE;
        if (!td()) return FALSE;

    } else printf("T\'\t->\te\n");

    return TRUE;
}

bool f() {
    if (str[pointer] == '(') {
        printf("F\t->\t(E)\n");
        pointer++;
        if (!e()) return FALSE;
        if (str[pointer] != ')') return FALSE;
        pointer++;

    } else if (str[pointer] == 'i') {
        printf("F\t->\ti\n");
        pointer++;
    } else return FALSE;

    return TRUE;
}

int main() {
    printf("Enter the expr: ");
    scanf("%s", str);

    bool success = e();
    if (success) printf("Expr parsed sucessfully\n");
    else printf("Expr is not parsed\n");

    return success? 0: 1;
}