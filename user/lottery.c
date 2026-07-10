#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if (argc != 2) {
    printf("usage: lottery tickets\\n");
    exit(1);
  }
  int n = atoi(argv[1]);
  if (settickets(n) < 0) {
    printf("lottery: invalid ticket count\\n");
    exit(1);
  }
  printf("tickets set to %d\\n", n);
  exit(0);
}
