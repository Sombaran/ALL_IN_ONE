//  Functor in C++.

//     Functor is also knows as partialy applied function

//     Consider a function1(int ) that takes only one argument.
//     However, while calling function1(int ) we have a lot more information that
//     We would like to pass to that, but we cannot as it accepts only one parameter.

//     What can be done?

//     Functors are objects that can be treated as they are a function or
//     function pointer.
//     Functors are most commonly used along with STLs in a scenario like following:

// A C++ program uses transform() in STL to add
// 1 to all elements of arr[]

#include <iostream>
#include <algorithm>
using namespace std;

#ifndef NULL
int increment(int x)
{
    return (x + 1);
}

int main()
{
    int arr[] = {1, 2, 3, 4, 5};
    int n = sizeof(arr) / sizeof(arr[0]);

    // Apply increment to all elements of
    // arr[] and store the modified elements
    // back in arr[]
    transform(arr, arr + n, arr, increment);

    for (int i = 0; i < n; i++)
        cout << arr[i] << " ";

    return 0;
}
#endif

// What is functor :
// Awswer : It work as function, and achieved by overloading "operator ()" in some class.

class Multiply
{
    int value;

public:
    Multiply(int val)
    {
        value = val;
    }
    int operator()(int multi_val)
    {
        return value * multi_val; // we are multiplying with multi_val to existing value of the class data memeber.
    }
};

int SampleFunctor()
{
    Multiply mul(12);
    cout << mul(2) << endl;
    cout << mul(3) << endl;

    return 0;
}
/*
  Function Object As known As Functors in C++
    Functors in C++ are short for function objects.
    Function objects are instances of C++ classes that have the operator() defined,
    Which gives the class object function semantics.
    In the class-like functions constructed in C++, the function is composed of the return type, function name, parameters, and function body.
    The basic accepted form of functors looks like this...


    class Functor
    {
    public:
        R operator()(P1, ..., Pn)
        {
            return R();
        }
    };

*/

struct doSomethingFunctor
{
    int i;
    bool b;
    double d;
    bool b1;
    doSomethingFunctor(int i, bool b, double d, bool b1) : i(i), b(b), d(d), b1(b1)
    {
    }
    bool operator()(bool b1)
    {
        return this->b1 == b1;
    }
};


int complexFunctor()
{
    doSomethingFunctor d1(100, false, 0.99, false);
    doSomethingFunctor d2(100, false, 0.99, true);

    cout << d1(true) << endl;
    cout << d2(true) << endl;

    return 0;
}

int main()
{
    SampleFunctor();
    complexFunctor();
}