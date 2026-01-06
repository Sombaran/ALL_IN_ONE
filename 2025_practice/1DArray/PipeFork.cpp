

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



int main(int argc, char** argv){

    /**
     * 
     * fd1[0]; //-> for using read end
     * fd2[1]; //-> for using write end
     */
    int fd1[2], fd2[2];
    if ((pipe(fd1) == -1) or (pipe(fd2) == -1)) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t mPid = fork();
    if (mPid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (mPid == 0) {
        // Child process
        close(fd1[1]); // Close unused write end

        char buffer[100];
        read(fd1[0], buffer, sizeof(buffer));
        std::cout << "Child received: " << buffer << std::endl;

        const char *response = "Hello from child";
        write(fd2[1], response, strlen(response) + 1);

        close(fd1[0]);
        close(fd2[1]);
    } else {
        // Parent process
        close(fd1[0]); // Close unused read end
        const char *message = "Hello from parent";
        write(fd1[1], message, strlen(message) + 1);
        int status{};
        waitpid(mPid, &status, 0);
        std::cout << "Child finished\n";
        char buffer[100];
        read(fd2[0], buffer, sizeof(buffer));
        std::cout << "Parent received: " << buffer << std::endl;

        close(fd1[1]);
        close(fd2[0]);
    }
}