#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_ARGS_SIZE 64
#define MAX_BUFFER_SIZE 1024

// Track prev args and cwd
char *prev_args[MAX_ARGS_SIZE];
char cwd[MAX_BUFFER_SIZE];

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

void get_cwd(char *cwd, size_t size) {
  if (getcwd(cwd, size) != NULL) {
    printf("Current directory: %s\n", cwd);
  } else {
    printf("getcwd() error");
  }
}

void help() {
  printf("This is John's Unix shell program!\n");
  printf("Here are the following commands:\n");
  printf("cd, mkdir, exit, !!\n");
  printf("You can also use pipes (|) and command redirections!\n");
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
  // *argv = NULL;
}

void process_pipe_cmds(char **argv1, char **argv2) {
  int p[2];

  if (pipe(p) < 0) {
    perror("Pipe failed");
    exit(1);
  }

  int pid1 = fork();

  if (pid1 < 0) {
    perror("Fork for first command failed");
    exit(1);
  }

  if (pid1 == 0) {
    dup2(p[1], STDOUT_FILENO);
    close(p[0]);
    close(p[1]);

    if (execvp(argv1[0], argv1) < 0) {
      printf("Execution of first command failed");
      exit(1);
    }
  }

  int pid2 = fork();

  if (pid2 < 0) {
    printf("Fork for second command failed");
    exit(1);
  }

  if (pid2 == 0) {
    dup2(p[0], STDIN_FILENO);
    close(p[1]);
    close(p[0]);

    if (execvp(argv2[0], argv2) < 0) {
      printf("Execution of second command failed");
      exit(1);
    }
  }

  close(p[0]);
  close(p[1]);

  waitpid(pid1, NULL, 0);
  waitpid(pid2, NULL, 0);
}

void execute(char **argv) {
  pid_t pid;
  int status;

  // Check for any pipes in the argument list,
  char **pipe_pos = argv;
  while (*pipe_pos != NULL && strcmp(*pipe_pos, "|") != 0) {
    pipe_pos++;
  }

  if (*pipe_pos != NULL) {
    *pipe_pos = NULL;
    char **argv2 = pipe_pos + 1;
    process_pipe_cmds(argv, argv2);
  } else {
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
      waitpid(pid, &status, 0);
    }
  }
}

int main(int arg, char *argv[]) {
  char line[MAX_BUFFER_SIZE];
  char *args[MAX_ARGS_SIZE];

  // Get current working directory
  get_cwd(cwd, sizeof(cwd));

  while (1) {
    printf("theshell> ");
    fgets(line, MAX_BUFFER_SIZE, stdin);

    parse(line, args);
    args[arr_len(args) - 1] = NULL;

    if (strcmp(args[0], "exit") == 0) {
      exit(0);
    } else if (strcmp(args[0], "help") == 0) {
      help();
      continue;
    } else if (strcmp(args[0], "!!") == 0) {
      if (prev_args[0] == NULL) {
        printf("No previous command supplied\n");
      } else {
        execute(prev_args);
      }
      continue;
    } else if (strcmp(args[0], "cd") == 0) {
      if (args[1] == NULL) {
        printf("No directory provided\n");
        continue;
      }
      if (chdir(args[1]) == 0) {
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
