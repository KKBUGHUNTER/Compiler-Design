#include<stdio.h>

int main() {
    int a = 10;
    float b = 20.34;
    char name[100] = "karthikeyan";

    if (a < b)
        printf("%d\n", a);
    else if (a > b)
        printf("%d\n", b);
    else
        printf("Equal\n");

    return 0;
}

