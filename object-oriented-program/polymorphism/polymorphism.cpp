// polymorphism

//     The word polymorphism means having many forms.
//     In simple words, we can define polymorphism as the ability of a message to be displayed in more than one form.
//     A real-life example of polymorphism, a person at the same time can have different characteristics.
//     Like a man at the same time is a father, a husband, an employee.
//     So the same person posses different behavior in different situations.
//     This is called polymorphism.
//     Polymorphism is considered as one of the important features of Object Oriented Programming.

//     In C++ polymorphism is mainly divided into two types:
//         1. Compile time Polymorphism
//         2. Runtime Polymorphism

// 1. Compile time Polymorphism :
//     This type of polymorphism is achieved by function overloading and operator overloading.

//     Function Overloading:
//         When there are multiple functions with same name but different parameters then these functions are said to be overloaded.
//         Functions can be overloaded by :
//             - change in number of arguments
//             - change in type of arguments.

// C++ program for function overloading

#include <iostream>
using namespace std;

class Geeks
{
public:
    // function with 1 int parameter
    void func(int x)
    {
        cout << "value of x is " << x << endl;
    }

    // function with same name but 1 double parameter
    void func(double x)
    {
        cout << "value of x is " << x << endl;
    }

    // function with same name and 2 int parameters
    void func(int x, int y)
    {
        cout << "value of x and y is " << x << ", " << y << endl;
    }
};

//----------------------------------------------------------------------------
class Test
{
    static void fun(int i) {}
    void fun(int i) {}
};

int fail_overloading1()
{
    Test t;
    getchar();
    return 0;
}
//----------------------------------------------------------------------------
int foo()
{
    return 10;
}

char foo()
{
    return 'a';
}

int fail_overloading2()
{
    char x = foo();
    getchar();
    return 0;
}
//----------------------------------------------------------------------------

int f(int x)
{
    return x + 10;
}

int f(const int x)
{
    return x + 10;
}

int fail_overloading3()
{
    getchar();
    return 0;
}
//----------------------------------------------------------------------------

int f(int x, int y)
{
    return x + 10;
}

int f(int x, int y = 10)
{
    return x + y;
}

int fail_overloading4()
{
    getchar();
    return 0;
}
//----------------------------------------------------------------------------
int main()
{

    Geeks obj1;

    // Which function is called will depend on the parameters passed
    // The first 'func' is called
    obj1.func(7);

    // The second 'func' is called
    obj1.func(9.132);

    // The third 'func' is called
    obj1.func(85, 64);

    // Rule of functin overloading:

    // 1. Member function declarations with the same name and the name parameter-type-list cannot be overloaded
    // if any of them is a static member function declaration.
    fail_overloading1();

    // 2. Function declarations that differ only in the return type.
    // For example, the following program fails in compilation.
    fail_overloading2();

    // 3. Parameter declarations that differ only in a pointer * versus an array [] are equivalent.
    // That is, the array declaration is adjusted to become a pointer declaration.
    // Only the second and subsequent array dimensions are significant in parameter types.

    // int fun(int *ptr);
    // int fun(int ptr[]); // redeclaration of fun(int *ptr)

    // 4. Parameter declarations that differ only in that one is a function type and the other is
    // a pointer to the same function type are equivalent.

    // void h(int());
    // void h(int (*)()); // redeclaration of h(int())

    // 5. Parameter declarations that differ only in the presence or absence of const and/or volatile are equivalent.
    fail_overloading3();

    // 6. Two parameter declarations that differ only in their default arguments are equivalent
    fail_overloading4();

    return 0;
}
