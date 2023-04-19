// Name Mangling and extern "C" in C++

//     C++ supports function overloading, i.e.,
//     There can be more than one function with the same name but, different parameters.
//     How does the C++ compiler distinguish between different functions when it generates object code.
//         it changes names by adding information about arguments.
//         This technique of adding additional information to function names is called Name Mangling.
//         C++ standard doesn’t specify any particular technique for name mangling,
//         so different compilers may append different information to function names.

//     Consider the following example of Name Mangling, having the various declarations of function f():

int f(void) { return 1; }
int f(int) { return 0; }
void g(void) { int i = f(), j = f(0); }

// Some C++ compilers may mangle the above names to the following,

int __f_v(void) { return 1; }
int __f_i(int) { return 0; }
void __g_v(void) { int i = __f_v(), j = __f_i(0); }

//     Note: C does not support function overloading,
//           So, when we link a C code in C++, we have to make sure that name of a symbol is not changed.

//     How to handle C symbols when linking from C++?
//         In C, names may not be mangled as it doesn’t support function overloading.
//         So how to make sure that name of a symbol is not changed when we link a C code in C++.
//         For example, see the following C++ program that uses printf() function of C.

// C Program to demonstrate it doesn't support Name Mangling

#include <iostream>
using namespace std;
int printf(const char *format, ...);

// Driver Code
int main()
{
    printf("GeeksforGeeks");
    return 0;
}

//     Explanation:
//         The reason for compiler error is simple, the name of printf() is changed by the C++ compiler
//         It doesn’t find the definition of the function with a new name.

//     Solution: Extern "C" in C++.

//     When some code is put in the extern "C" block,
//     The C++ compiler ensures that the function names are un-mangled -
//         That the compiler emits a binary file with their names unchanged,
//         As a C compiler would do.

extern "C"
{
    int printf(const char *format, ...);
}

// Driver Code
int main()
{
    printf("GeeksforGeeks");
    return 0;
}

// Output
//     GeeksforGeeks

// Therefore, all C style header files (stdio.h, string.h, etc) have their declarations in the extern "C" block.

#ifdef __cplusplus
extern "C"
{
#endif
// Declarations of this file
// add C style header files
#ifdef __cplusplus
}
#endif

// Summery:
//     1. Since C++ supports function overloading, additional information has to be added to function names
//         (called Name mangling) to avoid conflicts in binary code.
//     2. Function names may not be changed in C as it doesn’t support function overloading.
//         To avoid linking problems, C++ supports the extern "C" block.
//         C++ compiler makes sure that names inside the extern "C" block are not changed.