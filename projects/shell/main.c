#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int arr_len(char **arr) {
  int length = 0;
  while (arr[length] != NULL) {
    length++;
  }
  return length;
}

void parse(char *line, char **argv) {
  while (*line != '\0') {
    while (*line == ' ' || *line == '\t' || *line == '\n')
      *line++ = '\0';
    *argv++ = line;
    while (*line != '\0' && *line != ' ' && *line != '\t' && *line != '\n')
      line++;
  }
  *argv = NULL;
}

void execute(char **argv) {
  pid_t pid;
  int status;
  if ((pid = fork()) < 0) {
    printf("Forking child process failed\n");
    exit(1);
  } else if (pid == 0) {
    printf("command invoked - argv[0]: %s\n", argv[0]);

    // If last word is ECHO, then print the rest of the words for one word per
    // line
    int length = arr_len(argv);
    if (strcmp(argv[length - 2], "ECHO") == 0) {
      int i = 1;
      while (argv[i] != NULL) {
        printf("%s\n", argv[i]);
        i++;
      }
    }

    if (execvp(*argv, argv) < 0) {
      printf("Execution failed\n");
      exit(1);
    }
  } else {
    while (wait(&status) != pid) {
      // wait for something
      printf("hello");
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

    if (strcmp(args[0], "exit") == 0) {
      exit(0);
    }
    execute(args);
  }
}
