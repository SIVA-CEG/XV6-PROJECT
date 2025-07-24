#include "types.h"
#include "stat.h"
#include "user.h"

int main() {
  int i;
  for(i = 0; i < 3; i++) {
    int pid = fork();
    if(pid == 0) {
      int mypid = getpid();
      setpriority(mypid, 30);
      while(1);
    }
  }

  sleep(100); // Give children time to start and set their priorities
  printf(1, "[PARENT] Killing all processes with priority 30...\n");
  killpriority(30);
  sleep(50);
  printf(1, "[PARENT] Done.\n");
  exit();
}

