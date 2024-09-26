#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

char *prev_args[64];
char cwd[1024];

int arr_len(char **arr) {
  int length = 0;
  while (arr[length] != NULL) {
    length++;
  }
  return length;
}

void save_prev_args(char **src, char **dst) {
  int i = 0;
  while (src[i] != NULL) {
    dst[i] = strdup(src[i]);
    i++;
  }
  dst[i] = NULL;
}

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
    // If last word is ECHO, then print the rest of the words for one word per
    // line
    int length = arr_len(argv);
    if (strcmp(argv[length - 2], "ECHO") == 0) {
      for (int i = 0; i < length - 1; i++) {
        printf("%s\n", argv[i]);

        // There is technically a space char for each argument provided
        // so, print SPACE until the last argument, ECHO
        if (i != length - 2) {
          printf("SPACE\n");
        }

        // Print PIPE if the current argument is a pipe character
        if (strcmp(argv[i], "|") == 0) {
          printf("PIPE\n");
        }
      }
    }
    // printf("print args\n");
    // for (int i = 0; i < length; i++) {
    //   printf("argv[%d]: %s\n", i, argv[i]);
    //   if (argv[i] == NULL) {
    //     printf("NULL\n");
    //   }
    // }

    if (execvp(argv[0], argv) < 0) {
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

  // Get current working directory
  if (getcwd(cwd, sizeof(cwd)) != NULL) {
    printf("Current directory: %s\n", cwd);
  } else {
    perror("getcwd() error");
  }

  while (1) {
    printf("%s > ", cwd);
    fgets(line, 1024, stdin);
    printf("line: %s", line);

    // parse() doesn't set the last element to NULL, so do it manually.
    parse(line, args);
    args[arr_len(args) - 1] = NULL;

    if (strcmp(args[0], "exit") == 0) {
      exit(0);
    } else if (strcmp(args[0], "help") == 0) {
      printf("This is a simple Unix shell program\n");
      printf("Here are the following commands:\n");
      printf("cd, mkdir, exit, !!");
      printf("Type 'exit' to exit the shell\n");
      continue;
    } else if (strcmp(args[0], "!!") == 0) {
      if (prev_args[0] == NULL) {
        printf("No previous command\n");
        continue;
      }
      printf("Executing previous command\n");
      execute(prev_args);
      continue;
    }
    // Track previous command before execution
    save_prev_args(args, prev_args);

    execute(args);
  }
}
