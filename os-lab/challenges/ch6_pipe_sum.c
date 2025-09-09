#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pipe_fd[2];
    pid_t pid;
    
    // Create pipe
    if (pipe(pipe_fd) == -1) {
        perror("pipe failed");
        return 1;
    }
    
    printf("Parent creating pipe for communication with child\n");
    
    // Fork a child process
    pid = fork();
    
    if (pid == 0) {
        // Child process (consumer)
        close(pipe_fd[1]);  // Close write end
        
        printf("Child: Reading numbers from pipe...\n");
        
        int number;
        int sum = 0;
        int count = 0;
        
        // Read numbers from pipe until EOF
        while (read(pipe_fd[0], &number, sizeof(int)) > 0) {
            printf("Child: Read number %d\n", number);
            sum += number;
            count++;
        }
        
        close(pipe_fd[0]);  // Close read end
        
        printf("Child: Read %d numbers, sum = %d\n", count, sum);
        exit(0);
        
    } else if (pid > 0) {
        // Parent process (producer)
        close(pipe_fd[0]);  // Close read end
        
        printf("Parent: Writing numbers 1-10 to pipe...\n");
        
        // Write numbers 1 through 10 to pipe
        for (int i = 1; i <= 10; i++) {
            printf("Parent: Writing number %d\n", i);
            write(pipe_fd[1], &i, sizeof(int));
        }
        
        close(pipe_fd[1]);  // Close write end (signals EOF to child)
        
        // Wait for child to finish
        int status;
        wait(&status);
        
        printf("Parent: Child has finished processing all numbers\n");
        
    } else {
        perror("fork failed");
        return 1;
    }
    
    return 0;
}