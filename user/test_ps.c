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
    printf("test_ps: getpinfo failed
");
    exit(1);
  }
  
  printf("=== getpinfo Test ===
");
  printf("Total processes: %d
", n);
  printf("PID	STATE		PRIORITY	TICKETS	NAME
");
  
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
    printf("%d	%s	%d		%d	%s
", 
           pi[i].pid, state, pi[i].priority, pi[i].tickets, pi[i].name);
  }
  
  // Verify default values
  int ok = 1;
  for (int i = 0; i < n; i++) {
    if (pi[i].priority != 50 || pi[i].tickets != 1) {
      printf("ERROR: Process %d has wrong defaults (priority=%d, tickets=%d)
",
             pi[i].pid, pi[i].priority, pi[i].tickets);
      ok = 0;
    }
  }
  
  if (ok) {
    printf("
All processes have correct defaults: priority=50, tickets=1
");
  } else {
    printf("
Some processes have incorrect defaults
");
  }
  
  exit(ok ? 0 : 1);
}
