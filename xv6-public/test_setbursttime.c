#include "types.h"
#include "stat.h"
#include "user.h"

int main() {
  int mypid = getpid();
  setbursttime(mypid, 50);
  printf(1, "Set burst time to 50 for PID %d\n", mypid);
  exit();
}

