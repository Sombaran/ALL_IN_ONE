#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <iostream>
#include <semaphore.h>

#define SHM_NAME        "/my_shm"
#define SHM_SIZE        4096
#define SEM_CAN_WRITE   "/my_shm_can_write"
#define SEM_HAS_DATA    "/my_shm_has_data"


auto simpleShareMemoryRead() -> void {
    std::string name = "/my_shm"; 
    const size_t SIZE = 4096;

    int shm_fd = shm_open(name.c_str(), O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    void *ptr = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    printf("Reader got: %s", (char *)ptr);

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
    //simpleShareMemoryRead();

    #define SEM_NAME "/mysem"
    #define SIZE 1024

    // Open shared memory object
    int shm_fd = shm_open(SHM_NAME, O_RDONLY, 0666);

    // Map shared memory
    void* ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);

    // Open semaphore
    sem_t* sem = sem_open(SEM_NAME, 0);

    // Wait for writer
    sem_wait(sem);

    // Read data
    std::cout << "Data read: " << (char*)ptr << std::endl;

    // Cleanup
    munmap(ptr, SIZE);
    close(shm_fd);

    // Remove shared memory and semaphore
    shm_unlink(SHM_NAME);
    sem_unlink(SEM_NAME);

    return 0;


    return 0;
}