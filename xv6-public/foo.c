#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[]) {
    int k, n, id; 
    double x = 0, z;

    // Default value if argument is not provided
    if (argc < 2)
        n = 1;  
    else
        n = atoi(argv[1]);

    // Set bounds for n
    if (n < 0 || n > 100)
        n = 2; 

    // Create child processes
    for (k = 0; k < n; k++) {
        id = fork(); 

        if (id < 0) {
            printf(1, "%d failed in fork!\n", getpid()); 
        } else if (id > 0) {  
            // Parent
            printf(1, "Parent %d creating child %d\n", getpid(), id);
            wait();  // Parent waits for each child to terminate
        } else {  
            // Child
            printf(1, "Child %d created\n", getpid()); 
            for (z = 0; z < 8000000.0; z += 0.001) {
                x = x + 3.14 * 69.69; // CPU work (useless calculation to consume CPU time)
            }
            exit(); // Exit child process after work is done
        }
    }

    exit(); // Exit parent process after all children are done
}
