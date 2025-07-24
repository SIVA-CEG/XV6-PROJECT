#include "types.h"
#include "stat.h"
#include "user.h"

int main() {
    int pid;
    
    // Test vfork
    printf(1, "Before vfork\n");

    pid = vfork();
    
    if (pid < 0) {
        // vfork failed
        printf(1, "vfork failed\n");
        exit();
    } else if (pid == 0) {
        // Child process
        printf(1, "In child process\n");
        // Exit child process
        exit();
    } else {
        // Parent process
        printf(1, "In parent process, child pid: %d\n", pid);
    }

    exit();
}
