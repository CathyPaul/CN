#include <stdio.h>

int main() {
    int num = 1234;
    char str[20];  // Buffer to store string

    sprintf(str, "%d", num);  // Convert int to string

    printf("Integer: %d\n", num);
    printf("String: %s\n", str);

    return 0;
}

