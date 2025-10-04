#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>

#include <readline/readline.h>
#include <readline/history.h>

#define HISTORY_FILE ".kubsh_history"

sig_atomic_t signal_received = 0;

void debug(char *line) {
  printf("%s\n", line);
}

void sig_handler(int signum) {
  signal_received = signum;
  printf("Configuration reloaded");
}

int main() {
  signal(SIGHUP, sig_handler);
  read_history(HISTORY_FILE);

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

    if (!strcmp(input, "\\q")) {
      break;
    } else if (strncmp(input, "debug ", 5) == 0) {
      debug(input);
    } else {
      printf("%s: command not found\n", input);
    }
    free(input);
  }

  write_history(HISTORY_FILE);
  
  return 0;
}
