#include "types.h"
#include "stat.h"
#include "user.h"

int main() {
  int mypid = getpid();
  int prio = getpriority(mypid);
  printf(1, "Current priority: %d\n", prio);
  exit();
}
