// C++ continue Statement
/*
In computer programming, the continue statement is used to skip the current iteration of the loop and the control of the program goes to the next iteration.

The syntax of the continue statement is:
    continue;


Before you learn about the continue statement, make sure you know about,
1. C++ for loop
2. C++ if...else
3. C++ while loop

*/

// Example 1: continue with for loop
// program to print the value of i

#include <iostream>
using namespace std;

#ifndef NULL

int main()
{
    for (int i = 1; i <= 5; i++)
    {
        // condition to continue
        if (i == 3)
        {
            continue;
        }
        cout << "continue: " << i << endl;
    }

    return 0;
}
#endif

/*
This means
    When i is equal to 3, the continue statement skips the current iteration and
        starts the next iteration
    Then, i becomes 4, and the condition is evaluated again.
    Hence, 4 and 5 are printed in the next two iterations.

    Note:
        The continue statement is almost always used with decision-making statements.
*/

// Example 2: continue with while loop

// In a while loop, continue skips the current iteration and control flow of the
//      program jumps back to the while condition.

// program to calculate positive numbers till 50 only
// if the user enters a negative number,
// that number is skipped from the calculation

// negative number -> loop terminate
// numbers above 50 -> skip iteration

#ifndef NULL
int main()
{
    int sum = 0;
    int number = 0;

    while (number >= 0)
    {
        // add all positive numbers
        sum += number;

        // take input from the user
        cout << "Enter a number: ";
        cin >> number;

        // continue condition
        if (number > 50)
        {
            cout << "The number is greater than 50 and won't be calculated." << endl;
            number = 0; // the value of number is made 0 again
            continue;
        }
    }

    // display the sum
    cout << "The sum is " << sum << endl;

    return 0;
}
#endif

// Example 3: continue with Nested loop
//  When continue is used with nested loops, it skips the current iteration of the inner loop. For example,

// using continue statement inside
// nested for loop

#include <iostream>
using namespace std;

int main()
{
    int number;
    int sum = 0;

    // nested for loops

    // first loop
    for (int i = 1; i <= 3; i++)
    {   
        // second loop
        for (int j = 1; j <= 3; j++)
        {
            if (j == 2)
            {
                continue;
            }
            cout << "i = " << i << ", j = " << j << endl;
        }
    }

    return 0;
}