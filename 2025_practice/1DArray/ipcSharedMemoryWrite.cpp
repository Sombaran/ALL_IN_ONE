/**
 * @file ipcSharedMemoryWrite.cpp
 * @author Sombaran
 * @brief Example of writing to a shared memory object using POSIX shared memory APIs in C++
 * @version 0.1 
 * @date 2024-06-10
 * @description This program creates a shared memory object, writes a message to it, and then cleans up.
 * Detailed Steps


Initialize constants

name = "/my_shm"
SIZE = 4096
message = "Hello from writer"



Open or create shared memory object

Call shm_open(name, O_CREAT | O_RDWR, 0666)
If it returns -1, log error and exit.



Resize shared memory to desired length

Call ftruncate(shm_fd, SIZE)
If it returns -1, log error and exit.



Map shared memory into the process address space

Call mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0)
If it returns MAP_FAILED, log error and exit.



Write data into the mapped region

Copy message (including null terminator) using memcpy(ptr, message, strlen(message) + 1)



Cleanup local mappings and fd

munmap(ptr, SIZE)
close(shm_fd)
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

#define SHM_NAME        "/my_shm"
#define SHM_SIZE        4096
#define SEM_CAN_WRITE   "/my_shm_can_write"
#define SEM_HAS_DATA    "/my_shm_has_data"


auto simpleShareMemoryWrite() -> void {
    std::string name = "/my_shm"; 
    const size_t SIZE = 4096;
    std::string message = "Hello from writer processes via lambda function !\n";

    int shm_fd = shm_open(name.c_str(), O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(shm_fd, SIZE) == -1) {
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }

    void *ptr = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    memcpy(ptr, message.c_str(), message.size() + 1);

    munmap(ptr, SIZE);
    close(shm_fd);
};

int main (int const argc, char **argv) {
    /**
     * @brief Shared memory object name and size
     * Name of the shared memory object
     * It is a logical name managed by the kernel in a special namespace for shared memory objects
     * On most Linux systems, these objects are implemented as files under /dev/shm (a temporary filesystem in RAM). 
     * For example, "/my_shm" would correspond to /dev/shm/my_shm.
     * This location is not persistent; 
     * it exists only while the shared memory object is alive. When you call shm_unlink(name), the kernel removes it.
     * */ 
    simpleShareMemoryWrite();
    #define SIZE            1024
    #define SEM_NAME "/mysem"
    // Create shared memory object
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, SIZE);

    // Map shared memory
    void* ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

    // Create/open semaphore
    sem_t* sem = sem_open(SEM_NAME, O_CREAT, 0666, 0);

    // Write data
    std::string input;
    std::cout << "Enter data to write: ";
    std::getline(std::cin, input);
    strcpy((char*)ptr, input.c_str());

    std::cout << "Data written: " << (char*)ptr << std::endl;

    // Signal reader
    sem_post(sem);

    // Cleanup
    munmap(ptr, SIZE);
    close(shm_fd);

    
    return 0;
}