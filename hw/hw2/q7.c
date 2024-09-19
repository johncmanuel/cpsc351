
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, const char *argv[]) {
  int p[2];

  if (pipe(p) < 0) {
    printf("pipe error");
    exit(1);
  }

  int rc1 = fork();

  if (rc1 < 0) {
    fprintf(stderr, "Fork 1 failed\n");
    exit(1);
  }
  // child 1
  else if (rc1 == 0) {
    close(p[0]);
    dup2(p[1], STDOUT_FILENO);
    close(p[1]);

    char *args[] = {strdup("grep"), strdup("-o"), strdup("close"),
                    strdup("q7.c"), NULL};
    execvp(args[0], args);
  }

  int rc2 = fork();

  if (rc2 < 0) {
    fprintf(stderr, "Fork 2 failed\n");
    exit(1);
  }
  // child 2
  else if (rc2 == 0) {
    close(p[1]);
    dup2(p[0], STDIN_FILENO);
    close(p[0]);

    char *args[] = {strdup("wc"), strdup("-l"), NULL};
    execvp(args[0], args);
  }

  close(p[0]);
  close(p[1]);

  return 0;
}
