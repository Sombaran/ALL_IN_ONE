//  Exceptions while using delete keyword

//  1. Trying to delete Non - pointer object

#include <iostream>
using namespace std;
#ifndef NULL

int main()
{
    int x;
    // Delete operator always
    // requires pointer as input
    delete x; // error: type ‘int’ argument given to ‘delete’, expected pointer
    return 0;
}
#endif

//  2. Trying to delete pointer to a local stack allocated variable.

// #ifndef NULL
class Base
{
};

int main()
{
    Base x;
    Base *b = &x;

    // x is present on stack frame as local variable, only dynamically
    // allocated variables can be destroyed using delete operator
    delete b; // Runtime error // delete called on dynamically unallocated object ‘x’

    return 0;
}
// #endif