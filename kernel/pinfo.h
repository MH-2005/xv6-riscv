#ifndef PINFO_H
#define PINFO_H

struct pinfo {
  int pid;
  int state;
  int priority;
  int tickets;
  int sched_count;
  char name[16];
};

#endif