#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>

int main() {
  size_t n = 0;
  char *str = NULL;

  while (1) {
    printf("Enter programs to run: ");
    ssize_t len = getline(&str, &n, stdin);

    if (len == -1) {
      perror("Failed to read line!\n");
      continue;
    }
    if (len > 0 && str[len - 1] == '\n') {
      str[len - 1] = '\0';
    }

    pid_t cpid = fork();

    if (cpid < 0) {
      perror("Failed to fork!\n");
      continue;
    }

    else if (cpid > 0) {
      int wait = waitpid(cpid, NULL, 0);

      if (wait == -1) {
        perror("Wait Failed\n");
        continue;
      }
    }

    else {
      int exec = execlp(str, str);

      if (exec == -1) {
        printf("Failed to Execute File\n");
      }

      break;
    }
  }

  free(str);
  return 0;
}
