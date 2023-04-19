//      Function overloading and const keyword

#include <iostream>
using namespace std;
// 1. The two methods
//     'void fun() const'
//     'void fun()'
//     have same signature except that one is const and other is not.
//     The output, we observe that:
//     'const void fun()' is called on const object
//     'void fun()' is called on non-const object.

//     C++ allows member methods to be overloaded on the basis of const type.
//     Overloading on the basis of const type can be useful when a function return reference or pointer.
//     We can make one function const, that returns a const reference or const pointer,
//     other non-const function, that returns non-const reference or pointer.

//----------------------------------------------------------------------------
class Test
{
protected:
    int x;

public:
    Test(int i) : x(i) {}
    void fun() const
    {
        cout << "fun() const called " << endl;
    }
    void fun()
    {
        cout << "fun() called " << endl;
    }
};

int example1()
{
    Test t1(10);
    const Test t2(20);
    t1.fun();
    t2.fun();
    return 0;
}

// Output:
//     fun() called
//     fun() const called
//----------------------------------------------------------------------------

// 2. example

void fun(const int i)
{
    cout << "fun(const int) called ";
}
void fun(int i)
{
    cout << "fun(int ) called ";
}
int example2()
{
    const int i = 10;
    fun(i);
    return 0;
}
// Output:
//     Compiler Error: redefinition of 'void fun(int)'
//----------------------------------------------------------------------------

// 3. example
void fun(char *a)
{
    cout << "non-const fun() " << a;
}

void fun(const char *a)
{
    cout << "const fun() " << a;
}

int example3()
{
    const char *ptr = "GeeksforGeeks";
    fun(ptr);
    return 0;
}
// Output:
//     const fun() GeeksforGeeks

//----------------------------------------------------------------------------
// 4. example

void fun(const int &i)
{
    cout << "fun(const int &) called ";
}
void fun(int &i)
{
    cout << "fun(int &) called ";
}
int main()
{
    const int i = 10;
    fun(i);
    return 0;
}
// Output :
//     fun(const int &) called

//----------------------------------------------------------------------------
int main()
{
    example1();

    // C++ allows functions to be overloaded on the basis of const-ness of parameters only
    // if the const parameter is a reference or a pointer.
    // That is why the program 2 failed in compilation, but the program 3 worked fine.
    // This rule actually makes sense. In program 2,
    // The parameter 'i' is passed by value, so 'i' in fun() is a copy of 'i' in main().
    // Hence fun() cannot modify 'i' of main().

    // Therefore, it doesn't matter whether 'i' is received as a const parameter or normal parameter.
    // When we pass by reference or pointer, we can modify the value referred or pointed,
    // so we can have two versions of a function, one which can modify the referred or pointed value, other which can not.

    example2();
    example3();
    example4();

    return 0;
}