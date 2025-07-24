#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;
  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;
  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;
  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

int
sys_uptime(void)
{
  uint xticks;
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int sys_vfork(void) {
  struct proc *np;
  int pid;

  // Debug: Check if already holding the lock
  if (holding(&ptable.lock)) {
    panic("ptable.lock already held");
  }

  cprintf("Acquiring ptable lock...\n");
  acquire(&ptable.lock);
  cprintf("Acquired ptable lock.\n");

  // Step 1: Allocate a new process structure
  if ((np = allocproc()) == 0) {
    cprintf("Failed to allocate process\n");
    release(&ptable.lock); // Release lock in case of failure
    return -1;
  }

  // Step 2: Set up shared memory for parent and child
  np->pgdir = myproc()->pgdir; // Use myproc() to get the current process' pgdir

  // Step 3: Mark the child as runnable and set the state
  np->state = RUNNABLE;

  pid = np->pid; // Store the process ID for the new child

  // Step 4: Block the parent and let the child run
  myproc()->state = SLEEPING;  // Parent will be put to sleep
  sched();  // Schedule the next process (child)

  cprintf("Releasing ptable lock...\n");
  release(&ptable.lock);  // Release lock before returning
  cprintf("Released ptable lock.\n");

  return pid; // Return child PID
}

int sys_test_lock(void) {
  cprintf("Acquiring ptable lock...\n");
  acquire(&ptable.lock);
  cprintf("Acquired ptable lock.\n");

  // Perform some simple operations with the process table (like allocating a process)
  struct proc *np = allocproc();
  if (np == 0) {
    release(&ptable.lock);
    return -1;
  }

  cprintf("Releasing ptable lock...\n");
  release(&ptable.lock);
  cprintf("Released ptable lock.\n");

  return 0;
}


int sys_ChangePriority(void)
{
  int pid, priority;

  if (argint(0, &pid) < 0 || argint(1, &priority) < 0)
    return -1;

  return ChangePriority(pid, priority);
}

int
sys_getpriority(void) {
  int pid;
  struct proc *p;

  if(argint(0, &pid) < 0)
    return -1;

  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++) {
    if(p->pid == pid) {
      return p->priority;
    }
  }
  return -1;
}

int
sys_setpriority(void) {
  int pid, prio;
  struct proc *p;

  if(argint(0, &pid) < 0 || argint(1, &prio) < 0)
    return -1;

  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++) {
    if(p->pid == pid) {
      p->priority = prio;
      return 0;
    }
  }
  return -1;
}

int
sys_getparents(void) {
  if(myproc()->parent)
    return myproc()->parent->pid;
  return -1;
}


int
sys_getchildren(void) {
  int pid;
  struct proc *p;
  int count = 0;

  if(argint(0, &pid) < 0)
    return -1;

  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++) {
    if(p->parent && p->parent->pid == pid) {
      cprintf("Child PID: %d\n", p->pid);
      count++;
    }
  }
  return count;
}

int
sys_killpriority(void) {
  int prio;
  struct proc *p;

  if(argint(0, &prio) < 0)
    return -1;

  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++) {
    if(p->priority == prio && p->pid > 2) { // avoid init/sh
      kill(p->pid);
    }
  }
  return 0;
}

int
sys_setbursttime(void) {
  int pid, bt;
  struct proc *p;

  if(argint(0, &pid) < 0 || argint(1, &bt) < 0)
    return -1;

  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++) {
    if(p->pid == pid) {
      p->burst_time = bt;
      return 0;
    }
  }
  return -1;
}

int
sys_getnumsyscalls(void) {
  int pid;
  struct proc *p;

  if(argint(0, &pid) < 0)
    return -1;

  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++) {
    if(p->pid == pid) {
      return p->num_syscalls;
    }
  }
  return -1;
}
