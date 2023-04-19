// Q) How many VPTR (Vtable pointer) will be created internally for following program?
#include <iostream>
using namespace std;

class A
{
public:
    virtual void f()
    {
        cout << "A:f()" << endl;
    }
};

class D : public A
{
    void f()
    {
        cout << "B:f()" << endl;
    }
};

// Option:
//     1. 0
//     2. 1
//     3. 2
//     4. 3

// Answer: 2
//      For the above scenario, 2 virtual table will be created that is
//      1 for base class and another 1 for derived class.
//      But, compiler set only one public VPTR in base class internally.
//      If 2nd Vtable is required to be initialized then the same VPTR is inherited and
//      used at run time. Note that Constructors initializes the VPTR.
//      So, answer is 1 public VPTR in base class.

// Q) How many VTable will be created for following program?

// class A
class A
{
public:
    virtual void f()
    {
        cout << "A:f()" << endl;
    }
};

class D : public A
{
    void f()
    {
        cout << "B:f()" << endl;
    }
};
// Option:
//     1. 0
//     2. 1
//     3. 2
//     4. 3
//  Answer: 3

/*
Q) Who initializes the VPTR?

1. Constructor initialize the VPTR
2. By default, initiation is done
3. Class itself
4. None

Answer - 1
    Constructor initializes the VPTR.
*/

/*
Q) What is VPTR ( V Table pointer)?

1. Holds the address of the V table
2. Holds the address of the virtual functions
3. Holds the address of pure virtual functions
4. None
Answer: 1
    VPTR is used to hold the address of the VTable. This pointer is set internally into base class as public V table pointer and accessible to all derived classes
*/


/*
Q) In C++, virtual function is used in which concept?

1. function overloading
2. function overriding
3. constructor overloading
4. All of the above

Answer: 2
    Virtual functions are used in function overriding concept in inheritance relationship

*/