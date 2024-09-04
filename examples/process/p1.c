#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, const char* argv[]) { 
    printf("hello (pid:%d)\n", (int)getpid());

    int rc = fork();
    if (rc < 0) { fprintf(stderr, "Fork failed\n"); exit(1); }
    else if (rc == 0) { // child {
        printf("child (pid:%d)\n", (int)getpid());
    } else {
        printf("parent of %d (pid:%d)\n\n", rc, (int)getpid());
    }
    printf("hello i finished");
    return 0;
}
