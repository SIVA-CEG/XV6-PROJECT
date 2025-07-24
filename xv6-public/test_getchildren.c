#include "types.h"
#include "stat.h"
#include "user.h"

int main() {
  int pid1 = fork();
  if(pid1 == 0) {
    sleep(100);
    exit();
  }

  int pid2 = fork();
  if(pid2 == 0) {
    sleep(100);
    exit();
  }

  sleep(10);
  printf(1, "My children are:\n");
  getchildren(getpid());
  wait();
  wait();
  exit();
}

