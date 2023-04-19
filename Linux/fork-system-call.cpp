// fork() system call
/*
    Fork system call is used for creating a new process, which is called child process.
    Which runs concurrently with the process that makes the fork() call (parent process).
    OS allocate different data and states for these two processes.
    After a new child process is created, both processes will execute the next instruction after the fork() system call.
    
    A child process uses the same
        1. PC - program counter
        2. CPU registers
        3. open files
        which use in the parent process.

    Note :
    Child and parent processes are located in separate physical address spaces.
    As a result, the modification in the parent process does not appear in the child process.

    It takes no parameters and returns an integer value.
    Below are different values returned by fork().

        Negative Value:
            creation of a child process was unsuccessful.
        Zero:
            Returned to the newly created child process.
        Positive value:
            Returned to parent or caller. The value contains process ID of newly created child process.
*/

#include <iostream>
using namespace std;
#include <sys/types.h>
#include <unistd.h>

// l. Predict the Output of the following program:.
// int main()
//     {

//         // make two process which run same
//         // program after this instruction
//         fork();

//         printf("Hello world!\n");
//         return 0;
//     }

// Output:

// Hello world!
// Hello world!

// 2. Calculate number of times hello is printed:

// int main()
// {
//     fork();
//     fork();
//     fork();
//     cout << "print()" << endl;
//     return 0;
// }
// Output :
//     hello
//     hello
//     hello
//     hello
//     hello
//     hello
//     hello
//     hello

//  The number of times ‘hello’ is printed is equal to number of process created.
//  Total Number of Processes = 2n,
//  Where n is number of fork system calls.
//  So here n = 3, 2^3 = 8

// 3. Predict the Output of the following program :

// void forkexample()
// {
//     int ret = fork();
//     cout << "ret: " << ret << endl;
//     // child process because return value zero
//     if (ret == 0)
//         printf("Hello from Child!\n");
//     // parent process because return value non-zero (greater then 0).
//     else
//         printf("Hello from Parent!\n");
// }
// int main()
// {
//     forkexample();
//     return 0;
// }

// Note :
//    Two outputs are possible because parent & child process are running concurrently.
//    Depends on the OS will first give control to the parent or to child process.

// Important :
//      Parent process and child process are running the same program.
//      But it does not mean they are identical.
//      OS allocate different data and states for these two processes.
//      The control flow of these processes can be different. See next example:

void forkexample()
{
    int x = 1;

    if (fork() == 0)
        printf("Child has x = %d\n", ++x);
    else
        printf("Parent has x = %d\n", --x);
}
int main()
{
    forkexample();
    return 0;
}
// Output :
//     Parent has x = 0
//     Child has x = 2
//         (or)
//     Child has x = 2
//     Parent has x = 0

// Note:
//     Global variable change in one process does not affected two other processes.
//     because data/state of two processes are different.
//     Parent and child run simultaneously so two outputs are possible.