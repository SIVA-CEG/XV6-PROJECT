#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"


int ChangePriority(int pid, int priority);

int main(int argc, char *argv[])
{
  int priority, pid;

  if(argc < 3){
    printf(2, "Usage: nice pid priority\n");
    exit();
  }

  pid = atoi(argv[1]);
  priority = atoi(argv[2]);

  if(priority < 0 || priority > 100){
    printf(2, "Invalid priority (0-100)!\n");
    exit();
  }

  printf(1, "Changing priority of pid=%d to %d\n", pid, priority);

  
  ChangePriority(pid, priority);

  exit();
}

