#include <stdio.h>
#include <unistd.h>


int main()
{ 
    FILE *file;
    int pid = fork();

    if (pid > 0)
    {
        printf("Parent Process: PID of child = %d\n", pid);
    }
    else if (pid == 0)
    {
        printf("Child process: My PID = %d\n", pid);
    }
    else
    {
        perror("fork failed");
    }
    
    
    while (1)
    {
        file = fopen("test.txt", "a");
        if (file == NULL) {
            perror("Error opening file");
            return 1;
        }

        fprintf(file, "Process: %s, PID: %d\n", (pid == 0 ? "Child" : "Parent"), pid);
        fflush(file);
        fclose(file);
        sleep(1);
    }
    
    return 0;
}