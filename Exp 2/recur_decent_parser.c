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
    if (!t()) return FALSE;
    if (!ed()) return FALSE;

    printf("E\t->\tTE\'\n");
    return TRUE;
}

bool ed() {
    if (str[pointer] == '+' || str[pointer] == '-') {
        char c = str[pointer++];
        if (!t()) return FALSE;
        if (!ed()) return FALSE;

        printf("E\'\t->\t%cTE\'\n", c);
    } else printf("E\'\t->\te\n");

    return TRUE;
}

bool t() {
    if (!f()) return FALSE;
    if (!td()) return FALSE;

    printf("T\t->\tFT\'\n");
    return TRUE;
}

bool td() {
    if (str[pointer] == '*' || str[pointer] == '/') {
        char c = str[pointer++];
        if (!f()) return FALSE;
        if (!td()) return FALSE;

        printf("T\'\t->\t%cFT\'\n", c);
    } else printf("T\'\t->\te\n");

    return TRUE;
}

bool f() {
    if (str[pointer] == '(') {
        pointer++;
        if (!e()) return FALSE;
        if (str[pointer] != ')') return FALSE;
        pointer++;

        printf("F\t->\t(E)\n");
    } else if (str[pointer] == 'i') {
        pointer++;

        printf("F\t->\ti\n");
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