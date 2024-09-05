
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, const char* argv[]) {
    int rc = fork();
    if (rc < 0) {
        printf("fork failed");
        exit(1);
    }
    // Child
    else if (rc == 0) {
        int w = wait(NULL);
        // printf("child\n");
        printf("child output of wait: %d\n", w);
    }
    // Parent
    else {
        int w = wait(NULL);
        // printf("parent");
        printf("parent output of wait: %d\n", w);
    }
    return 0;
}
