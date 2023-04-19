// Understanding constexpr specifier in C++

/*

constexpr is a feature added in C++ 11.
The main idea is performance improvement of programs by doing computations at compile
time rather than run time.
Note that once a program is compiled and finalized by developer, it is run multiple times by users.
The idea is to spend time in compilation and save time at run time
(similar to template metaprogramming)
constexpr specifies that the value of an object or a function can be evaluated at compile time.
The expression can be used in other constant expressions.
For example, in below code product() is evaluated at compile time.
*/

// constexpr function for product of two numbers.
// By specifying constexpr, we suggest compiler to
// to evaluate value at compile time
#include <iostream>
using namespace std;

constexpr int greaterValue(int x, int y)
{
    return (x > y ? x : y);
}

int main()
{
    constexpr int x{5}; // not constexpr
    constexpr int y{6}; // not constexpr

    std::cout << greaterValue(x, y) << " is greater!"; // will be evaluated at runtime

    return 0;
}

/*
Note:
    A function be declared as constexpr
    In C++ 11, a constexpr function should contain only one return statement.
    C++ 14 allows more than one statement.
    constexpr function should refer only to constant global variables.
    constexpr function can call only other constexpr function not simple function.
    Function should not be of a void type and some operators like prefix increment (++v) are not allowed in constexpr function.
*/

// example 1: Constexpr functions can also be evaluated at runtime

//  Functions with a constexpr return value can also be evaluated at runtime, \
//  in which case they will return a non-constexpr result. For example:

#ifndef NULL

constexpr int greaterValue(int x, int y)
{
    return (x > y ? x : y);
}

int main()
{
    int x{5}; // not constexpr
    int y{6}; // not constexpr

    std::cout << greaterValue(x, y) << " is greater!"; // will be evaluated at runtime

    return 0;
}
#endif

// example 2: So when is a constexpr function evaluated at compile-time?
/*
You might think that a constexpr function would evaluate at compile-time whenever possible, but unfortunately this is not the case.
    According to the C++ standard, a constexpr function that is eligible for compile-time evaluation must be evaluated at compile-time if the return value is used where a constant expression is required. Otherwise, the compiler is free to evaluate the function at either compile-time or runtime.
*/

#ifndef NULL
constexpr int greaterValue(int x, int y)
{
    return (x > y ? x : y);
}

int main()
{
    constexpr int g{greaterValue(5, 6)}; // case 1: evaluated at compile-time
    std::cout << g << "is greater!";

    int x{5};                                     // not constexpr
    std::cout << greaterValue(x, 6) << " is greater!"; // case 2: evaluated at runtime

    std::cout << greaterValue(5, 6) << " is greater!"; // case 3: may be evaluated at either runtime or compile-time

    return 0;
}
#endif