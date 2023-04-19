// C++ while Loop
/*

In this tutorial, we will learn the use of while and do...while loops in C++ programming with the help of some examples.

In computer programming, loops are used to repeat a block of code.

For example, let's say we want to show a message 100 times. Then instead of writing the print statement 100 times, we can use a loop.

That was just a simple example; we can achieve much more efficiency and sophistication in our programs by making effective use of loops.

There are 3 types of loops in C++.
    1. for loop
    2. while loop
    3. do...while loop

    The syntax of the while loop is:
        while (condition) {
            // body of the loop
        }

    Here,
    A while loop evaluates the condition
        If the condition evaluates to true, the code inside the while loop is executed.
    The condition is evaluated again.
        This process continues until the condition is false.
    When the condition evaluates to false, the loop terminates.
*/

// Example 1: Display Numbers from 1 to 5
// C++ Program to print numbers from 1 to 5

#include <iostream>
using namespace std;

#ifndef NULL

int main()
{
    int i = 1;

    // while loop from 1 to 5
    while (i <= 5)
    {
        cout << i << " ";
        ++i;
    }

    return 0;
}
#endif
// Example 2: Sum of Positive Numbers Only

// program to find the sum of positive numbers
// if the user enters a negative number, the loop ends
// the negative number entered is not added to the sum

int main()
{
    int number;
    int sum = 0;

    // take input from the user
    cout << "Enter a number: ";
    cin >> number;

    while (number >= 0)
    {
        // add all positive numbers
        sum += number;

        // take input again if the number is positive
        cout << "Enter a number: ";
        cin >> number;
    }

    // display the sum
    cout << "\nThe sum is " << sum << endl;

    return 0;
}

/*
    In this program, the user is prompted to enter a number, which is stored in the variable number.

    In order to store the sum of the numbers, we declare a variable sum and initialize it to the value of 0.

    The while loop continues until the user enters a negative number. During each iteration, the number entered by the user is added to the sum variable.

    When the user enters a negative number, the loop terminates. Finally, the total sum is displayed.
*/

/*
    Infinite while loop
        If the condition of a loop is always true, the loop runs for infinite times (until the memory is full).

    For example,
        // infinite while loop
        while(true) {
            // body of the loop
        }


    Here is an example of an infinite do...while loop.
        // infinite do...while loop
        int count = 1;
        do {
        // body of loop
        }
        while(count == 1);

    In the above programs, the condition is always true.
    Hence, the loop body will run for infinite times.

*/