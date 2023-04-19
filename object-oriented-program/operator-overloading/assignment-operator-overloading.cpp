
//  When should we write our own assignment operator in C++?
//     The answer is same as Copy Constructor:
//         If a class does not contain pointers, then there is no need to write assignment operator
//             and copy constructor.
//         The compiler creates a default copy constructor and assignment operators for every class.

//         The compiler created copy constructor and assignment operator may not be sufficient.
//         when we have pointers or any run time allocation of resource like file handle,
//             a network connection..etc. For example, consider the following program.

//  The compiler does not create default assignment operator in the following cases:
//     1. Class has a non-static data member of a const type or a reference type.
//     2. Class has a non-static data member of a type that has an inaccessible
//         copy assignment operator.
//     3. Class is derived from a base class with an inaccessible copy assignment operator.

//     When any of the above conditions is true
//     Then user must define his own assignment operator.

#include <iostream>
using namespace std;

#ifndef NULL
// A class compiler default assignment operator
class Test
{
    int *ptr;

public:
    Test(int i = 0) { ptr = new int(i); }
    void setValue(int i) { *this->ptr = i; }
    int getValue() { return *ptr; }
};

int main()
{
    Test T1(10);
    Test T2(5);

    cout << "Before assignment value of T1: " << T1.getValue() << endl;
    cout << "Before assignment value of T2: " << T2.getValue() << endl;

    T2 = T1; // here it will call the default/compiler givern assignment operator

    cout << "After assignment Operator T2 = T1 value of T1: " << T1.getValue() << endl;
    cout << "After assignment Operator T2 = T1 value of T2: " << T2.getValue() << endl;

    T1.setValue(100);

    cout << "After Setting T1 Object value of T1: " << T1.getValue() << endl;
    cout << "After Setting T1 Object value of T2: " << T2.getValue() << endl;
    return 0;
}

//   Output of above program is "100". If we take a look at main(),
//     we modified ‘T1’ object using setValue() function
//     but the changes are also reflected in object ‘T2’.
//     This type of unexpected changes cause problems.

//     Since there is no user defined assignment operator in the above program.
//     Compiler creates a default assignment operator.
//     Which copies ‘ptr’ of T1 object right hand side to T2 object left hand side.
//     So both ‘ptr’s start pointing to the same location.

#endif

//     We can handle the above problem in two ways.
//     1) Do not allow assignment of one object to other object.
//          We can create our own dummy assignment operator and make it private.

//     2) Write your own assignment operator that does deep copy assignment.
//         Its Same is true for Copy Constructor.

#ifndef NULL
class Test
{
    int *ptr;

public:
    Test(int i = 0) { ptr = new int(i); }
    void setValue(int i) { *this->ptr = i; }
    int getValue() { return *ptr; }
    Test &operator=(const Test &obj);
};

Test &Test::operator=(const Test &obj)
{
    // Check for self assignment
    if (this != &obj)
        *this->ptr = *(obj.ptr);
    return *this;
}

int main()
{
    Test T1(10);
    Test T2(5);
    cout << "Before assignment value of T1: " << T1.getValue() << endl;
    cout << "Before assignment value of T2: " << T2.getValue() << endl;

    T2 = T1; // here it will call the default/compiler givern assignment operator

    cout << "After assignment Operator T2 = T1 value of T1: " << T1.getValue() << endl;
    cout << "After assignment Operator T2 = T1 value of T2: " << T2.getValue() << endl;

    T1.setValue(100);

    cout << "After Setting T1 Object value of T1: " << T1.getValue() << endl;
    cout << "After Setting T1 Object value of T2: " << T2.getValue() << endl;
    return 0;
}

//   Output of above program is "100". If we take a look at main(),
//     we modified ‘T1’ object using setValue() function
//     but the changes are bot reflected in object ‘T2’.
//     Because T1 pointer data member are pointing to different addree allocation.
//     So both ‘ptr’s are not pointing to the same location.
#endif

// assignment
//     a=b
//     a+=b
//     a-=b
//     a*=b
//     a/=b
//     a%=b
//     a&=b
//     a|=b
//     a^=b
//     a<<=b
//     a>>=b

// #ifndef NULL
class Test
{
    int *ptr;

public:
    Test(int i = 0) { ptr = new int(i); }
    void setValue(int i) { *this->ptr = i; }
    int getValue() { return *ptr; }

    Test &operator=(const Test &obj);   // a=b
    Test &operator+=(const Test &obj);  // a+=b
    Test &operator-=(const Test &obj);  // a-=b
    Test &operator*=(const Test &obj);  // a*=b
    Test &operator/=(const Test &obj);  // a/=b
    Test &operator%=(const Test &obj);  // a%=b
    Test &operator&=(const Test &obj);  // &=b
    Test &operator|=(const Test &obj);  // a|=b
    Test &operator^=(const Test &obj);  // a^=b
    Test &operator<<=(const Test &obj); // a<<=b
    Test &operator>>=(const Test &obj); // a>>=b
};

Test &Test::operator=(const Test &obj)
{
    // Check for self assignment
    if (this != &obj)
        *this->ptr = *(obj.ptr);

    return *this;
}

Test &Test::operator+=(const Test &obj)
{
    // Check for self assignment
    if (this != &obj)
        *this->ptr = *this->ptr + *(obj.ptr);
    return *this;
}

Test &Test::operator-=(const Test &obj)
{
    // Check for self assignment
    if (this != &obj)
        *this->ptr = *this->ptr - *(obj.ptr);
    return *this;
}

Test &Test::operator*=(const Test &obj)
{
    // Check for self assignment
    if (this != &obj)
        *this->ptr = *this->ptr * *(obj.ptr);
    return *this;
}

Test &Test::operator/=(const Test &obj)
{
    // Check for self assignment
    if (this != &obj)
        *this->ptr = *this->ptr / *(obj.ptr);
    return *this;
}

Test &Test::operator%=(const Test &obj)
{
    // Check for self assignment
    if (this != &obj)
        *this->ptr = *this->ptr * *(obj.ptr);
    return *this;
}

Test &Test::operator&=(const Test &obj)
{
    // Check for self assignment
    if (this != &obj)
        *this->ptr = *this->ptr & *(obj.ptr);
    return *this;
}

Test &Test::operator|=(const Test &obj)
{
    // Check for self assignment
    if (this != &obj)
        *this->ptr = *this->ptr | *(obj.ptr);
    return *this;
}

Test &Test::operator^=(const Test &obj)
{
    // Check for self assignment
    if (this != &obj)
        *this->ptr = *this->ptr ^ *(obj.ptr);
    return *this;
}

Test &Test::operator<<=(const Test &obj)
{
    // Check for self assignment
    if (this != &obj)
        *this->ptr = *this->ptr << *(obj.ptr);
    return *this;
}

Test &Test::operator>>=(const Test &obj)
{
    // Check for self assignment
    if (this != &obj)
        *this->ptr = *this->ptr >> *(obj.ptr);
    return *this;
}

int main()
{
    Test T1(2);
    Test T2(4);
    cout << "Before assignment value of T1: " << T1.getValue() << endl;
    cout << "Before assignment value of T2: " << T2.getValue() << endl;

    T2 = T1;

    cout << "After assignment Operator T2 = T1 value of T1: " << T1.getValue() << endl;
    cout << "After assignment Operator T2 = T1 value of T2: " << T2.getValue() << endl;

    T1.setValue(8);

    cout << "After Setting T1 Object value of T1: " << T1.getValue() << endl;
    cout << "After Setting T1 Object value of T2: " << T2.getValue() << endl;

    T2 += T1;

    cout << "After assignment Operator T2 += T1 value of T2: " << T2.getValue() << endl;

    T2 -= T1;

    cout << "After assignment Operator T2 -= T1 value of T2: " << T2.getValue() << endl;

    T2 *= T1;

    cout << "After assignment Operator T2 *= T1 value of T2: " << T2.getValue() << endl;

    T2 /= T1;

    cout << "After assignment Operator T2 /= T1 value of T2: " << T2.getValue() << endl;

    T2 %= T1;

    cout << "After assignment Operator T2 %= T1 value of T2: " << T2.getValue() << endl;
    T1.setValue(13);
    T2.setValue(60);
    // T2               =>  60 = 0011 1100
    // T1               =>  13 = 0000 1101
    // T2 = T2 << T1    =>  12 = 0000 1100
    T2 &= T1;

    cout << "After assignment Operator T2 &= T1 value of T2: " << T2.getValue() << endl;
    T1.setValue(13);
    T2.setValue(60);
    // T2               =>  60 = 0011 1100
    // T1               =>  13 = 0000 1101
    // T2 = T2 << T1    =>  61 = 0011 1101
    T2 |= T1;

    cout << "After assignment Operator T2 |= T1 value of T2: " << T2.getValue() << endl;
    T1.setValue(13);
    T2.setValue(60);
    // T2               =>  60 = 0011 1100
    // T1               =>  13 = 0000 1101
    // T2 = T2 << T1    =>  49 = 0011 0001

    T2 ^= T1;

    cout << "After assignment Operator T2 ^= T1 value of T2: " << T2.getValue() << endl;

    T1.setValue(2);
    T2.setValue(60);
    T2 <<= T1;
    // T2               =>  60 = 0011 1100
    // T1               =>  13 = 0000 1101
    // T2 = T2 << T1    => 240 = 1111 0000
    cout << "After assignment Operator T2 <<= T1 value of T2: " << T2.getValue() << endl;

    T1.setValue(2);
    T2.setValue(60);
    // T2               =>  60 = 0011 1100
    // T1               =>  13 = 0000 1101
    // T2 = T2 >> T1    =>  15 = 0000 1111
    T2 >>= T1;

    cout << "After assignment Operator T2 >>= T1 value of T2: " << T2.getValue() << endl;

    return 0;
}
// #endif