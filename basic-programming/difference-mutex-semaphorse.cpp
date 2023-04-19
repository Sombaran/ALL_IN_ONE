Difference between Mutex and Semaphore
In any operating system, process synchronization is extremely important for sharing data in a way that it remains consistent. Here system resources are shared and concurrent access is provided for this data. By proper process, synchronization data inconsistency can be avoided. Along with this, the critical section problem can be handled.

The critical section problem can be solved using software methods. The most popular of these methods is Mutex and Semaphore. Mutex is basically a locking method for a resource. A semaphore is used as a signalling mechanism.

In this article, we will take a look at their basic differences to get a better understanding of them.

What is Mutex?
Mutex actually stands for Mutual Exclusion Object. The resource access is operated by a mutex.

Mutual exclusion is a condition where only a process can access the resource at a given time. The process cannot enter the critical section when another process is present. In an operating system, a Mutex is used for protecting the critical section.

A mutex will allow many threads to access a single resource. But these resources can do it only at a particular point in time. They are not allowed access simultaneously.

This happens in a simple process. When a program initialises, a mutex object is created for a resource. This mutex object is then created with a unique ID or name.

Suppose the program thread has to utilise the resource, the lock on the mutex object is occupied. After using the resource, the thread releases the lock on the mutex.

The other threads that have to use the resource follow the same process. The other process may want to obtain the lock on the mutex object. In this case, the process is put on hold in a queue. The thread will only get access to the resource after the lock is released.

Mutex helps to avoid race conditions. This is a condition where multiple resources try to access the same point of time.

However, you can lock a mutex more than once. This is possible in case of a recursive mutex. Here, the mutex has a count. This determines the number of times it was locked or unlocked. A single state is retained by a mutex. The coder has to unlock it as many times it was locked.

What is Semaphore?
A semaphore is a variable that is very important for process synchronization. The S is used for denoting the variable. The variable is modified by two functions – signal and wait. Suppose a process is modifying the value of a semaphore. During this time, other processes cannot alter it.

The two methods of modifying the semaphore are as follows:

Wait () – This method controls the entry of a thread inside the critical section. The method only decreases the value of the argument S. This is only done when the value of Wait is positive. There can be a situation where the value of S is negative. In that case, no operation is performed. The syntax is:

wait (S)

{
while (S <= 0);

S--;
}
Signal () – This method controls the exit of a thread from the critical section. The value of S is incremented by 1. This is done when there are no processes blocked in the queue. The syntax is:

signal (S)

{
S++;
}
Types of Semaphores:

The two types of semaphores are as follows:

Binary semaphore – In this semaphore, the value ranges between 0 and 1. It is like a mutex lock. Here, when a process requires a resource, it executes the wait operation. Then the value of the semaphore changes from 1 to 0.

The signal operation is executed when if the process releases a resource. After releasing the resource, the value is incremented by 1. For implementing mutual exclusion, this type of semaphore can be used.

Counting semaphore – This type of semaphore can handle more than one resource. According to the number of resources in the system, this value is initialised. Here the wait () operation is executed when a resource is necessary. The semaphore value is decreased by 1. After releasing the resource, the signal is executed. The semaphore is incremented by 1.

If the processes occupy the resources, the semaphore value becomes 0.

Semaphore vs Mutex
Basic difference
A semaphore is a signalling mechanism used during process synchronization. But, the mutex is a locking mechanism used for handling processes.

Modifications
The wait and signal operations can modify a semaphore. The processes obtaining or releasing a lock can have ownership on the semaphore value. Any such process can change this value.

Whereas, in case of a mutex, it is modified only by the process that may request or release a resource. So, it can be locked or unlocked. Only the process that has acquired a lock can release the lock on the mutex.

Type
A semaphore is an object. On the other hand, a mutex is actually a variable.

Resource handing
If no resource is free then the process needing a resource executes wait operation. It has to wait until the count of the semaphore is larger than 0.

But in case of a mutex, if it is locked, the process has to wait. The process is kept in a queue. It will get access to the desired resource when the mutex is unlocked.

Conclusion

Both mutex and semaphores are important utilities needed for managing the different processes within an operating system. A semaphore is used for signalling the availability of resources. Mutex is used for locking a resource. It prevents problems like a race condition.

Mutex is primarily used for achieving mutual exclusion. However, a semaphore is used for notifying events and synchronization. Furthermore, a mutex is a variable that has no further categories. Semaphores, on the other hand, have two types binary and counting semaphore.