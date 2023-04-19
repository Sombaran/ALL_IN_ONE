// Semaphores

//     Semaphore proposed by Edsger Dijkstra, is 0 Technique to manage concurrent
//     processes by using a simple integer value, which is known as a semaphore.
//     Semaphore is simply a variable which is non-negative and shared between
//     Threads. This variable is used To solve the critical section problem and To achieve
//     process synchronization in The multiprocessing environment.

//     A semaphore S is an integer variable That, apart from iniTializaTion, is
//     accessed only Through Two standard atomic operations: wait () and signal ().

//     All the modifications To The integer value of The semaphore in The waiT 0 and signal() operations must be executed indivisibly.
//     That is, when one process modifies The semaphore value, no other process can simultaneously modify That same semaphore value.

//     Some point regarding P and V operation :
//         1. P operation is also called wait, sleep, or down operation.
//         2. V operation is also called signal, wake-up, or up operation.
//         3. Both operations are atomic and semaphore(s) is always initialized to one.
//              Here atomic means that variable on which read, modify and update happens at the same time/moment with no pre-emption i.e.
//              in-between read, modify and update no other operation is performed that may change the variable.
//         4. A critical section is surrounded by both operations to implement process synchronization.
//              See the below image. The critical section of Process P is in between P and V operation.

//     waiT ( ) —> P [from The Dutch word proberen, which means "To test"]

//         Definition of wait ( ):

//             P(semaphores S){
//                 while(S<=0) /* wait until s=O */
//                 ;
//                 S = S-1;
//             }

//     signal () —> V [from The DuTch word verhogen, which means " to incremenT"]

//         Definition of signal ( ):

//             V(semaphores S){
//                 S = S+1;
//             }

//     Types of Semaphores:
//         1. Binary Semaphore:
//             The value of a binary semaphore can range only between 0 and 1. On some systems,
//             binary semaphores are known as mutex locks, as They are locks That provide mutual
//             exclusion.
//         2. Counting Semaphore:
//             Its value can range over an unrestricted domain. It is used to control access to a
//             resource that has multiple instance.

//         1. sem_init() :      Initialize semaphore
//         2. sem_destroy() :   releases all resources
//         3. sem_wait() :      Wait for the semaphore to acquire
//         4. sem_post() :      Release semaphore
//         5. sem_trywait() :   Only works when the caller does not have to wait
//         6. sem_getvalue() :  Reads the counter value of the semaphore
//         7. sem_open() :      Connects to, & optionally creates, a named semaphore( like sem_init() )
//         8. sem_unlink() :    Ends connection to an open semaphore & causes the semaphore to be removed when 
//                              the last process closes it( like sem_destroy())

// Steps to use seaphores:

//         1. Include semaphore.h
//         2. Compile the code by linking with -lpthread -lrt

//         To lock a semaphore or wait we can use the sem_wait function:
//             int sem_wait(sem_t *sem);

//         To release or signal a semaphore, we use the sem_post function:
//             int sem_post(sem_t *sem);

//         A semaphore is initialised by using sem_init(for processes or threads) or sem_open (for IPC).
//             sem_init(sem_t *sem, int pshared, unsigned int value);

//             Where,
//                 sem : Specifies the semaphore to be initialized.

//             pshared :
//                 This argument specifies whether or not the newly initialized semaphore is shared between processes or between threads.
//                 A non-zero value means the semaphore is shared between processes and a value of zero means it is shared between threads.
//             value :
//                 Specifies the value to assign to the newly initialized semaphore.

//         To destroy a semaphore, we can use sem_destroy.
//             sem_destroy(sem_t *mutex);

//         To declare a semaphore, the data type is sem_t.

//         Problem in this implementation of semaphore :

//              The main problem with semaphores is that they require busy waiting, If a process is in the critical section,
//         then other processes trying to enter critical section will be waiting until the critical section is not occupied
//         by any process.

//         Whenever any process waits then it continuously checks for semaphore value (look at this line while
//         (s==O); in P operation) and waste CPU cycle.

//         There is also a chance of "spinlock" as the processes keep on spins while waiting for the lock.

//         To avoid this another implementation is provided below.

//         struct Semaphore
//         {
//             int value;

//             // q contains all Process Control Blocks(PCBs)
//             // corresponding to processes got blocked
//             // while performing down operation.
//             Queue<process> q;

//         } P(Semaphore s)
//         {
//             s.value = s.value - 1;
//             if (s.value < 0)
//             {

//                 // add process to queue
//                 // here p is a process which is currently executing
//                 q.push(p);
//                 block();
//             }
//             else
//                 return;
//         }

//         V(Semaphore s)
//         {
//             s.value = s.value + 1;
//             if (s.value >= 0)
//             {

//                 // remove process p from queue
//                 Process p = q.pop();
//                 wakeup(p);
//             }
//             else
//                 return;
//         }
//
//             In this implementation whenever the process waits it is added to a waiting queue of processes associated with that semaphore.
//             This is done through system call block() on that process.
//             When a process is completed it calls the signal function and one process in the queue is resumed.
//             It uses wakeup() system call.