#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include "traps.h"
#include "spinlock.h"

// Interrupt descriptor table (shared by all CPUs).
struct gatedesc idt[256];
extern uint vectors[];  // in vectors.S: array of 256 entry pointers
struct spinlock tickslock;
uint ticks;

pte_t* walkpgdir(pde_t *pgdir, const void *va, int alloc); // Add walkpgdir prototype

void
tvinit(void)
{
  int i;

  for(i = 0; i < 256; i++)
    SETGATE(idt[i], 0, SEG_KCODE<<3, vectors[i], 0);
  SETGATE(idt[T_SYSCALL], 1, SEG_KCODE<<3, vectors[T_SYSCALL], DPL_USER);

  initlock(&tickslock, "time");
}

void
idtinit(void)
{
  lidt(idt, sizeof(idt));
}

//PAGEBREAK: 41
void
trap(struct trapframe *tf)
{
  if(tf->trapno == T_SYSCALL){
    if(myproc()->killed)
      exit();
    myproc()->tf = tf;
    syscall();
    if(myproc()->killed)
      exit();
    return;
  }

  if (tf->trapno == T_PGFLT) {
    uint addr = rcr2();
    pte_t *pte = walkpgdir(myproc()->pgdir, (void *)addr, 0);

    if (!pte || !(*pte & PTE_P)) {
      cprintf("page fault: invalid access at addr 0x%x\n", addr);
      myproc()->killed = 1;
      return;
    }

    if (*pte & PTE_COW) {
      // Allocate new page
      char *mem = kalloc();
      if (!mem) {
        cprintf("COW kalloc failed\n");
        myproc()->killed = 1;
        return;
      }

      // Copy old page contents
      memmove(mem, (char*)P2V(PTE_ADDR(*pte)), PGSIZE);

      // Remap with write permissions
      *pte = V2P(mem) | PTE_U | PTE_W | PTE_P;

      lcr3(V2P(myproc()->pgdir)); // TLB flush
      return;
    } else {
      cprintf("page fault not handled (not COW)\n");
      myproc()->killed = 1;
      return;
    }
  }

  switch(tf->trapno){
  case T_IRQ0 + IRQ_TIMER:
    if(cpuid() == 0){
      acquire(&tickslock);
      ticks++;
      wakeup(&ticks);
      release(&tickslock);
    }
    lapiceoi();
    break;
  case T_IRQ0 + IRQ_IDE:
    ideintr();
    lapiceoi();
    break;
  case T_IRQ0 + IRQ_IDE+1:
    // Bochs generates spurious IDE1 interrupts.
    break;
  case T_IRQ0 + IRQ_KBD:
    kbdintr();
    lapiceoi();
    break;
  case T_IRQ0 + IRQ_COM1:
    uartintr();
    lapiceoi();
    break;
  case T_IRQ0 + 7:
  case T_IRQ0 + IRQ_SPURIOUS:
    cprintf("cpu%d: spurious interrupt at %x:%x\n",
            cpuid(), tf->cs, tf->eip);
    lapiceoi();
    break;

  //PAGEBREAK: 13
  default:
    if(myproc() == 0 || (tf->cs&3) == 0){
      // In kernel, must be our bug.
      cprintf("unexpected trap %d from cpu %d eip %x (cr2=0x%x)\n",
              tf->trapno, cpuid(), tf->eip, rcr2());
      panic("trap");
    }
    // In user space: kill the process.
    cprintf("pid %d %s: trap %d err %d on cpu %d "
            "eip 0x%x addr 0x%x--kill proc\n",
            myproc()->pid, myproc()->name, tf->trapno,
            tf->err, cpuid(), tf->eip, rcr2());
    myproc()->killed = 1;
  }

  // Force process exit if it has been killed and is in user space.
  if(myproc() && myproc()->killed && (tf->cs&3) == DPL_USER)
    exit();

  // Yield CPU on timer interrupt.
  if(myproc() && myproc()->state == RUNNING &&
     tf->trapno == T_IRQ0+IRQ_TIMER)
    yield();

  // Check for kill after yield.
  if(myproc() && myproc()->killed && (tf->cs&3) == DPL_USER)
    exit();
}
