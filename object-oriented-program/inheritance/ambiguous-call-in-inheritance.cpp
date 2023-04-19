// Avoiding Ambiguity in Inheritance
// In some cases of inheritance, the compiler faces ambiguity.An example of such a case is

#include <iostream>
using namespace std;

#ifndef NULL
class A
{
protected:
    int a;
};
class B : public A
{
};
class C : public A
{
};
class D : public B, public C
{
public:
    void fun()
    {
        a = 7; // error: reference to ‘a' is ambiguous
    }
    void display()
    {
        cout << "a from B = " << a << endl; // error: reference to ‘a' is ambiguous
        cout << "a from C = " << a << endl; // error: reference to ‘a' is ambiguous
    }
};
int main()
{
    D obj;
    obj.fun();
    obj.display();
    return 0;
}
#endif

// Solution:
//     This happened because class D has two copies of variable a,
//     one via class B and other via class C.
//     We can avoid such ambiguity in two ways.

// Solution 1:
//     1. Using scope resolution operator (::)

#include <iostream>
using namespace std;

#ifndef NULL
class A
{
protected:
    int a;
};
class B : public A
{
};
class C : public A
{
};
class D : public B, public C
{
public:
    void fun()
    {
        B::a = 7;
        C::a = 10;
    }
    void display()
    {
        cout << "a from B = " << B::a << endl;
        cout << "a from C = " << C::a << endl;
    }
};
int main()
{
    D obj;
    obj.fun();
    obj.display();
    return 0;
}
#endif

// 2. Using the virtual base class
//     Example of avoiding ambiguity using virtual base class

#include <iostream>
using namespace std;
class A
{
protected:
    int a;
};
class B : virtual public A
{
};
class C : virtual public A
{
};
class D : public B, public C
{
public:
    void fun()
    {
        a = 7;
    }
    void display()
    {
        cout << "a from B = " << a << endl;
        cout << "a from C = " << a << endl;
    }
};
int main()
{
    D obj;
    obj.fun();
    obj.display();
    return 0;
}