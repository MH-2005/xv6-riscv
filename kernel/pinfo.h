#ifndef PINFO_H
#define PINFO_H

// pinfo is in a separate header (not proc.h) so both kernel and user space
// can include it without pulling in kernel-only types/definitions.
struct pinfo {
  int pid;
  int state;
  int priority;     // 0=highest ... 100=lowest
  int tickets;      // for lottery scheduling (default 1)
  int sched_count;  // how many times this proc got CPU (lottery stats)
  char name[16];
};

#endif
