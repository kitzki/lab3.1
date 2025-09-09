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
        // Child runs ls -la
        execlp("ls", "ls", "-la", NULL);
        perror("exec failed");  // only runs if exec fails
        exit(1);
    }
    else {
        // Parent waits
        wait(NULL);
        printf("Parent: child finished executing ls -la\n");
    }

    return 0;
}
