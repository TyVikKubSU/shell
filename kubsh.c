#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <readline/readline.h>

void debug(char *line) {
  printf("%s\n", line);
}

int main() {
  char *input;
  
  while (true) {
    input = readline("$ ");

    if (!strcmp(input, "\\q")) {
      break;
    } else if (strncmp(input, "debug ", 5) == 0) {
      debug(input);
    } else {
      printf("%s: command not found\n", input);
    }
  }
  
  return 0;
}
