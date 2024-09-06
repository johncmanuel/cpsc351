
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, const char* argv[]) {
    int p[2];

    // Create pipe
    if (pipe(p) < 0) {
        printf("pipe error");
        exit(1);
    }

    int rc1 = fork();

    if (rc1 < 0) { fprintf(stderr, "Fork 1 failed\n"); exit(1); }
    // Child 1
    else if (rc1 == 0) {
        close(p[0]);
        dup2(p[1], STDOUT_FILENO);
        close(p[1]);
        printf("child 1");
    }
    // Parent
    else {
        wait(NULL);
    }

    int rc2 = fork();

    if (rc2 < 0) { fprintf(stderr, "Fork 2 failed\n"); exit(1); }
    // Child 2
    else if (rc2 == 0) {
        close(p[1]);
        dup2(p[0], STDIN_FILENO);
        close(p[1]);
        printf("child 2");
    }
    // Parent
    else {
        wait(NULL);
    }

    return 0;
}