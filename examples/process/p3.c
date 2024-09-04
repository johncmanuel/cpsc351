#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, const char* argv[]) {
    int rc = fork();
    if (rc < 0) { fprintf(stderr, "Fork failed\n"); exit(1); }  // failed
    else if (rc == 0) { // child {
        printf("child (pid:%d)\n", (int)getpid());
        printf("word count of p3.c is... (lines, words, characters)...\n");

        // char* myargs[] = { strdup("cal"), strdup("sep"), strdup("2024"), NULL };
        // char* myargs[] = { strdup("ls"), strdup("-la"), NULL };
        char* myargs[] = { strdup("pwd"),  NULL };
        // int numbers[] = { 1, 2, 3, 4, 5, 6, 7, 8 };

        execvp(myargs[0], myargs);  // runs wordcount on p3.c
        fprintf(stderr, "Could not execute wc\n");  // should never get here because execvp will exit
    }
    else {
        printf("parent of %d (pid:%d)\n\n", rc, (int)getpid());
    }
    return 0;
}
