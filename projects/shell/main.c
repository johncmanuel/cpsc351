// John Carlo Manuel
// CPSC 351
// Unix Shell Project
// 10-5-24

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_ARGS_SIZE 64
#define MAX_BUFFER_SIZE 1024

// Track prev args and cwd
char *prev_args[MAX_ARGS_SIZE];
char cwd[MAX_BUFFER_SIZE];

char *builtin_cmds[] = {"cd", "exit", "help", "!!", "mkdir"};

// Declare builtin cmds functions
int run_cd(char **argv);
int run_help();
void run_exit();
int run_prev_cmd();
int run_mkdir(char **argv);
int run_shell(char **argv);

int arr_len(char **arr) {
  int length = 0;
  while (arr[length] != NULL) {
    length++;
  }
  return length;
}

void save_prev_args(char **src, char **dst) {
  int i = 0;
  while (src[i] != NULL && i < MAX_ARGS_SIZE) {
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

// Checks for ECHO at end of the argument list and
// prints SPACE and PIPE if those are found. Also print
// each argument on a new line
void check_for_echo(char **argv) {
  int length = arr_len(argv);
  if (strcmp(argv[length - 1], "ECHO") == 0) {
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
}

void parse_user_input(char *line, char **argv) {
  int num_args = 0;
  char *token = strtok(line, " \n");

  while (token != NULL && num_args < MAX_ARGS_SIZE) {
    argv[num_args++] = token;
    token = strtok(NULL, " \n");
  }
  argv[num_args] = NULL;
}

// Supports only two commands
void process_pipe_cmds(char **argv1, char **argv2) {
  int p[2];

  if (pipe(p) < 0) {
    perror("Pipe failed");
    exit(1);
  }

  pid_t pid1 = fork();

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

  pid_t pid2 = fork();

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

int process_redirection(char **argv) {
  for (int i = 0; argv[i] != NULL; i++) {
    int fd = 0;
    if (strcmp(argv[i], ">") == 0 || strcmp(argv[i], ">>") == 0 ||
        strcmp(argv[i], "<") == 0 || strcmp(argv[i], "<<") == 0) {
      if (argv[i + 1] == NULL) {
        printf("No file specified for redirection\n");
        return -1;
      }
      if (strcmp(argv[i], "<") == 0 || strcmp(argv[i], "<<") == 0) {
        fd = open(argv[i + 1], O_RDONLY);
        dup2(fd, STDIN_FILENO);
      } else {
        fd = open(argv[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        dup2(fd, STDOUT_FILENO);
      }
      if (fd < 0) {
        printf("Failed to open file");
        return -1;
      }
      close(fd);
      argv[i] = NULL;
      i++;
    }
  }
  return 0;
}

void execute_args(char **argv) {

  // Check for any pipes in the argument list and increment its position and
  // count once found
  char **pipe_pos = argv;
  while (*pipe_pos != NULL && strcmp(*pipe_pos, "|") != 0) {
    pipe_pos++;
  }

  if (*pipe_pos != NULL) {
    *pipe_pos = NULL;
    char **argv2 = pipe_pos + 1;
    process_pipe_cmds(argv, argv2);
    return;
  }

  pid_t pid = fork();
  int status;

  if (pid < 0) {
    printf("Forking child process failed\n");
    exit(1);
  } else if (pid == 0) {
    if (process_redirection(argv) < 0) {
      exit(1);
    }
    if (execvp(argv[0], argv) < 0) {
      printf("Execution failed\n");
      exit(1);
    }
  } else {
    waitpid(pid, &status, 0);
  }
}

int run_cd(char **argv) {
  if (argv[1] == NULL) {
    printf("No directory provided\n");
    return 1;
  }
  if (chdir(argv[1]) == 0) {
    get_cwd(cwd, sizeof(cwd));
  } else {
    printf("Failed to change directory\n");
  }
  return 1;
}

int run_prev_cmd() {
  if (prev_args[0] == NULL) {
    printf("No previous command supplied\n");
  } else {
    run_shell(prev_args);
  }
  return 1;
}

int run_mkdir(char **argv) {
  if (argv[1] == NULL) {
    printf("No directory provided\n");
    return 1;
  }
  if (mkdir(argv[1], 0700) != 0) {
    printf("Failed to create directory\n");
  }
  return 1;
}

void run_exit() { exit(0); }

int run_help() {
  printf("This is jcsh, John Carlo's Shell!\n");
  printf("Here are the following commands:\n");
  printf("cd, mkdir, exit, !!\n");
  printf(
      "You can also use pipes (|) and command redirections (>, >>, <, <<)!\n");
  printf("Type 'exit' to exit the shell\n");
  return 1;
}

int run_shell(char **argv) {
  if (argv[0] == NULL) {
    return 1;
  }
  check_for_echo(argv);

  // Remove ECHO from end of args before executing
  int length = arr_len(argv);
  if (strcmp(argv[length - 1], "ECHO") == 0) {
    argv[length - 1] = NULL;
  }

  // Check for builtin commands
  for (int i = 0; i < sizeof(builtin_cmds) / sizeof(char *); i++) {
    if (strcmp(argv[0], builtin_cmds[i]) == 0) {
      int status = 0;
      if (strcmp(argv[0], "cd") == 0) {
        status = run_cd(argv);
      } else if (strcmp(argv[0], "exit") == 0) {
        run_exit();
      } else if (strcmp(argv[0], "help") == 0) {
        status = run_help();
      } else if (strcmp(argv[0], "!!") == 0) {
        status = run_prev_cmd();
      } else if (strcmp(argv[0], "mkdir") == 0) {
        status = run_mkdir(argv);
      }
      return status;
    }
  }

  return 0;
}

int main(int arg, char *argv[]) {
  char line[MAX_BUFFER_SIZE];
  char *args[MAX_ARGS_SIZE];

  // Get current working directory
  get_cwd(cwd, sizeof(cwd));

  while (1) {
    printf("jcsh > ");

    char *chars = fgets(line, MAX_BUFFER_SIZE, stdin);
    if (chars == NULL) {
      fprintf(stderr, "Error reading input\n");
      return 1;
    }

    line[strlen(line) - 1] = '\0';
    parse_user_input(line, args);

    if (run_shell(args) == 1) {
      save_prev_args(args, prev_args);
      continue;
    }

    execute_args(args);

    // Track previous command before execution
    save_prev_args(argv, prev_args);
  }
  return 0;
}
