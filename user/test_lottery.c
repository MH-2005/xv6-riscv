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
      volatile long count = 0;
      for (long j = 0; j < 20000000; j++) {
        count++;
      }
      exit(0);
    } else {
      pids[i] = pid;
    }
  }

  volatile long w = 0;
  for (long j = 0; j < 5000000; j++) w++;

  struct pinfo pi[NPROC];
  int n = getpinfo((uint64)pi);
  
  printf("\n=== Scheduling Statistics ===\n");
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < 3; j++) {
      if (pi[i].pid == pids[j]) {
        printf("PID %d: tickets=%d, sched_count=%d\n",
               pi[i].pid, pi[i].tickets, pi[i].sched_count);
      }
    }
  }

  for (int i = 0; i < 3; i++) {
    wait(0);
  }

  printf("\n=== Lottery Test Complete ===\n");
  printf("Expected: sched_count roughly proportional to tickets\n");
  exit(0);
}
