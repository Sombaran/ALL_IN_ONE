// Can a C++ class have an object of self type?

// 1. A class declaration can contain static object of self type and pointer of self type,

// Example 1- A class can have a static member of self type
#include <iostream>
using namespace std;
#ifndef NULL
class Test
{
    static Test self; // works fine

    /* other stuff in class*/
};

int main()
{
    Test t;

    return 0;
}
#endif

// Example 2 - It can also have pointer to self type.
#ifndef NULL
class Test
{
    Test *self; // works fine

    /* other stuff in class*/
};

int main()
{
    Test t;

    return 0;
}
#endif

// Example 3 - It cannot have a non - static object of self type.

// #ifndef NULL
class Test
{
    Test self; // Error

    /* other stuff in class*/
};

int main()
{
    Test t;
    return 0;
}
// #endif

// Note:
//     If a non - static object is member then declaration of class is incomplete
//     The compiler has no way to find out size of the objects of the class.
//     Static variables do not contribute to the size of objects.
//     So no problem in calculating size with static variables of self type.
//     For a compiler, all pointers have a fixed size irrespective of the data type
//          they are pointing to, so no problem with this also.