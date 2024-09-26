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

void execute(char **argv, char **prev_args) {
  pid_t pid;
  int status;
  if ((pid = fork()) < 0) {
    printf("Forking child process failed\n");
    exit(1);
  } else if (pid == 0) {
    printf("command invoked: %s\n", argv[0]);

    // If last word is ECHO, then print the rest of the words for one word per
    // line
    int length = arr_len(argv);
    if (strcmp(argv[length - 2], "ECHO") == 0) {
      for (int i = 0; i < length - 1; i++) {
        printf("%s\n", argv[i]);
      }
    }

    if (execvp(*argv, argv) < 0) {
      printf("Execution failed\n");
      exit(1);
    }
  } else {
    while (wait(&status) != pid) {
      // wait for something
    }
  }
  // Copy the current args to prev_args
  for (int i = 0; i < arr_len(argv); i++) {
    prev_args[i] = argv[i];
  }
}

int main(int arg, char *argv[]) {
  char line[1024];
  char *args[64];
  char *prev_args[64];

  while (1) {
    printf("shell> ");
    fgets(line, 1024, stdin);
    printf("line: %s", line);

    // Print SPACE and/or PIPE for each space or pipe character respectively
    for (int i = 0; i < strlen(line); i++) {
      if (line[i] == ' ') {
        printf("SPACE\n");
      } else if (line[i] == '|') {
        printf("PIPE\n");
      }
    }
    printf("====================================\n");

    parse(line, args);

    if (strcmp(args[0], "exit") == 0) {
      exit(0);
    } else if (strcmp(args[0], "help") == 0) {
      printf("This is a simple shell program\n");
      printf("Here are the following commands:\n");
      printf("cd, mkdir, exit, !!");
      printf("Type 'exit' to exit the shell\n");
      continue;
    } else if (strcmp(args[0], "!!") == 0) {
      if (prev_args[0] == NULL) {
        printf("No previous command\n");
        continue;
      }
    }
    execute(args, prev_args);
  }
}
