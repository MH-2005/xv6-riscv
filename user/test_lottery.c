// Test program for Lottery Scheduling (Section 3)
// Creates children with different ticket counts and shows CPU share.

#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int tickets[3] = {10, 50, 100};  // different ticket counts

  printf("=== Lottery Scheduling Test ===\n");
  printf("Creating 3 children with tickets: 10, 50, 100\n");
  printf("Expected: child with 100 tickets gets most CPU time\n");

  for (int i = 0; i < 3; i++) {
    int pid = fork();
    if (pid < 0) {
      printf("fork failed\n");
      exit(1);
    }
    if (pid == 0) {
      // Child: set own tickets
      settickets(tickets[i]);
      printf("Child %d started with %d tickets\n", getpid(), tickets[i]);

      // CPU-bound loop: count iterations (approximate CPU time received)
      volatile long count = 0;
      for (long j = 0; j < 5000000; j++) {
        count++;
      }

      printf("Child %d (tickets=%d) finished after %ld iterations\n",
             getpid(), tickets[i], count);
      exit(0);
    }
  }

  // Parent waits for all children
  for (int i = 0; i < 3; i++) {
    wait(0);
  }

  printf("=== Lottery Test Complete ===\n");
  printf("Check: child with 100 tickets should have finished earliest\n");
  exit(0);
}
