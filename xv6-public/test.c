// In user/test.c or a similar test file
void test_lock() {
  int result = sys_test_lock();
  if (result == 0) {
    cprintf("Test lock successful\n");
  } else {
    cprintf("Test lock failed\n");
  }
}
