#include "types.h"
#include "stat.h"
#include "user.h"

int main() {
  int pid = fork();

  if (pid == 0) {

    int parent = getparents();
    printf(1, "\n[CHILD] My parent is PID %d\n", parent);

    int mypid = getpid();
    printf(1, "[CHILD] My PID is %d\n", mypid);

    int prio = getpriority(mypid);
    printf(1, "[CHILD] My current priority is %d\n", prio);

    printf(1, "[CHILD] Setting my priority to 25...\n");
    setpriority(mypid, 25);

    prio = getpriority(mypid);
    printf(1, "[CHILD] New priority: %d\n", prio);

    setbursttime(mypid, 50);


    int sc = getnumsyscalls(mypid);
    printf(1, "[CHILD] I made %d syscalls so far.\n", sc);

    sleep(100);
    exit();
  } else {

    sleep(10);
    int mypid = getpid();
    printf(1, "\n[PARENT] My PID is %d\n", mypid);
    printf(1, "[PARENT] My children are:\n");
    getchildren(mypid);

    int prio = getpriority(pid);
    printf(1, "[PARENT] Child's current priority: %d\n", prio);

    printf(1, "[PARENT] Killing all processes with priority 25 (child)...\n");
    killpriority(25);

    wait();
  }

  exit();
}
