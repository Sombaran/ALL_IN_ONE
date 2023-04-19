// // C++ Inline Function with Examples
//     The objective behind using a function is to save memory space.
//     When you call a function, it takes a lot of extra time in performing tasks 
//          such as jumping to the calling function.
//     Sometimes, the time taken for jumping to the calling function will be greater than
//          the time taken to execute that function.

//     You can overcome this problem with the help of macros.
//     But the main drawback of macros is that they are not functions that’s 
//          why the error checking won’t happen while compilation.

//     But in C++, you can make use of inline functions to completely ignore this problem.

//     Let us learn more about inline function in C++.

//     Inline Function in C++ from the name you can easily say that
//     an inline function is a function that is expanded in line when it is invoked. 
//          Also helps in saving time.
//     Using inline functions, the compiler will replace the function call with the
//          function code which will reduce the overhead of function calls.

//     Following function that compiler may not perform inlining:-

//         1. A function containing a loop.
//         2. If the function is recursive.
//         3. If there are static variables present in the function.
//         4. If there is a switch or goto statement in the function.
//         5. If a return statement exists in the function.

//     Syntax of C++ Inline Function:-

//     inline function-header
//     {
//         // body of the function
//     }

//     Example of C++ inline function

#include <iostream>
using namespace std;
inline int square(int a)
{
    return a * a;
}
inline int cube(int s)
{
    return s * s * s;
}
int main()
{
    int a = 5;
    cout << "The cube of 5 is: " << cube(5) << endl;
    cout << "Square of 5 is:  " << square(a) << endl;
    return 0;
}
// Output:-
//     The cube of 5 is: 125
//     Square of 5 is: 25

//     When to use inline function in C++?
//         1. If you need better performance, you can use an inline function.
//         2. Over macros, you can also use the inline function.
//         3. If you want to hide the implementation details of the function then you can use the inline keyword
//             outside the class with the function definition.

//     Points to be noted when using C++ inline functions
//         1. You should always keep your inline function small to get better efficiency and great results.
//         2. You should not turn all the functions into inline.
//             Because it might decrease the efficiency and might lead to code bloat.
//         3. You should define large functions outside the definition of a class using scope resolution.

//     Advantages of inline functions in C++
//         1. You don’t have to call a function in the inline function.
//             It avoids function call overhead.
//         2. With the help of an inline function, you can also save the overhead of the return statement from a function.
//         3. It does not require any stack.
//         4. It is more beneficial to the embedded systems because it requires less code.

//     Disadvantages of inline functions in C++
//         1. If you make use of several inline functions then the binary executable will also become large.
//         2. Use of several inline functions can reduce instruction cache hit rate.
//         3. It can increase compile time overhead if any changes are made inside the inline function and
//             then the code will have to recompile again to save the changes.
//         4. If the size of the binary executable file is large then 
//            it can cause thrashing.