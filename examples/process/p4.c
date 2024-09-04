#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, const char* argv[]) { 
    int rc = fork();
    if (rc < 0) { fprintf(stderr, "Fork failed\n"); exit(1); }  // failed
    else if (rc == 0) { // child {
        close(STDOUT_FILENO);    // redirect stdout
        open("./p4.output", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);

        // printf("child (pid:%d)\n", (int)getpid());
        char* myargs[] = { strdup("wc"), strdup("p4.c"), NULL };
        execvp(myargs[0], myargs);  // runs wordcount on p3.c
        fprintf(stderr, "Could not execute wc\n");  // should never get here because execvp will exit
    } else {
        wait(NULL);
    }
    return 0;
}
