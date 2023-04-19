// Why is the Size of an Empty Class Not Zero in C++?

// 1. When the structure was introduced in C, there was no concept of Objects
//      at that time. So, according to the C standard, it was decided to
//      keep the size of the empty structure to zero.

// 2. In C++, the Size of an empty structure/class is one byte
//      as to call a function at least empty structure/class should have some
//      size (minimum 1 byte is required)
//      i.e. one byte to make them distinguishable.

//  * Now to understand the size of an empty class.
//    let’s learn what is empty class is first!

// Empty class:
//      It is a class that does not contain any data members
//                  (e.g. int a, float b, char c, and string d, etc.)
//      An empty class may contain member functions.

// Why actually an empty class in C++ takes one byte?
//      Simply a class without an object requires no space allocated to it.
//      The space is allocated when the class is instantiated.
//      so 1 byte is allocated by the compiler to an object of an empty class
//          for its unique address identification.

//      If a class has multiple objects they can have different unique
//          memory locations. Suppose,
//      If a class does not have any size, what would be stored on
//          the memory location? That’s the reason.
//      when we create an object of an empty class in a C++ program,
//      It needs some memory to get stored, and the minimum amount of memory
//      that can be reserved is 1 byte.

//      Hence, if we create multiple objects of an empty class.
//      Every object will have a unique address.

#include <iostream>
using namespace std;
#ifndef NULL
// Creating an Empty Class
class Empty_class
{
    void fun();
};
// Driver Code
int main()
{
    cout << "Size of Empty Class is = "
         << sizeof(Empty_class);
    return 0;
}

//     The size of an empty class is not zero.
//     It is 1 byte generally. It is nonzero to ensure that the
//     two different objects will have different addresses.
#endif

// Creating an Empty Class
#ifndef NULL
class Empty
{
};

// Creating a Derived Class
class Derived : Empty
{
};

// Driver Code
int main()
{
    Derived d;
    cout << sizeof(d);
    return 0;
}

//  Note: The output is not greater than 4.
//      There is an interesting rule that says that an empty base class need
//      not be represented by a separate byte. So compilers are free to make
//      optimization in case of empty base classes.
#endif

// CPP Program as an exercise
#include <iostream>
using namespace std;

class Empty
{
};

class Derived1 : public Empty
{
};

class Derived2 : virtual public Empty
{
};

class Derived3 : public Empty
{
    char c;
};

class Derived4 : virtual public Empty
{
    char c;
};

class Dummy
{
    char c;
};

int main()
{
    cout << "sizeof(Empty) " << sizeof(Empty) << endl;       // 1
    cout << "sizeof(Derived1) " << sizeof(Derived1) << endl; // 1
    cout << "sizeof(Derived2) " << sizeof(Derived2) << endl; // 8
    cout << "sizeof(Derived3) " << sizeof(Derived3) << endl; // 1
    cout << "sizeof(Derived4) " << sizeof(Derived4) << endl; // 16
    cout << "sizeof(Dummy) " << sizeof(Dummy) << endl;       // 1

    return 0;
}
