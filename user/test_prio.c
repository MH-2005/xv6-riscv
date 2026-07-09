#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int priorities[3] = {10, 30, 50};
  int pids[3];

  printf("=== Priority Scheduling Test ===\n");

  for (int i = 0; i < 3; i++) {
    int pid = fork();
    if (pid < 0) {
      printf("fork failed\n");
      exit(1);
    }
    if (pid == 0) {
      // Child: busy loop
      volatile int dummy = 0;
      for (int j = 0; j < 2000000; j++) {
        dummy += j;
      }
      printf("Child %d finished\n", getpid());
      exit(0);
    } else {
      pids[i] = pid;
    }
  }

  // Parent sets priorities for each child
  for (int i = 0; i < 3; i++) {
    setpriority(pids[i], priorities[i]);
    printf("Parent set priority %d for pid %d\n", priorities[i], pids[i]);
  }

  for (int i = 0; i < 3; i++) {
    wait(0);
  }

  printf("=== Priority Test Complete ===\n");
  printf("Expected: children with lower priority values finish first\n");
  exit(0);
}
