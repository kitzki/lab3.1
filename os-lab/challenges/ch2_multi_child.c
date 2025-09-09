#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_children>\n", argv[0]);
        return 1;
    }
    
    int n = atoi(argv[1]);
    if (n <= 0) {
        fprintf(stderr, "Number of children must be positive\n");
        return 1;
    }
    
    printf("Parent creating %d children\n", n);
    
    // Fork n children
    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        
        if (pid == 0) {
            // Child process
            printf("Child %d starting\n", i);
            exit(i + 1);  // Exit with code equal to index + 1
        } else if (pid < 0) {
            perror("fork failed");
            return 1;
        }
        // Parent continues to create more children
    }
    
    // Parent waits for all children and reports their exit status
    printf("Parent waiting for all children to finish...\n");
    
    for (int i = 0; i < n; i++) {
        int status;
        pid_t finished_pid = wait(&status);
        
        if (finished_pid > 0) {
            if (WIFEXITED(status)) {
                int exit_code = WEXITSTATUS(status);
                int child_index = exit_code - 1;  // Convert back to index
                printf("Child %d (PID %d) finished with exit code %d\n", 
                       child_index, finished_pid, exit_code);
            } else {
                printf("Child PID %d terminated abnormally\n", finished_pid);
            }
        }
    }
    
    printf("All children have finished\n");
    return 0;
}