#include <stdio.h>

int main() {
    int a = 10;
#ifdef DEBUG
    printf("def a debug\n");
#endif
    for(int i = 0; i < 3; i++) {
        printf("hello world %d\n", i);
    }
    return 0;
}