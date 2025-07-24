#include "types.h"
#include "stat.h"
#include "user.h"

int main(void) {
  char *p = sbrk(4096);
  p[0] = 'A';

  int pid = vfork();
  if(pid == 0){
    p[0] = 'B';
    printf(1, "Child sees: %c\n", p[0]);
    exit();
  } else {
    wait();
    printf(1, "Parent sees: %c\n", p[0]);
  }

  exit();
}
