#include "kernel/types.h"
#include "user/user.h"
#include "kernel/pinfo.h"
#include "kernel/param.h"

int
main(int argc, char *argv[])
{
  struct pinfo pi[NPROC];
  int n = getpinfo((uint64)pi);
  if (n < 0) {
    printf("getpinfo failed
");
    exit(1);
  }
  printf("PID	STATE	PRI	TKT	SCH	NAME
");
  for (int i = 0; i < n; i++) {
    char *state;
    switch (pi[i].state) {
      case 0: state = "UNUSED"; break;
      case 1: state = "USED"; break;
      case 2: state = "SLEEP"; break;
      case 3: state = "RUNBLE"; break;
      case 4: state = "RUN"; break;
      case 5: state = "ZOMBIE"; break;
      default: state = "UNKNWN"; break;
    }
    printf("%d	%s	%d	%d	%d	%s
",
           pi[i].pid, state, pi[i].priority, pi[i].tickets, pi[i].sched_count, pi[i].name);
  }
  exit(0);
}
