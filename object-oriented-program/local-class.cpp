// Local Classes in C++
//     A class declared inside a function becomes local to that function and
//     It is called Local Class in C++.
//     A local class name can only be used locally i.e., inside the function and not outside it.
//     The methods of a local class must be defined inside it only.
//     A local class can have static functions.
//     A local class can not have static data members.

//     For example, in the following program, Test is a local class in fun().

#include <iostream>
using namespace std;

void fun()
{
    // local to fun
    class Test
    {
        // members of Test class
    };
}

// Driver Code
int main()
{
    return 0;
}

//-------------------------------------------------------------------------------
// Following are some interesting facts about Local Classes in C++:

// 1) A local class type name can only be used in the enclosing function.
//     following program, declarations of t and tp are valid in fun(), but invalid in main().

void fun()
{
    // Local class
    class Test
    {
        // Body
    };

    Test t;   // Fine
    Test *tp; // Fine
}

int main1()
{
    Test t;   // error: ‘Test’ was not declared in this scop
    Test *tp; // error: ‘tp’ was not declared in this scope; did you mean ‘tm’?
    return 0;
}
//-------------------------------------------------------------------------------

// 2) All the methods of Local classes must be defined inside the class only.
//    program 1 compile successfully
//    program 2 fails in the compilation.

// Program - 1 // compile success

void fun()
{
    class Test // local to fun
    {
    public:
        // Fine as the method is defined
        // inside the local class
        void method()
        {
            cout << "Local Class method() called";
        }
    };

    Test t;
    t.method();
}

int main()
{
    fun();
    return 0;
}

// Program - 2 // compile fail

void fun()
{
    class Test // local to fun
    {
    public:
        void method();
    };

    // Error as the method is defined outside the local
    // class
    void Test::method() // error: qualified-id in declaration before ‘(’ token
    {
        cout << "Local Class method()";
    }
}

int main()
{
    return 0;
}
//-------------------------------------------------------------------------------

// 3) A Local class cannot contain static data members. It may contain static functions though.
//     program 1 fails in compilation,
//     program 2 works fine.

// Program - 1 // compile success

void fun()
{
    class Test // local to fun
    {
        static int i; // error: local class shall not have static data member
    };
}

int main()
{
    return 0;
}

// Program - 2 // compile failure

// C++ program without any compilation error to demonstrate
//     that a Local class cannot contain static data members

void fun()
{
    class Test // local to fun
    {
    public:
        static void method() // works fine
        {
            cout << "Local Class method() called";
        }
    };

    Test::method();
}

int main()
{
    fun();
    return 0;
}
//-------------------------------------------------------------------------------

// 4) Member methods of the local class can only access static and enum variables
//    of the enclosing function.
//    Non-static variables of the enclosing function are not accessible inside local classes.
//    program 1 compiles and runs fine.
//    program 2 fails in the compilation.

// Program - 1 // compiles success

void fun()
{
    static int x;
    enum
    {
        i = 1,
        j = 2
    };

    // Local class
    class Test
    {
    public:
        void method()
        {
            cout << "x = " << x
                 << endl; // fine as x is static
            cout << "i = " << i
                 << endl; // fine as i is enum
        }
    };

    Test t;
    t.method(); // error
}

int main()
{
    fun();
    return 0;
}

// Program - 2 // compilation fail

void fun()
{
    int value = 10;
    class Base
    {
    public:
        Base()
        {
            cout << Value << endl; //  error: ‘Value’ was not declared in this scope;
        }
    };
    Base b;
}

int main()
{
    fun();
    return 0;
}
//-------------------------------------------------------------------------------

// 5) Local classes can access global types, variables, and functions.
//    Also, local classes can access other local classes of the same function.
//    For example, the following program works fine.

int x;

void fun()
{
    // First Local class
    class Test1
    {
    public:
        Test1() { cout << "Test1::Test1()" << endl; }
    };

    // Second Local class
    class Test2
    {
        // Fine: A local class can use other local classes
        // of same function
        Test1 t1;

    public:
        void method()
        {
            // Fine: Local class member methods can access
            // global variables.
            cout << "x = " << x << endl;
        }
    };

    Test2 t;
    t.method();
}

int main()
{
    fun();
    return 0;
}

// Output
//     Test1::Test1()
//     x = 0
//-------------------------------------------------------------------------------