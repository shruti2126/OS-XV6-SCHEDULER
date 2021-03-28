#include "types.h"
#include "stat.h"
#include "user.h"

#define NUMLOOPS 800000000

int main(int argc, char **argv) {
  if (argc != 2) {
    printf(2, "Incorrect number of args: loop - %d args, argv[1] - %s\n", argc,
           argv[1]);
    exit();
  }

  sleep(atoi(argv[1]));

  int i = 0, j = 0;
  while (i < NUMLOOPS) {
    j += i * j + 1;
    i++;
  }

  exit();
}