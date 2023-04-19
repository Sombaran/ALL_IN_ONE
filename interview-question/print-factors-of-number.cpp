//  Print factors of the given number

/*
    Then, for loop is executed with an initial condition i = 1 and checked whether n is perfectly divisible by i or not. If n is perfectly divisible by i then, i will be the factor of n.
*/

#include <iostream>
using namespace std;

int main()
{
    int value;
    cout << "Enter the value:";
    cin >> value;

    for (int i = 1; i <= value; i++)
    {
        if (value % i == 0)
            cout << i << " ";
    }
    cout << endl;
}