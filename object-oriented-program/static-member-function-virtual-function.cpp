// Can Static Functions Be Virtual in C++?
//     In C++, a static member function of a class cannot be virtual.
//     Virtual functions are invoked when you have a pointer or reference
//        to an instance of a class.
//     Static functions aren’t tied to the the instance of a class or object
//     But they are tied to the class.
//     C++ doesn’t have pointers-to-class,
//     so there is way to call/invoke a static function virtually.

#include <iostream>
using namespace std;
#ifndef NULL

// Point -1 : CPP Program to demonstrate Virtual member functions cannot be static
class Test
{
public:
    // error: member ‘fun’ cannot be declared both ‘virtual’ and ‘static’
    virtual static void fun()
    {
    }
};
#endif

// Point - 2 : static member function cannot be const and volatile. Following code also fails in compilation,
#ifndef NULL
class Test1
{
public:
    static void fun() const {}
};
#endif

// 	static member function
//	   1. static member functions do not have this pointer.

#ifndef NULL
class Test
{
    static Test *fun()
    {
        return this; // compiler error
    }
};

int main()
{
    Test T;
    return 0;
}
#endif
//    2) A static member function cannot be virtual.
//    3) A static member function can not be declared const, volatile, or const volatile.

// Point - 3 : Suppose form two overloaded function if any of them is a static member function
//              then compiler gives error.
//    For example, following program fails in compilation with error cannot be overloaded
//       void Test::fun()
//       static void Test::fun()
#ifndef NULL
class Test1
{
public:
    static void fun() {}
    void fun() {}
};

int main()
{
}
#endif