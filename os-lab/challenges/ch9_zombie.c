#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t child_pid;
    
    printf("Parent PID: %d\n", getpid());
    printf("Creating a child process that will become a zombie...\n");
    
    child_pid = fork();
    
    if (child_pid == 0) {
        // Child process
        printf("Child PID: %d, Parent PID: %d\n", getpid(), getppid());
        printf("Child: Exiting immediately to become a zombie\n");
        exit(42);  // Exit with code 42
        
    } else if (child_pid > 0) {
        // Parent process
        printf("Parent: Created child with PID %d\n", child_pid);
        printf("\n=== ZOMBIE DEMONSTRATION ===\n");
        printf("The child has exited but parent hasn't called wait() yet.\n");
        printf("In another terminal, run: ps -l | grep %d\n", child_pid);
        printf("You should see the child process with state 'Z' (zombie).\n");
        printf("Also try: ps -eo pid,ppid,state,comm | grep %d\n", child_pid);
        printf("\nParent sleeping for 10 seconds to keep zombie alive...\n");
        
        // Sleep for 10 seconds to demonstrate zombie state
        for (int i = 10; i > 0; i--) {
            printf("Parent: %d seconds remaining (zombie should be visible in ps)\n", i);
            sleep(1);
        }
        
        printf("\n=== ZOMBIE CLEANUP ===\n");
        printf("Parent: Now calling wait() to clean up the zombie...\n");
        
        int status;
        pid_t waited_pid = wait(&status);
        
        if (waited_pid == child_pid) {
            printf("Parent: Successfully reaped child %d\n", waited_pid);
            if (WIFEXITED(status)) {
                printf("Parent: Child exited with code %d\n", WEXITSTATUS(status));
            }
            printf("Parent: Zombie has been cleaned up.\n");
            printf("Run 'ps -l | grep %d' again - the zombie should be gone.\n", child_pid);
        } else {
            printf("Parent: Error reaping child\n");
        }
        
    } else {
        perror("fork failed");
        return 1;
    }
    
    printf("Parent: Demonstration complete\n");
    return 0;
}