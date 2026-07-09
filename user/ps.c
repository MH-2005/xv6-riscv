#include "kernel/types.h"
#include "user/user.h"
#include "kernel/pinfo.h"
#include "kernel/param.h"

int
main(int argc, char *argv[])
{
  struct pinfo pi[64];
  int n = getpinfo((uint64)pi);
  if (n < 0) {
    printf("getpinfo failed\n");
    exit(1);
  }
  printf("PID\tSTATE\tPRIORITY\tTICKETS\tNAME\n");
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
    printf("%d\t%s\t%d\t%d\t%s\n", pi[i].pid, state, pi[i].priority, pi[i].tickets, pi[i].name);
  }
  exit(0);
}