
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, const char* argv[]) {
    int rc = fork();
    if (rc < 0) { fprintf(stderr, "Fork failed\n"); exit(1); }
    // Child 
    else if (rc == 0) {
        close(STDOUT_FILENO);
        printf("after close");
    }
    // Parent
    else {
        wait(NULL);
    }
    return 0;
}