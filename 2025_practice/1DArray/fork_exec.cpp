/**
 * @details: Why does the if/else seem to run twice?
 * When you call fork(), it creates a new process by duplicating the calling process.
 * This means that both the parent and child processes will continue executing from the point where fork() was called.
 * As a result, both processes will execute the code following the fork() call.
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <errno.h>
#include <cstring>
#include <semaphore.h>
#include <sys/wait.h>

void simplePlayWithFork() {
    pid_t newPid = fork();
    if (newPid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    std::cout << "After fork(), PID = " << getpid() << ", newPid = " << newPid << std::endl;    
    if (newPid == 0) {
        // Child process
        printf("Child process: PID = %d, Parent PID = %d\n", getpid(), getppid());
    } else {
        // Parent process
        printf("Parent process: PID = %d, Child PID = %d\n", getpid(), newPid);
    }
}

void complexPlayWithFork() {
    int runTime = 2;
    do {
        pid_t newPid = fork();
        if (newPid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        std::cout << "After fork(), PID = " << getpid() << ", newPid = " << newPid << " " << runTime << std::endl;    
        if (newPid == 0) {
            // Child process
            //execl("./ipcSharedMemoryWrite", "ipcSharedMemoryWrite", (char *)NULL);
            //perror("execl");
            //exit(EXIT_FAILURE);        
            printf("Child process: PID = %d, Parent PID = %d, runTime = %d\n", getpid(), getppid(), runTime);

        } else {
            // Parent process
            //execl("./ipcSharedMemoryRead", "ipcSharedMemoryRead", (char *)NULL);
            //perror("execl");
            //exit(EXIT_FAILURE);        
            printf("Parent process: PID = %d, Child PID = %d, runTime = %d\n", getpid(), newPid, runTime);
        }
    } while (runTime-- > 0);
}


void execExample() {
    std::string command {"ls"};
    pid_t pid = fork();
    if (pid == 0) {
        // Child
        std::cout << "In child process, PID = " << getpid()  << " " << pid << std::endl;
        // Replace the child process with a new program
        
        char* const args[] = {
            (char*)"ls",       // argv[0] = program name
            (char*)"-lrt",
            nullptr            // argv must be null-terminated
        };

        execvp(command.c_str(), args);
        perror("execvp");  // Only runs if exec fails
        _exit(127);
    } else {
        // Parent
        std::cout << "In parent process, PID = " << getpid() << ", waiting for child PID = " << pid << std::endl;
        int status{};
        waitpid(pid, &status, 0);
        printf("Child finished\n");
    }
}

int main() {

    /**
     * Before fork():
    Parent
    [ user stack | heap | data | code ]
    [ kernel stack ]

    After fork():
    Parent
    [ user stack (COW) | ... ]
    [ kernel stack ]
    Child
    [ user stack (same contents, COW → diverges on write) | ... ]
    [ kernel stack ]
     */
    
    //simplePlayWithFork();

    //complexPlayWithFork();

    /**
     * 
     * Parent calls fork() → creates child.
     * Child calls exec() → runs a new program.
     * Parent calls waitpid() → waits for child to finish.
     */
    execExample();
    return (0);
}