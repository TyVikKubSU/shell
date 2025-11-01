#define FUSE_USE_VERSION 31
#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "vfs.h"

#define HISTORY_FILE ".kubsh_history"

sig_atomic_t signal_received = 0;

void debug(char *line) {
  printf("%s\n", line);
}

void sig_handler(int signum) {
  signal_received = signum;
  printf("Configuration reloaded");
}

void fork_exec(char *full_path, int argc, char **argv) {
  int pid = fork();
  if (pid == 0) {
    // child process
    execv(full_path, argv);
    perror("execve");
  } else {
    int status;
    waitpid(pid, &status, 0);
  }
}

int is_executable(const char *path) {
  return access(path, X_OK) == 0;
}

void create_user(const char *command) {
  char *const argv[] = {"useradd", "-m", "-s", "/bin/bash", "qqq", NULL};
  int err = execv("/usr/sbin/useradd", argv);
  printf("%d", err);
}

char *find_in_path(const char *command) {
  char *path_env = getenv("PATH");
  // разбиваем PATH на массив
  // проходимся по каждому элементу массива
    // соединяем, чтобы получился полный путь
    //if is_executable(полный_путь) {
    //  return полный_путь
    //}
  // возвращаем NULL
}

int main() {
	rl_clear_signals();
  signal(SIGHUP, sig_handler);
  read_history(HISTORY_FILE);
  start_users_vfs("users");

  char *input;
  
  while (true) {
    input = readline("$ ");

    if (signal_received) {
      signal_received = 0;
      continue;
    }

    if (input == NULL) {
      break;
    }
    add_history(input);

    char *argv[11];
    int argc = 0;
    char *token = strtok(input, " ");
    while (token != NULL && argc < 10) {
      argv[argc++] = token;
      token = strtok(NULL, " ");
    }
    argv[argc] = NULL;
    char *cmd_path = find_in_path(argv[0]);

    if (!strcmp(input, "\\q")) {
      break;
    } else if (strncmp(input, "debug ", 5) == 0) {
      debug(input);
    } else if (strncmp(input, "\\u ",2) == 0) {
      create_user(input);
    } else if (cmd_path) {
      fork_exec(cmd_path, argc, argv);
    } else {
      printf("%s: command not found\n", input);
    }
    free(input);
  }

  write_history(HISTORY_FILE);
  stop_users_vfs();
  
  return 0;
}
