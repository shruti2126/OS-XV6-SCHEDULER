#include "types.h"
#include "stat.h"
#include "user.h"
#include "pstat.h"
#include "param.h"

int getIndex(struct pstat *s, int p) {
  for (int i = 0; i < NPROC; i++) {
    if (s->inuse[i]) {
      if (s->pid[i] == p) {
        return i;
      }
    }
  }
  return -1;
}

int main(int argc, char **argv) {
  int pidA, pidB;
  struct pstat stats;

  if (argc != 6) {
    printf(2, "incorrect number of args: schedtest\n");
    exit();
  }

  pidA = fork2(atoi(argv[1]));  // time slice for A
  char *argvA[] = {"loop", argv[2], 0};

  if (pidA < 0) {
    printf(1, "init: fork failed\n");
    exit();
  }

  if (pidA == 0) {
    exec(argvA[0], argvA);
    printf(1, "init: exec loop A failed\n");
    exit();
  }

  pidB = fork2(atoi(argv[3]));  // time slice for B
  char *argvB[] = {"loop", argv[4], 0};

  if (pidB < 0) {
    printf(1, "init: fork failed\n");
    exit();
  }

  if (pidB == 0) {
    exec(argvB[0], argvB);
    printf(1, "init: exec loop B failed\n");
    exit();
  }

  if (pidA > 0 || pidB > 0) {
    sleep(atoi(argv[5]));
    if (getpinfo(&stats) != 0) {
      printf(1, "getpinfo failed.\n");
      exit();
    }
    // Loop stats: stats->pid == pidA, stats->pid == pidA to find index
    int aIndex = getIndex(&stats, pidA);
    int bIndex = getIndex(&stats, pidB);
    if (aIndex == -1 || bIndex == -1) {
      printf(1, "getIndex couldn't find pid, failed: a: %d, b: %d\n", aIndex,
             bIndex);
      exit();
    }

    printf(1, "%d %d\n", stats.compticks[aIndex], stats.compticks[bIndex]);
    wait();
    wait();
    exit();
  }
}
