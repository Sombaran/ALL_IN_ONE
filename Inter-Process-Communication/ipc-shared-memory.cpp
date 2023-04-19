// IPC through shared memory

/*
    Inter Process Communication through shared memory is a concept where two or more process can access the common memory.
    Communication is done via this shared memory where changes made by one process can be viewed by another process.

    The problem with pipes, FIFO and message queue – is that for two process to exchange information. The information has to go through the kernel.

        1. Server reads from the input file.
        2. The server writes this data in a message using either a pipe, fifo or message queue.
        3. The client reads the data from the IPC channel, Again requiring the data to be copied from kernel’s space IPC buffer to the client’s space buffer.
        4. Finally the data is copied from the client’s buffer.

    A total of four copies of data are required (2 read and 2 write).

    So, shared memory provides a way by letting two or more processes share a memory segment. With Shared Memory the data is only copied twice – from input file into shared memory and from shared memory to the output file.

    SYSTEM CALLS USED ARE:

        ftok():
            It is use to generate a unique key.
        shmget():
            int shmget(key_t, size_tsize, intshmflg);
            It returns an identifier for the shared memory segment.
        void *shmat(int shmid ,void *shmaddr ,int shmflg);
            Before you can use a shared memory segment, 
            You have to attach yourself to it using shmat().
            shmid is shared memory id.
            shmaddr specifies specific address to use.
            But we should set it to zero and OS will automatically choose the address.
        int shmdt(void *shmaddr);
            When you’re done with the shared memory segment.
            Your program should detach itself from it using shmdt().
        shmctl(int shmid,IPC_RMID,NULL);
            when you detach from shared memory, it is not destroyed.
            So, to destroy shmctl() is used.
*/

// Process - 1 Write into shared memory.

#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
using namespace std;

int write_shared_memory()
{
    // ftok to generate unique key
    key_t key = ftok("shmfile", 65);

    // shmget returns an identifier in shmid
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);

    // shmat to attach to shared memory
    char *str = (char *)shmat(shmid, (void *)0, 0);

    cout << "Write Data : ";
    gets(str);

    printf("Data written in memory: %s\n", str);

    // detach from shared memory
    shmdt(str);

    return 0;
}

// Process - 2 Read from shared memory.

#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
using namespace std;

int read_shared_memory()
{
    // ftok to generate unique key
    key_t key = ftok("shmfile", 65);

    // shmget returns an identifier in shmid
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);

    // shmat to attach to shared memory
    char *str = (char *)shmat(shmid, (void *)0, 0);

    printf("Data read from memory: %s\n", str);

    // detach from shared memory
    shmdt(str);

    // destroy the shared memory
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}

int main()
{
    write_shared_memory();
    read_shared_memory();
    return 0;
}