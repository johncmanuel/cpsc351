#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"

int main(int argc, const char* argv[]) { 
    int *p = malloc(sizeof(int));
    assert(p != NULL);

    printf("\n(%d) address pointed to by p: %p", getpid(), p);
    *p = 0;
    while (1) { 
        Spin(1);
        printf("\n(%d) p: %d", getpid(), ++*p);
    }
    printf("\n");
    return 0;
}