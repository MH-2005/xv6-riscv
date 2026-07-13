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
  // ---------- Reliable detection of active scheduler ----------
  int lottery_active = 0;

  int p[2];
  if (pipe(p) < 0) {
    printf("pipe failed\n");
    exit(1);
  }

  int test_pid = fork();
  if (test_pid < 0) {
    printf("fork failed\n");
    exit(1);
  }
  if (test_pid == 0) {
    close(p[0]);

    settickets(100);
    volatile int dummy = 0;
    for (int j = 0; j < 5000000; j++)
      dummy++;

    // Static so it lives in BSS, not on the tiny user stack
    static struct pinfo pi[NPROC];
    int n = getpinfo((uint64)pi);
    int my_sched = 0;
    if (n >= 0) {
      for (int i = 0; i < n; i++) {
        if (pi[i].pid == getpid()) {
          my_sched = pi[i].sched_count;
          break;
        }
      }
    }

    write(p[1], &my_sched, sizeof(my_sched));
    close(p[1]);
    exit(0);
  } else {
    close(p[1]);
    int child_sched = 0;
    if (read(p[0], &child_sched, sizeof(child_sched)) != sizeof(child_sched)) {
      printf("read from pipe failed\n");
    }
    close(p[0]);
    wait(0);

    if (child_sched > 0)
      lottery_active = 1;
  }
  // ------------------------------------------------------------------------

  int nchildren = 20;
  int pids[20];
  int tickets[20];
  int priorities[20];

  if (lottery_active)
    printf("=== Stress Test (Lottery mode detected) ===\n");
  else
    printf("=== Stress Test (Priority / Round‑Robin mode) ===\n");
  printf("Creating %d children...\n", nchildren);

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

      volatile long count = 0;
      for (long j = 0; j < 50000000L; j++) {
        count++;
      }
      exit(0);
    } else {
      pids[i] = pid;
      tickets[i] = tkt;
      priorities[i] = prio;
    }
  }

  pause(80);   // ~8 seconds

  // Static again – avoid stack overflow
  static struct pinfo pi[NPROC];
  int n = getpinfo((uint64)pi);
  if (n < 0) {
    printf("getpinfo failed\n");
    for (int i = 0; i < nchildren; i++) wait(0);
    exit(1);
  }

  if (lottery_active) {
    printf("\n=== Mid-run Statistics (Lottery mode) ===\n");
    printf("PID   TICKETS   SCHED_COUNT   PRIORITY\n");
  } else {
    printf("\n=== Mid-run Statistics (Priority mode) ===\n");
    printf("PID   PRIORITY\n");
  }

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < nchildren; j++) {
      if (pi[i].pid == pids[j]) {
        if (lottery_active) {
          printf("%d\t%d\t%d\t\t%d\n",
                 pi[i].pid, tickets[j], pi[i].sched_count, priorities[j]);
        } else {
          printf("%d\t%d\n", pi[i].pid, priorities[j]);
        }
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
