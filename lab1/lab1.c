#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char *str = NULL;
  size_t bufsize = 0;
  char *token;
  char *saveptr;

  printf("Type Something:  ");

  ssize_t read = getline(&str, &bufsize, stdin);

  while (strcmp(str, "q\n") != 0) {
    if (read == -1) {
      printf("Error");
    }

    token = strtok_r(str, " ", &saveptr);
    printf("\nTokens:\n");

    while (token) {
      printf("%s \n", token);
      token = strtok_r(NULL, " ", &saveptr);
    }

    printf("Type Something:  ");

    read = getline(&str, &bufsize, stdin);
  }

  free(str);
  return 0;
}
