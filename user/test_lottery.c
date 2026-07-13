#include "kernel/types.h"
#include "user/user.h"
#include "kernel/pinfo.h"
#include "kernel/param.h"

int
main(int argc, char *argv[])
{
  int tickets[3] = {10, 30, 100};
  int pids[3];

  printf("=== Lottery Scheduling Statistical Test ===\n");
  printf("Creating 3 children with tickets: 10, 30, 100\n");

  for (int i = 0; i < 3; i++) {
    int pid = fork();
    if (pid < 0) {
      printf("fork failed\n");
      exit(1);
    }
    if (pid == 0) {
      settickets(tickets[i]);

      // Extremely long CPU-bound loop – will generate many scheduling events
      volatile long count = 0;
      for (long j = 0; j < 2000000000L; j++) {
        count++;
      }
      exit(0);
    } else {
      pids[i] = pid;
    }
  }

  // Let the children run and accumulate significant statistics
  printf("Letting children run for ~15 seconds...\n");
  pause(150);

  struct pinfo pi[NPROC];
  int n = getpinfo((uint64)pi);
  if (n < 0) {
    printf("getpinfo failed\n");
    exit(1);
  }

  int counts[3] = {0, 0, 0};
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < 3; j++) {
      if (pi[i].pid == pids[j]) {
        counts[j] = pi[i].sched_count;
      }
    }
  }

  int total = counts[0] + counts[1] + counts[2];
  printf("\n=== Mid-run Scheduling Statistics ===\n");
  for (int j = 0; j < 3; j++) {
    int percent = total ? (counts[j] * 100) / total : 0;
    printf("PID %d: tickets=%d, sched_count=%d (~%d%% of CPU)\n",
           pids[j], tickets[j], counts[j], percent);
  }
  printf("Expected: sched_count proportional to tickets (10:30:100 ~ 7%%:21%%:71%%)\n");

  // Wait for all children to finish
  for (int i = 0; i < 3; i++) {
    wait(0);
  }

  printf("\n=== Lottery Test Complete ===\n");
  exit(0);
}
