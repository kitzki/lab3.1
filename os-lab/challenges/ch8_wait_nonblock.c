#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int main() {
    pid_t pids[3];
    int sleep_times[3] = {1, 2, 3};
    int children_finished = 0;
    int total_children = 3;
    
    printf("Parent spawning 3 children with sleep times: 1s, 2s, 3s\n");
    
    // Spawn 3 children
    for (int i = 0; i < 3; i++) {
        pids[i] = fork();
        
        if (pids[i] == 0) {
            // Child process
            printf("Child %d (PID %d): Starting, will sleep for %d second(s)\n", 
                   i + 1, getpid(), sleep_times[i]);
            sleep(sleep_times[i]);
            printf("Child %d (PID %d): Finished sleeping, exiting\n", 
                   i + 1, getpid());
            exit(i + 1);  // Exit with unique code
            
        } else if (pids[i] < 0) {
            perror("fork failed");
            return 1;
        } else {
            printf("Parent: Created child %d with PID %d\n", i + 1, pids[i]);
        }
    }
    
    printf("\nParent: All children created, now polling with WNOHANG...\n");
    
    // Poll for finished children using WNOHANG
    while (children_finished < total_children) {
        pid_t finished_pid;
        int status;
        
        // Non-blocking wait - check if any child has finished
        finished_pid = waitpid(-1, &status, WNOHANG);
        
        if (finished_pid > 0) {
            // A child has finished
            children_finished++;
            if (WIFEXITED(status)) {
                int exit_code = WEXITSTATUS(status);
                printf("Parent: Child PID %d finished (exit code %d) [%d/%d completed]\n", 
                       finished_pid, exit_code, children_finished, total_children);
            } else {
                printf("Parent: Child PID %d terminated abnormally [%d/%d completed]\n", 
                       finished_pid, children_finished, total_children);
            }
        } else if (finished_pid == 0) {
            // No children have finished yet
            printf("Parent: No children finished yet, continuing other work...\n");
            usleep(100000);  // Sleep for 100ms to simulate other work
        } else {
            // Error occurred
            if (children_finished < total_children) {
                perror("waitpid failed");
                break;
            }
        }
    }
    
    printf("\nParent: All children have finished. Parent is not blocked during waiting!\n");
    return 0;
}