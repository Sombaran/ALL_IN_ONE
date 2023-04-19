// Consteval is updated in (C++ 20)
/*
C++20 introduces the keyword consteval,
which is used to indicate that a function must evaluate at compile-time, otherwise a compile error will result. 
Such functions are called immediate functions.
*/

#include <iostream>
// consteval is defined in c++ 20 so it will not compile with c++ version less than 20.

consteval int greater(int x, int y) // function is now consteval
{
    return (x > y ? x : y);
}

int main()
{
    constexpr int g{greater(5, 6)};               // ok: will evaluate at compile-time
    std::cout << greater(5, 6) << " is greater!"; // ok: will evaluate at compile-time

    int x{5};                                     // not constexpr
    std::cout << greater(x, 6) << " is greater!"; // error: consteval functions must evaluate at compile-time

    return 0;
}

//example 1: Using consteval to make constexpr execute at compile-time C++20
/*
The downside of consteval functions is that such functions can’t evaluate at runtime, making them less flexible than constexpr functions, which can do either. Therefore, it would still be useful to have a convenient way to force constexpr functions to evaluate at compile-time (even when the return value is being used where a constant expression is not required), so that we could have compile-time evaluation when possible, and runtime evaluation when we can’t.

Consteval functions provides a way to make this happen, using a neat helper function:
*/
#include <iostream>

// Uses abbreviated function template (C++20) and `auto` return type to make this function work with any type of value
// See 'related content' box below for more info (you don't need to know how these work to use this function)
consteval auto compileTime(auto value)
{
    return value;
}

constexpr int greater(int x, int y) // function is constexpr
{
    return (x > y ? x : y);
}

int main()
{
    std::cout << greater(5, 6);              // may or may not execute at compile-time
    std::cout << compileTime(greater(5, 6)); // will execute at compile-time

    int x { 5 };
    std::cout << greater(x, 6);              // we can still call the constexpr version at runtime if we wish

    return 0;
}

/*
Note:

This works because consteval functions require constant expressions as arguments -- therefore, if we use the return value of a constexpr function as an argument to a consteval function, the constexpr function must be evaluated at compile-time! 
The consteval function just returns this argument as its own return value, so the caller can still use it.

Note that the consteval function returns by value. While this might be insfficient to do at runtime (if the value was some type that is expensive to copy, e.g. std::string), in a compile-time context, it doesn’t matter because the entire call to the consteval function will simply be replaced with the calculated return value.
*/