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
      str[len - 1] = '\0'; // getline() also gets the \n from entering
                           // into terminal, we need to remove it
    }

    pid_t cpid = fork(); // Returns cpid of Child to Parent
                         // and 0 to Child

    // If Fork Fails
    if (cpid < 0) {
      perror("Failed to fork!\n");
      continue; // Restart Loop
    }

    // Parent Process Function
    else if (cpid > 0) {
      int wstatus = 0;
      int wait = waitpid(cpid, &wstatus, 0); // Parent waits until
                                             // Child status changes

      if (wait == -1) {
        perror("Wait Failed\n");
        continue; // Restart Loop
      }
    }
    // Child Process Function
    else {
      int exec = execlp(str, str, NULL);

      if (exec == -1) {
        printf("Failed to Execute File\n");
      }
      break; // Exit Loop and free Child
    }
  }

  free(str);
  return 0;
}
