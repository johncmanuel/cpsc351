#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void parse(char *line, char **argv) {
  while (*line != '\0') {
    while (*line == ' ' || *line == '\t' || *line == '\n')
      *line++ = '\0';
    *argv++ = line;
    while (*line != '\0' && *line != ' ' && *line != '\t' && *line != '\n')
      line++;
  }
  *argv = '\0';
}

void execute(char **argv) {
  pid_t pid;
  int status;
  if ((pid = fork()) < 0) {
    printf("Forking child process failed\n");
    exit(1);
  } else if (pid == 0) {
    if (execvp(*argv, argv) < 0) {
      printf("Execution failed\n");
      exit(1);
    }
  } else {
    while (wait(&status) != pid) {
      // wait for something
    }
  }
}

int main(int arg, char *argv[]) {
  char line[1024];
  char *args[64];
  while (1) {
    printf("shell> ");
    fgets(line, 1024, stdin);
    printf("line: %s\n", line);

    parse(line, args);

    // Print out the arguments
    for (int i = 0; args[i] != NULL; i++) {
      printf("arg[%d]: %s\n", i, args[i]);
      if (strcmp(args[i], "|") == 0) {
        printf("PIPE\n");
      }
    }

    if (strcmp(args[0], "exit") == 0) {
      exit(0);
    }
    execute(args);
  }
}
