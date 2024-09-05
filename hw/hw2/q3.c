#define _GNU_SOURCE
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
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
        char* args[] = { strdup("/bin/ls"), NULL };

        // to be used with execvpe
        // char* envp[] = { "hello=world", NULL };

        // execl(args[0], args[0], args[1]);
        // execle(args[0], args[1]);
        execlp(args[0], args[0], args[1]);
        // execvp(args[0], args);
        // execv(args[0], args);
        // execvpe(args[0], args, envp);
    }
    // Parent
    else {
        wait(NULL);
    }
    return 0;
}
