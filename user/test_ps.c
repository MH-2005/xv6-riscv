#include "kernel/types.h"
#include "user/user.h"
#include "kernel/pinfo.h"
#include "kernel/param.h"

static void
print_pad(int n)
{
  if (n < 10)
    printf("  %d", n);
  else if (n < 100)
    printf(" %d", n);
  else
    printf("%d", n);
}

int
main(int argc, char *argv[])
{
  struct pinfo pi[NPROC];
  int n = getpinfo((uint64)pi);
  if (n < 0) {
    printf("getpinfo failed\n");
    exit(1);
  }

  printf("PID   STATE     PRI  TKT  SCH NAME\n");

  for (int i = 0; i < n; i++) {
    char *state;
    switch (pi[i].state) {
      case 0: state = "UNUSED"; break;
      case 1: state = "USED"; break;
      case 2: state = "SLEEPING"; break;
      case 3: state = "RUNNABLE"; break;
      case 4: state = "RUNNING"; break;
      case 5: state = "ZOMBIE"; break;
      default: state = "UNKNOWN"; break;
    }

    // PID
    if (pi[i].pid < 10)
      printf("  %d   ", pi[i].pid);
    else if (pi[i].pid < 100)
      printf(" %d   ", pi[i].pid);
    else
      printf("%d   ", pi[i].pid);

    // STATE
    printf("%s", state);
    int slen = strlen(state);
    for (int j = slen; j < 9; j++)
      printf(" ");

    // PRI, TKT, SCH
    print_pad(pi[i].priority);
    printf("  ");
    print_pad(pi[i].tickets);
    printf("  ");
    print_pad(pi[i].sched_count);
    printf("  ");

    // NAME
    printf("%s\n", pi[i].name);
  }

  exit(0);
}