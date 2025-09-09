#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    printf("Demonstrating execl vs execv with two child processes\n\n");
    
    // Child A: uses execl
    pid_t pid_a = fork();
    if (pid_a == 0) {
        printf("Child A: Using execl with individual arguments\n");
        execl("/bin/echo", "echo", "one", "two", NULL);
        // If execl fails, this line will execute
        perror("execl failed");
        exit(1);
    } else if (pid_a < 0) {
        perror("fork failed for Child A");
        return 1;
    }
    
    // Child B: uses execv
    pid_t pid_b = fork();
    if (pid_b == 0) {
        printf("Child B: Using execv with argument array\n");
        char *argv_array[] = {"/bin/echo", "one", "two", NULL};
        execv("/bin/echo", argv_array);
        // If execv fails, this line will execute
        perror("execv failed");
        exit(1);
    } else if (pid_b < 0) {
        perror("fork failed for Child B");
        return 1;
    }
    
    // Parent waits for both children
    int status_a, status_b;
    
    printf("Parent waiting for Child A...\n");
    waitpid(pid_a, &status_a, 0);
    printf("Child A completed\n\n");
    
    printf("Parent waiting for Child B...\n");
    waitpid(pid_b, &status_b, 0);
    printf("Child B completed\n\n");
    
    printf("Both children have finished. Both should have printed 'one two'.\n");
    
    return 0;
}