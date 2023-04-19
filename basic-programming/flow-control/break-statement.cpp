//   C++ break Statement

/*
In this tutorial, we will learn about the break statement and its working in loops with the help of examples.

In C++, the break statement terminates the loop when it is encountered.

The syntax of the break statement is:
    break;

Before you learn about the break statement, make sure you know about:
1. C++ for loop
2. C++ if...else
3. C++ while loop
*/

// Example 1: break with for loop
// program to print the value of i

#include <iostream>
using namespace std;
#ifndef NULL

int main()
{
    for (int i = 1; i <= 5; i++)
    {
        // break condition
        if (i == 3)
        {
            break;
        }
        cout << i << endl;
    }

    return 0;
}
#endif

/*
Note:
    if (i == 3) {
        break;
    }

This means, when i is equal to 3, the break statement terminates the loop. Hence, the output doesn't include values greater than or equal to 3.
*/

// Example 2: break with while loop

// program to find the sum of positive numbers
// negative number -> loop terminate
// the negative number entered is not added to sum
#ifndef NULL
int main()
{
    int number;
    int sum = 0;

    while (true)
    {
        // take input from the user
        cout << "Enter a number: ";
        cin >> number;

        // break condition
        if (number < 0)
        {
            break;
        }

        // add all positive numbers
        sum += number;
    }

    // display the sum
    cout << "The sum is " << sum << endl;

    return 0;
}
#endif

// Example 3: break with Nested loop

// using break statement inside
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
            if (i == 2)
            {
                break;
            }
            cout << "i = " << i << ", j = " << j << endl;
        }
    }

    return 0;
}