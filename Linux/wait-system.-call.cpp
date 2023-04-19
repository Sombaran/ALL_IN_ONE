// Wait System Call in C

//     A call to wait() system call it blocks the calling process until
//     one of its child processes exits or a signal is received.
//     After child process terminates, parent continues its execution after
//      wait() system call instruction.
//     Child process may terminate due to any of these:
//         1. It calls exit();
//         2. It returns (an int) from main
//         3. It receives a signal (from the OS or another process)
//             whose default action is to terminate.

//         pid_t wait(int *stat_loc);
//     take one argument status and returns a process ID of dead children.

// Note :
//     1. If any process has more than one child processes, then after calling wait()\
//      by parent process, parent process has to be in wait state if no child terminates.

//     2. If only one child process is terminated, then return a wait() returns
//      process ID of the terminated child process.

//     3. If more than one child processes are terminated than wait() reap
//      any arbitrarily child and return a process ID of that child process.

//     4. When wait() returns they also define exit status (which tells our, parent
//      process why child process terminated) via pointer, If status are not NULL.

//     5. If any process has no child process then wait() returns immediately "-1".

// Example :

// C program to demonstrate working of wait()
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    if (fork() == 0)
        printf("HC: hello from child\n");
    else
    {
        printf("HP: hello from parent\n");
        wait(NULL);
        printf("CT: child has terminated\n");
    }

    printf("Bye\n");
    return 0;
}

// Output :

//     HC: hello from child
//     Bye
//     HP: hello from parent
//     CT: child has terminated

//         OR

//     HP: hello from parent
//     HC: hello from child
//     HC: Bye
//     CT: child has terminated

//    Note: bye will not print before 'HC: hello from child'

//     Child status information:
//     Status information about the child reported by wait is more than just the
//     exit status of the child, it also includes

//         1. normal/abnormal termination
//         2. termination cause
//         3. exit status

//     We know if more than one child processes are terminated, then wait() reaps
//     any arbitrarily child process but if we want to reap any specific child process.
//     Then, we use waitpid() function.

//         Syntax in c language:
//             pid_t waitpid (child_pid, &status, options);

// C program to demonstrate waitpid()

void waitexample()
{
    int i, stat;
    pid_t pid[5];
    for (i = 0; i < 5; i++)
    {
        if ((pid[i] = fork()) == 0)
        {
            sleep(1);
            exit(100 + i);
        }
    }

    // Using waitpid() and printing exit status
    // of children.
    for (i = 0; i < 5; i++)
    {
        pid_t cpid = waitpid(pid[i], &stat, 0);
        if (WIFEXITED(stat))
            printf("Child %d terminated with status: %d\n",
                   cpid, WEXITSTATUS(stat));
    }
}

// Driver code
int main()
{
    waitexample();
    return 0;
}

// Output:
//     Child 50 terminated with status: 100
//     Child 51 terminated with status: 101
//     Child 52 terminated with status: 102
//     Child 53 terminated with status: 103
//     Child 54 terminated with status: 104