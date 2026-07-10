#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if (argc != 3) {
    printf("usage: chpri pid priority\\n");
    exit(1);
  }
  int pid = atoi(argv[1]);
  int priority = atoi(argv[2]);
  int ret = setpriority(pid, priority);
  if (ret < 0) {
    printf("chpri: failed to set priority\\n");
    exit(1);
  }
  printf("priority of pid %d set to %d\\n", pid, priority);
  exit(0);
}
