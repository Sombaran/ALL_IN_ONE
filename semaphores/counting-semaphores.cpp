// C program to demonstrate working of counting Semaphores

#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <thread>
using namespace std;

sem_t mutex;
int value = 0;
void function(const char *name)
{
    // wait
    sem_wait(&mutex);
    printf("\nEntered..\n");

    // critical section
    cout << "name: " << name << " value: " << (++value) << endl;
    sleep(1);
    // signal
    printf("\nJust Exiting...\n");

    sem_post(&mutex);
}

int main()
{
    sem_init(&mutex, 0, 3);
    std::thread t1(function, "thread1");
    std::thread t2(function, "thread2");
    std::thread t3(function, "thread3");
    std::thread t4(function, "thread4");
    std::thread t5(function, "thread5");

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    sem_destroy(&mutex);
    return 0;
}
