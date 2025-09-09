#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <pattern>\n", argv[0]);
        fprintf(stderr, "Example: %s \"\\.c$\"\n", argv[0]);
        return 1;
    }
    
    char *pattern = argv[1];
    int pipe_fd[2];
    pid_t pid1, pid2;
    
    printf("Creating pipeline: ls | grep %s\n", pattern);
    
    // Create pipe
    if (pipe(pipe_fd) == -1) {
        perror("pipe failed");
        return 1;
    }
    
    // First child: ls (producer)
    pid1 = fork();
    if (pid1 == 0) {
        // Child 1: redirect stdout to pipe write end
        close(pipe_fd[0]);  // Close read end
        
        // Redirect stdout to pipe write end
        if (dup2(pipe_fd[1], STDOUT_FILENO) == -1) {
            perror("dup2 failed in ls process");
            exit(1);
        }
        close(pipe_fd[1]);  // Close original write end
        
        // Execute ls
        execlp("ls", "ls", NULL);
        perror("execlp ls failed");
        exit(1);
        
    } else if (pid1 < 0) {
        perror("fork failed for ls");
        return 1;
    }
    
    // Second child: grep (consumer)
    pid2 = fork();
    if (pid2 == 0) {
        // Child 2: redirect stdin from pipe read end
        close(pipe_fd[1]);  // Close write end
        
        // Redirect stdin from pipe read end
        if (dup2(pipe_fd[0], STDIN_FILENO) == -1) {
            perror("dup2 failed in grep process");
            exit(1);
        }
        close(pipe_fd[0]);  // Close original read end
        
        // Execute grep with the pattern
        execlp("grep", "grep", pattern, NULL);
        perror("execlp grep failed");
        exit(1);
        
    } else if (pid2 < 0) {
        perror("fork failed for grep");
        return 1;
    }
    
    // Parent: close pipe ends and wait for both children
    close(pipe_fd[0]);
    close(pipe_fd[1]);
    
    // Wait for both children to finish
    int status1, status2;
    waitpid(pid1, &status1, 0);
    waitpid(pid2, &status2, 0);
    
    printf("\nPipeline completed.\n");
    printf("ls exit status: %d\n", WEXITSTATUS(status1));
    printf("grep exit status: %d\n", WEXITSTATUS(status2));
    
    return 0;
}