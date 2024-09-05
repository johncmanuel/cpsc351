
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
        int w = waitpid(NULL);
        printf("child output of wait: %d, currPid: %d\n", w, (int)getpid());
    }
    // Parent
    else {
        int w = waitpid(NULL);
        printf("parent output of wait: %d, currPid: %d\n", w, (int)getpid());
    }
    return 0;
}
