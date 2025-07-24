#include "types.h"
#include "stat.h"
#include "user.h"

int main() {
  int mypid = getpid();
  int sc = getnumsyscalls(mypid);
  printf(1, "Number of syscalls made: %d\n", sc);
  exit();
}
