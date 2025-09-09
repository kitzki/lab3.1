#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(void) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        // Child exits immediately
        printf("Child exiting: PID=%d\n", getpid());
        fflush(stdout);
        exit(0);
    } else {
        // Parent sleeps forever, never calls wait()
        printf("Parent PID=%d, child PID=%d\n", getpid(), pid);
        fflush(stdout);
        printf("Check 'ps -el | grep Z' in this terminal or another. Parent sleeping forever...\n");
        fflush(stdout);
        while(1) sleep(1);  // infinite sleep
    }

    return 0;
}