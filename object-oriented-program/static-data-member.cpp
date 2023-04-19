/*
 * static-object-static-function.cpp
 *
 *  Created on: 14-Mar-2022
 *      Author: Jotirling Swami
 */

//  Static data members in C++
//     Static data members are class members that are declared using static keywords.

//     Only one copy of that member is created for the entire class
//     It is shared by all the objects of that class, no matter how many objects are created.
//     It is initialized before any object of this class is being created
//     It is initialized even before main starts.
//     It is visible only within the class,
//     but its lifetime is the entire program

//     Syntax
//         static data_type data_member_name;

#include <iostream>
using namespace std;

#ifndef NULL
class Base
{
public:
    Base() { cout << "Base's Constructor Called " << endl; }
};

class Derived
{
    static Base var_base;

public:
    Derived() { cout << "Derived's Constructor Called " << endl; }
};

int main()
{
    Derived b;
    return 0;
}
// Output:
//     Derived's Constructor Called
#endif

//     The above program calls only Derived's Constructor Called ,
//     it doesn’t call Base's Constructor Called .
//     The reason :
//     static members are only declared in a class declaration,
//     static members are not defined in class.
//     They must be explicitly defined outside the class using the scope resolution operator.

//     If we try to access static member 'var_base' without an explicit definition of it, we will get a compilation error.
//     For example, the following program fails in the compilation.

#ifndef NULL
class Base
{
    int x;

public:
    Base() { cout << "Base's constructor called " << endl; }
};

class Derived
{
    static Base var_base;

public:
    Derived() { cout << "Derived's constructor called " << endl; }
    static Base getBaseVar() { return var_base; }
};

int main()
{
    Derived b;
    Base var_base = b.getBaseVar();
    return 0;
}
#endif

//  Solution:   If we add the definition of 'static Base var_base' the program will work fine and will call Base’s constructor.

#ifndef NULL
class Base
{
    int x;

public:
    Base() { cout << "Base's constructor called " << endl; }
};

class Derived
{
    static Base var_base;

public:
    Derived() { cout << "Derived's constructor called " << endl; }
    static Base getBaseVar() { return var_base; }
};

Base Derived::var_base;
int main()
{
    Derived d1, d2, d3;
    Base var_base = d1.getBaseVar();
    return 0;
}
// Output:
//     Base's constructor called
//     Derived's constructor called
//     Derived's constructor called
//     Derived's constructor called

#endif

// Note :
//     The Derived's constructor called 3 times for 3 objects (d1, d2, and d3),
//     but calls A’s constructor only once.
//     The reason is, static members are shared among all objects.
//     That is why they are also known as class members or class fields.
//     Also, static members can be accessed without any object,

// The static member ‘variable’ is accessed without any object.
#include <iostream>
using namespace std;

class Base
{
    int x;

public:
    Base() { cout << "Base's constructor called " << endl; }
};

class Derived
{
    static Base var_base;

public:
    Derived() { cout << "Derived's constructor called " << endl; }
    static Base getBaseVar() { return var_base; }
};

Base Derived::var_base; // definition of var_base

int main()
{
    // static member 'var_base' is accessed without any object of Derived
    Base var_base = Derived::getBaseVar();

    return 0;
}
