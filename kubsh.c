#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <readline/readline.h>
#include <readline/history.h>

#define HISTORY_FILE ".kubsh_history"

void debug(char *line) {
  printf("%s\n", line);
}

int main() {
  read_history(HISTORY_FILE);

  char *input;
  
  while (true) {
    input = readline("$ ");

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
