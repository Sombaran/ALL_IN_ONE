

// Check the number is PRIME or NOT :

#include <iostream>
using namespace std;
int value = 81;
bool is_prime = true;

int main()
{
    // 0 and 1 are not prime numbers
    if (value <= 1)
        return false;

    // loop to check if n is prime
    for (int i = 2; i <= value / 2; ++i)
    {
        if (value % i == 0)
        {
            is_prime = false;
            break;
        }
    }

    if (is_prime)
        cout << value << " is a prime number";
    else
        cout << value << " is not a prime number";

    return 0;
}
