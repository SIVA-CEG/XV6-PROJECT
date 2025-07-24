#include "types.h"
#include "stat.h"
#include "user.h"

int main() {
  int mypid = getpid();
  printf(1, "Setting priority...\n");
  setpriority(mypid, 25);
  int prio = getpriority(mypid);
  printf(1, "New priority: %d\n", prio);
  exit();
}
