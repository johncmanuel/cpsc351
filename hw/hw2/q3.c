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
        const char* args[] = { strdup("/bin/ls"), NULL };
        execvp(args[0], args);
        // execl(args[0], args[0], args[1]);
    }
    // Parent
    else {
        wait(NULL);
    }
    return 0;
}
