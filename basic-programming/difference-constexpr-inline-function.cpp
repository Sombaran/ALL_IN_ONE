//  constexpr vs inline function :

/*
  constexpr vs inline functions: Both are for performance improvements,
  Inline functions request the compiler to expand at compile time and save time of function call overheads.
  In inline functions, expressions are always evaluated at run time.

  constexpr is different, here expressions are evaluated at compile time.
  Example of performance improvement by constexpr: Consider the following C++ program
  */

// A C++ program to demonstrate the use of constexpr
#include <iostream>
using namespace std;

constexpr long int fib(int n)
{
    return (n <= 1) ? n : fib(n - 1) + fib(n - 2);
}

int main()
{
    // value of res is computed at compile time.
    constexpr long int res = fib(30);
    cout << res;
    return 0;
}

/*
    When the above program is run on GCC, it takes 0.003 seconds (We can measure time using time command) If we remove const from below line, then the value of fib(5) is not evaluated at compile time, because result of constexpr is not used in a const expression.

    Change,
        constexpr long int res = fib(30);  
    To,
        long int res = fib(30);

    constexpr vs const They serve different purposes. 
    constexpr is mainly for optimization while const is for practically const objects like value of Pi. 
    Both of them can be applied to member methods. 
    Member methods are made const to make sure that there are no accidental changes by the method. 
    On the other hand, the idea of using constexpr is to compute expressions at compile time so that time can be saved when code is run. 
    const can only be used with non-static member function whereas constexpr can be used with member and non-member functions, 
    even with constructors but with condition that argument and return type must be of literal types. 
*/