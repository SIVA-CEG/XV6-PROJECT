#include "types.h"
#include "stat.h"
#include "user.h"

int main() {
  printf(1, "Killing processes with priority 25...\n");
  killpriority(25);
  exit();
}
