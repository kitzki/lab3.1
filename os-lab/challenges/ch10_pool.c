#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

void process_task(const char* task, int worker_id) {
    printf("Worker %d (PID %d): Starting task '%s'\n", worker_id, getpid(), task);
    
    // Simulate processing time based on task name length
    int work_time = (strlen(task) % 3) + 1;  // 1-3 seconds
    sleep(work_time);
    
    printf("Worker %d (PID %d): Completed task '%s' after %d seconds\n", 
           worker_id, getpid(), task, work_time);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <concurrency_limit> <task1> <task2> ... <taskN>\n", argv[0]);
        fprintf(stderr, "Example: %s 3 file1 file2 file3 file4 file5\n", argv[0]);
        return 1;
    }
    
    int max_workers = atoi(argv[1]);
    int total_tasks = argc - 2;
    char **tasks = &argv[2];
    
    if (max_workers <= 0) {
        fprintf(stderr, "Concurrency limit must be positive\n");
        return 1;
    }
    
    printf("Worker Pool Manager: Processing %d tasks with max %d workers\n", 
           total_tasks, max_workers);
    
    pid_t active_workers[max_workers];
    int active_count = 0;
    int next_task = 0;
    int completed_tasks = 0;
    int worker_counter = 1;
    
    // Initialize active workers array
    for (int i = 0; i < max_workers; i++) {
        active_workers[i] = -1;
    }
    
    // Main worker pool loop
    while (completed_tasks < total_tasks) {
        // Start new workers if we have capacity and tasks remaining
        while (active_count < max_workers && next_task < total_tasks) {
            // Find empty slot
            int slot = -1;
            for (int i = 0; i < max_workers; i++) {
                if (active_workers[i] == -1) {
                    slot = i;
                    break;
                }
            }
            
            // Fork new worker
            pid_t pid = fork();
            if (pid == 0) {
                // Child process - worker
                process_task(tasks[next_task], worker_counter);
                exit(0);  // Worker exits after completing task
                
            } else if (pid > 0) {
                // Parent - add to active workers
                active_workers[slot] = pid;
                printf("Manager: Started worker %d (PID %d) for task '%s' [%d/%d active]\n", 
                       worker_counter, pid, tasks[next_task], active_count + 1, max_workers);
                active_count++;
                next_task++;
                worker_counter++;
                
            } else {
                perror("fork failed");
                return 1;
            }
        }
        
        // Wait for at least one worker to finish
        if (active_count > 0) {
            int status;
            pid_t finished_pid = wait(&status);
            
            if (finished_pid > 0) {
                // Find and remove finished worker from active list
                for (int i = 0; i < max_workers; i++) {
                    if (active_workers[i] == finished_pid) {
                        active_workers[i] = -1;
                        active_count--;
                        completed_tasks++;
                        
                        printf("Manager: Worker PID %d finished [%d/%d tasks completed, %d/%d active]\n", 
                               finished_pid, completed_tasks, total_tasks, active_count, max_workers);
                        break;
                    }
                }
            }
        }
    }
    
    printf("\nManager: All %d tasks completed successfully!\n", total_tasks);
    printf("Manager: Worker pool demonstration finished.\n");
    return 0;
}