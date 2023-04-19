// Iterative C++ program to count
// number of digits in a number
#include <iostream>
using namespace std;
int main()
{
    long long value = 345289467;
    int count = 0;
    while (value != 0)
    {
        cout << "Number of digits : " << value << endl;
        value = value / 10;
        ++count;
    }
    cout << "Number of digits : " << count;
    return 0;
}

// This code is contributed
// by Akanksha Rai
