#include "types.h"
#include "stat.h"
#include "user.h"

int main() {
  int parent = getparents();
  printf(1, "My parent is: %d\n", parent);
  exit();
}
