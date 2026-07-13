#include "kernel/types.h"
#include "user/user.h"
#include "kernel/pinfo.h"
#include "kernel/param.h"

static unsigned int seed = 0;

static unsigned int
rand_ticket(void)
{
  if (seed == 0) {
    seed = (unsigned int)uptime() + 1;
  }
  seed = seed * 1103515245 + 12345;
  return (seed % 100) + 1;
}

int
main(int argc, char *argv[])
{
  int nchildren = 20;
  int pids[20];
  int tickets[20];
  int priorities[20];

  printf("=== Stress / Lottery Mixed Test ===\n");
  printf("Creating %d children with random tickets and priorities...\n", nchildren);

  for (int i = 0; i < nchildren; i++) {
    int tkt = rand_ticket();
    int prio = (i * 17) % 100;

    int pid = fork();
    if (pid < 0) {
      printf("fork failed at child %d\n", i);
      exit(1);
    }
    if (pid == 0) {
      settickets(tkt);
      setpriority(getpid(), prio);

      // Heavy work – long enough to collect many scheduler samples
      volatile long count = 0;
      for (long j = 0; j < 500000000L; j++) {
        count++;
      }
      exit(0);
    } else {
      pids[i] = pid;
      tickets[i] = tkt;
      priorities[i] = prio;
    }
  }

  printf("Letting children run for ~15 seconds...\n");
  pause(150);

  struct pinfo pi[NPROC];
  int n = getpinfo((uint64)pi);
  if (n < 0) {
    printf("getpinfo failed\n");
    exit(1);
  }

  printf("\n=== Mid-run Statistics (tickets vs sched_count) ===\n");
  printf("PID   TICKETS   SCHED_COUNT   PRIORITY\n");
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < nchildren; j++) {
      if (pi[i].pid == pids[j]) {
        printf("%d\t%d\t%d\t\t%d\n",
               pi[i].pid, tickets[j], pi[i].sched_count, priorities[j]);
        break;
      }
    }
  }

  for (int i = 0; i < nchildren; i++) {
    wait(0);
  }

  printf("\n=== Stress Test Complete (No crash/deadlock) ===\n");
  exit(0);
}
