#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    int fd[2];
    if (pipe(fd) == -1) {
        perror("pipe");
        exit(1);
    }

    pid_t pid1 = fork();
    if (pid1 < 0) {
        perror("fork");
        exit(1);
    }

    if (pid1 == 0) {
        // First child: run ls, write into pipe
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);

        printf("[child1] PID=%d about to exec ls\n", getpid());
        execlp("ls", "ls", (char *)NULL);
        perror("execlp ls");
        _exit(1);
    }

    // Parent continues, fork second child
    pid_t pid2 = fork();
    if (pid2 < 0) {
        perror("fork");
        exit(1);
    }

    if (pid2 == 0) {
        // Second child: run grep .c, read from pipe
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);

        printf("[child2] PID=%d about to exec grep .c\n", getpid());
        execlp("grep", "grep", ".c", (char *)NULL);
        perror("execlp grep");
        _exit(1);
    }

    // Parent: close both ends, wait for children
    close(fd[0]);
    close(fd[1]);

    printf("[parent] forked child1 with PID=%d and child2 with PID=%d\n", pid1, pid2);

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    printf("[parent] finished waiting for both children\n");

    return 0;
}