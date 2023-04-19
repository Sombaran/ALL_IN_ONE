
// Function Overloadmg in C++

//      Basis               Function Overloading
//      Inheritance         Can happen Without inheritance
//      Signature           Function signatures of overloaded functions must be different
//      Polymorphism        Compile time
//      Scope               Overloaded functions are in the same scope
//      Purpose             To have multiple functions With same name that act differently depending
//                          on parameters
//      Number of times     A function can be overloaded multiple times

//     1. In C++, two or more functions can have the same name if the number AND/OR type
//         of parameters are different, this is called function overloading.
//     2. Thus, overloaded functions are functions that have the same name but
//         different parameters.
//     3. However, if in two or more functions, only return type is different keeping number and
//         type of parameters same, it is not considered function overloading.
//         In this case, the compiler gives an error.
//     4. Function overloading is achieved during compile time.
//     5. It is a form of compile time polymorphism.

// Example to illustrate function overloading in C+ +

#include <iostream>
using namespace std;

int add(int a, int b)
{
    return a + b;
}

int add(int a, int b, int c)
{
    return a + b + c;
}

double add(double a, double b)
{
    return a + b;
}

int function_overloading()
{
    int x = 3, y = 7, z = 12;
    double n1 = 4.56, n2 = 13.479;

    cout << "x+y = " << add(x, y) << endl;
    cout << "x+y+z = " << add(x, y, z) << endl;
    cout << "n1+n2 = " << add(n1, n2) << endl;

    return 0;
}
// Output
//     x+y = 10
//     X+y+z = 22
//     n1+n2 = 18.039

// Function Overriding In C++

//      Basis               Function Overriding
//      Inheritance         Happens only when a class inherits from another class
//      Signature           Same function signatures
//      Polymorphism        Runtime
//      Scope               Functions are in different scope
//      Purpose             To perform additional or different tasks than the base class function
//      Number of times     A function is overridden single time in its derived class

//     1. When a member function of a base class is redefined in its derived class
//        With the same parameters and return type, it is called function overriding in C++.
//     2. Now, if we use an object of the derived class to call this function,
//        the function defined in the derived class is invoked.
//     3. The base class function is said to be overridden.
//     4. Function overriding is achieved during runtime.
//     5. It is a form of runtime polymorphism.

// Example to illustrate function overriding in C+ +

#include <iostream>
using namespace std;

class base
{
public:
    virtual void display()
    {
        cout << "Function of base class" << endl;
    }
};
class derived : public base
{
public:
    void display()
    {
        cout << "Function of derived class" << endl;
    }
};

int function_overridding()
{
    derived d1;
    d1.display();
    return 0;
}

// Output
//     Function of derived class

int main()
{
    function_overloading();
    function_overridding();
}