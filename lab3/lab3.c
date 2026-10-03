#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void add_to_history(char *history[], int *count, char *new_input) {
  if (*count < 5) {
    history[*count] = new_input;
    (*count)++;
  } else {
    free(history[0]);

    for (int i = 0; i < 5; i++) {
      history[i] = history[i + 1];
    }

    history[4] = new_input;
  }
}

void print_history(char *history[], int count) {
  for (int i = 0; i < count; i++) {
    printf("%s", history[i]);
  }
}

void free_history(char *history[], int count) {
  for (int i = 0; i < count; i++) {
    free(history[i]);
  }
}

int main() {
  char *history[5] = {NULL};
  int count = 0;

  char *line = NULL;
  size_t len = 0;
  ssize_t nread;

  while (1) {
    printf("Enter input: ");

    nread = getline(&line, &len, stdin);
    if (nread == -1) {
      free(line);
      break;
    }

    char *current_input = line;

    line = NULL;
    len = 0;

    add_to_history(history, &count, current_input);

    if (strcmp(current_input, "print\n") == 0) {
      print_history(history, count);
    }
  }

  free_history(history, count);
  return 0;
}
