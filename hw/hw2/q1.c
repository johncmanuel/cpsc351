#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, const char* argv[]) {
    int rc = fork();
    if (rc < 0) {
        printf("fork failed");
        exit(1);
    }
    else if (rc == 0) {
        printf("child");
    }
    else {
        printf("parent");
    }
    return 0;
}
