#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    printf("Main program: Preparing to exec worker program\n");
    
    pid_t pid = fork();
    
    if (pid == 0) {
        // Child process - will exec the worker program
        printf("Child: Setting up environment and arguments for worker\n");
        
        // Prepare arguments for worker program
        char *worker_args[] = {
            "./worker",           // argv[0] - program name
            "arg1",              // argv[1] - first argument
            "arg2",              // argv[2] - second argument  
            "hello world",       // argv[3] - argument with spaces
            NULL                 // argv[4] - null terminator
        };
        
        // Prepare environment - set MYVAR=hello
        char *worker_env[] = {
            "MYVAR=hello",       // Custom environment variable
            "PATH=/usr/bin:/bin", // Keep PATH so system still works
            "HOME=/tmp",         // Custom HOME for demonstration
            NULL                 // null terminator
        };
        
        printf("Child: Executing worker with custom environment (MYVAR=hello)\n");
        
        // Execute worker program with custom arguments and environment
        execve("./worker", worker_args, worker_env);
        
        // If we reach here, execve failed
        perror("execve failed");
        exit(1);
        
    } else if (pid > 0) {
        // Parent process
        printf("Parent: Waiting for worker to complete...\n");
        
        int status;
        wait(&status);
        
        if (WIFEXITED(status)) {
            printf("Parent: Worker completed with exit status %d\n", WEXITSTATUS(status));
        } else {
            printf("Parent: Worker terminated abnormally\n");
        }
        
        printf("Parent: Demonstration completed\n");
        
    } else {
        perror("fork failed");
        return 1;
    }
    
    return 0;
}