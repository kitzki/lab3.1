#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }
    else if (pid == 0) {
        // Child process
        printf("Hello from child (PID=%d, PPID=%d)\n", getpid(), getppid());
        sleep(2);
        exit(7);
    }
    else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            printf("child %d exited with status %d\n", pid, WEXITSTATUS(status));
        }
    }

    return 0;
}