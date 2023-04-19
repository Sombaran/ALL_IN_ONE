/*
Definition	
A process is a program under execution i.e an active program.	
A thread is a lightweight process that can be managed independently by a scheduler.

Context switching time	
Processes require more time for context switching as they are more heavy.	
Threads require less time for context switching as they are lighter than processes.

Memory Sharing	
Processes are totally independent and don’t share memory.	
A thread may share some memory with its peer threads.

Communication	
Communication between processes requires more time than between threads.	
Communication between threads requires less time than between processes.

Blocked	
If a process gets blocked, remaining processes can continue execution.	
If a user level thread gets blocked, all of its peer threads also get blocked.

Resource Consumption
Processes require more resources than threads.	
Threads generally need less resources than processes.

Dependency	
Individual processes are independent of each other.	
Threads are parts of a process and so are dependent.

Data and Code sharing	
Processes have independent data and code segments.	
A thread shares the data segment, code segment, files etc. with its peer threads.

Treatment by OS	
All the different processes are treated separately by the operating system.	
All user level peer threads are treated as a single task by the operating system.

Time for creation	
Processes require more time for creation.	
Threads require less time for creation.

Time for termination	
Processes require more time for termination.	
Threads require less time for termination.



What is Process?
A process is an instance of a program that is being executed. 
When we run a program, it does not execute directly. 
It takes some time to follow all the steps required to execute the program, and following these execution steps is known as a process.

A process can create other processes to perform multiple tasks at a time; the created processes are known as clone or child process, and the main process is known as the parent process. Each process contains its own memory space and does not share it with the other processes. It is known as the active entity. A typical process remains in the below form in memory.

A process in OS can remain in any of the following states:

NEW: A new process is being created.
READY: A process is ready and waiting to be allocated to a processor.
RUNNING: The program is being executed.
WAITING: Waiting for some event to happen or occur.
TERMINATED: Execution finished.

How do Processes work?
When we start executing the program, the processor begins to process it. 
It takes the following steps:

1. Firstly, the program is loaded into the computer's memory in binary code after translation.
2. A program requires memory and other OS resources to run it. The resources such that registers, program counter, and a stack, and these resources are provided by the OS.
3. A register can have an instruction, a storage address, or other data that is required by the process.
4. The program counter maintains the track of the program sequence.
5. The stack has information on the active subroutines of a computer program.
6. A program may have different instances of it, and each instance of the running program is knowns as the individual process.

Features of Process
Each time we create a process, we need to make a separate system call for each process to the OS. 
The fork() function creates the process.
Each process exists within its own address or memory space.
Each process is independent and treated as an isolated process by the OS.
Processes need IPC (Inter-process Communication) in order to communicate with each other.
A proper synchronization between processes is not required.


What is Thread?
A thread is the subset of a process and is also known as the lightweight process. 
A process can have more than one thread, and these threads are managed independently by the scheduler. 
All the threads within one process are interrelated to each other. 
Threads have some common information, such as data segment, code segment, files, etc., that is shared to their peer threads. 
But contains its own registers, stack, and counter.

How does thread work?
As we have discussed that a thread is a subprocess or an execution unit within a process. A process can contain a single thread to multiple threads. 

A thread works as follows:
When a process starts, OS assigns the memory and resources to it. 
Each thread within a process shares the memory and resources of that process only.
Threads are mainly used to improve the processing of an application. 
In reality, only a single thread is executed at a time, but due to fast context switching between threads gives an illusion that threads are running parallelly.
If a single thread executes in a process, it is known as a single-threaded And if multiple threads execute simultaneously, then it is known as multithreading.

Types of Threads
There are two types of threads, which are:
1. User Level Thread
As the name suggests, the user-level threads are only managed by users, and the kernel does not have its information.
These are faster, easy to create and manage.
The kernel takes all these threads as a single process and handles them as one process only. The user-level threads are implemented by user-level libraries, not by the system calls.

2. Kernel-Level Thread
The kernel-level threads are handled by the Operating system and managed by its kernel. These threads are slower than user-level threads because context information is managed by the kernel. To create and implement a kernel-level thread, we need to make a system call.

Features of Thread
Threads share data, memory, resources, files, etc., with their peer threads within a process.
One system call is capable of creating more than one thread.
Each thread has its own stack and register.
Threads can directly communicate with each other as they share the same address space.
Threads need to be synchronized in order to avoid unexpected scenarios.

Key Differences Between Process and Thread
A process is independent and does not contained within another process, whereas all threads are logically contained within a process.
Processes are heavily weighted, whereas threads are light-weighted.
A process can exist individually as it contains its own memory and other resources, whereas a thread cannot have its individual existence.
A proper synchronization between processes is not required. In contrast, threads need to be synchronized in order to avoid unexpected scenarios.
Processes can communicate with each other using inter-process communication only; in contrast, threads can directly communicate with each other as they share the same address space.


*/