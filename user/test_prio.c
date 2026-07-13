#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int priorities[3] = {10, 30, 50};
  int pids[3];

  printf("=== Priority Scheduling Test ===\n");

  // Fork children
  for (int i = 0; i < 3; i++) {
    int pid = fork();
    if (pid < 0) {
      printf("fork failed\n");
      exit(1);
    }
    if (pid == 0) {
      // Child: only set priority and compute – no prints!
      setpriority(getpid(), priorities[i]);

      volatile int dummy = 0;
      for (int j = 0; j < 1000000; j++) {
        dummy += j;
      }

      exit(0);
    } else {
      pids[i] = pid;
    }
  }

  // Parent: set priority for each child (though child already did, but it's safe)
  for (int i = 0; i < 3; i++) {
    setpriority(pids[i], priorities[i]);
    printf("Parent set priority %d for pid %d\n", priorities[i], pids[i]);
  }

  // Wait for each child and report in order of completion
  for (int i = 0; i < 3; i++) {
    int cpid = wait(0);
    // Find which priority this child had
    for (int j = 0; j < 3; j++) {
      if (pids[j] == cpid) {
        printf("Child pid %d (priority %d) finished\n", cpid, priorities[j]);
        break;
      }
    }
  }

  printf("=== Priority Test Complete ===\n");
  printf("Expected: children with lower priority values finish first\n");
  exit(0);
}
