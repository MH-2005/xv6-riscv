#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int priorities[3] = {10, 30, 50};
  int pids[3];

  printf("=== Priority Scheduling Test ===
");

  for (int i = 0; i < 3; i++) {
    int pid = fork();
    if (pid < 0) {
      printf("fork failed
");
      exit(1);
    }
    if (pid == 0) {
      // Child: CPU-bound work
      volatile int dummy = 0;
      for (int j = 0; j < 2000000; j++) {
        dummy += j;
      }
      printf("Child %d finished
", getpid());
      exit(0);
    } else {
      pids[i] = pid;
    }
  }

  // Parent sets each child's priority via syscall
  for (int i = 0; i < 3; i++) {
    setpriority(pids[i], priorities[i]);
    printf("Parent set priority %d for pid %d
", priorities[i], pids[i]);
  }

  for (int i = 0; i < 3; i++) {
    wait(0);
  }

  printf("=== Priority Test Complete ===
");
  printf("Expected: children with lower priority values finish first
");
  exit(0);
}
