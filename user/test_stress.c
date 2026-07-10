#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int n = 20;
  printf("=== Stress Test: %d children ===\\n", n);

  for (int i = 0; i < n; i++) {
    int pid = fork();
    if (pid < 0) {
      printf("fork failed at %d\\n", i);
      exit(1);
    }
    if (pid == 0) {
      volatile long count = 0;
      for (long j = 0; j < 1500000; j++) {
        count++;
      }
      exit(0);
    } else {
      setpriority(pid, (i * 17) % 100);
    }
  }

  for (int i = 0; i < n; i++) {
    wait(0);
  }

  printf("=== Stress Test Complete: No crash/deadlock ===\\n");
  exit(0);
}
