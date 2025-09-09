#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    printf("=== Worker Program Output ===\n");
    
    // Print all command line arguments
    printf("Worker received %d arguments:\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("  argv[%d] = \"%s\"\n", i, argv[i]);
    }
    
    char *myvar = getenv("MYVAR");
    if (myvar != NULL) {
        printf("Environment variable MYVAR = \"%s\"\n", myvar);
    } else {
        printf("Environment variable MYVAR is not set\n");
    }
    
    printf("Worker program completed successfully.\n");
    return 0;
}