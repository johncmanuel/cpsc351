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

int run_cd(char *path) { return chdir(path); }

void save_prev_args(char **src, char **dst) {
  int i = 0;
  while (src[i] != NULL) {
    dst[i] = strdup(src[i]);
    i++;
  }
  dst[i] = NULL;
}

void get_cwd(char *cwd, size_t size) {
  if (getcwd(cwd, size) != NULL) {
    printf("Current directory: %s\n", cwd);
  } else {
    perror("getcwd() error");
  }
}

void help() {
  printf("This is a simple Unix shell program\n");
  printf("Here are the following commands:\n");
  printf("cd, mkdir, exit, !!");
  printf("Type 'exit' to exit the shell\n");
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
  get_cwd(cwd, sizeof(cwd));

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
      help();
      continue;
    } else if (strcmp(args[0], "!!") == 0) {
      if (prev_args[0] == NULL) {
        printf("No previous command\n");
        continue;
      }
      printf("Executing previous command\n");
      execute(prev_args);
      continue;
    } else if (strcmp(args[0], "cd") == 0) {
      if (args[1] == NULL) {
        printf("No directory provided\n");
        continue;
      }
      if (run_cd(args[1]) == 0) {
        printf("Changed directory to %s\n", args[1]);
        get_cwd(cwd, sizeof(cwd));
      } else {
        printf("Failed to change directory\n");
      }
      continue;
    }

    // Track previous command before execution
    save_prev_args(args, prev_args);

    execute(args);
  }
}
