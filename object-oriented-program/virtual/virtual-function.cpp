//     Virtual Function in C++
//         It has numerous exciting and useful features and functionalities.

//     C++ Virtual Function
//         In C++, it is a member of a function in a class that we declare in the base class and
//             we also redefine it in a derived class.
//         It tells the compiler to perform dynamic linkage or late binding on the function.
//         The C++ programming language determines which function will be invoked at the runtime if the
//             function is defined as virtual and it is based on the type of object that is pointed by the base class.

//         The ‘virtual’ keyword is used to precede the normal declaration of a function.
//         There is a need to use the single pointer to refer to all the objects of the different classes.

//     Late Binding or Dynamic Linkage
//         In late binding, the compiler determines the type of the object at runtime.
//         And after that, it binds the function call.
//         Late binding is mainly used to resolve function calls during runtime.

//     Rules of Virtual Function in C++
//         1. It can’t be static.
//         2. It must be members of some other class.
//         3. It also can be a friend function of another class.
//         4. Virtual functions are always defined in the base class.
//         5. You can define a class as a virtual destructor but you can’t define it as a virtual constructor.
//         6. Always remember that the prototype of virtual functions should be the same in the base class
//             as well as the derived class.

//     Example:- C++ Virtual Function

#include <iostream>
using namespace std;

class VirtualClass
{
public:
    virtual void print()
    {
        cout << "Printing Base Class!" << endl;
    }

    void show()
    {
        cout << "Showing Base class!" << endl;
    }
};

class Tech : public VirtualClass
{
public:
    void print()
    {
        cout << "Printing Derived class!" << endl;
    }

    void show()
    {
        cout << "Just showing derived class!" << endl;
    }
};

int main()
{
    VirtualClass *ptr;
    Tech d;
    ptr = &d;
    ptr->print();
    ptr->show();
}