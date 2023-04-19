// do - while Loop in C++

/*
The do...while loop is a variant of the while loop with one important difference:
    the body of do...while loop is executed once before the condition is checked.

    Its syntax is:

        do {
            // body of loop;
        }
        while (condition);

Here,
    The body of the loop is executed at first. Then the condition is evaluated.
    If the condition evaluates to true, the body of the loop inside the do statement is executed again.
    The condition is evaluated once again.
    If the condition evaluates to true, the body of the loop inside the do statement is executed again.
    This process continues until the condition evaluates to false. Then the loop stops.
*/

// Example 1: Display Numbers from 1 to 5

#include <iostream>
using namespace std;

#ifndef NULL

int main()
{
    int i = 1;

    // do...while loop from 1 to 5
    do
    {
        cout << i << " ";
        ++i;
    } while (i <= 5);

    return 0;
}
#endif

// Example 4: Sum of Positive Numbers Only

// program to find the sum of positive numbers
// If the user enters a negative number, the loop ends
// the negative number entered is not added to the sum

#include <iostream>
using namespace std;

int main()
{
    int number = 0;
    int sum = 0;

    do
    {
        sum += number;

        // take input from the user
        cout << "Enter a number: ";
        cin >> number;
    } while (number >= 0);

    // display the sum
    cout << "\nThe sum is " << sum << endl;

    return 0;
}

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