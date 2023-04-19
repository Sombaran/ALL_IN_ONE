// Defaulted function c++11
/*
Using "=default", we are leaving the implementation of function to compiler to implement the default behaviour. defaulted function(=default) is applicable to only 6 default functions(they will be provided by compiler If you don’t write in code):

Default Constructor
Default destructor
Copy Constructor
Copy Assignment Operator
Move constructor
Move Assignment Operator
Note: If you write any constructor then compiler will not provide default and parameterised constructor but copy and operator assignment will be provided.

Example:
    virtual ~test()=default;
    test(const test& obj)=default;
    test(test&& obj)=default;
    Advantages / Benefits:

Just looking at the declaration, we will get to know that this function has been implemented as defaulted function.
If any virtual function is present in class then we can make destructor as virtual and add "=default" to get default implementation.
It is simple and easy to write.
It is less error prone.
Disadvantages / Drawbacks / Limitations:

Compiler do swallow copy, if we are having "pointer" or "file handler" as data member then please don’t rely on compiler implementation then do the "deep copy" implementation in code.
Source code and example:

*/
#include <iostream>
using namespace std;
class test
{
    int m_iA;

public:
    test() {}
    virtual ~test() = default;
    test(const test &obj) = default;
    test(test &&obj) = default;
};
int main()
{
    std::cout << "Defaulted function" << std::endl;
    test obj1;
    test obj2(obj1);
    return 0;
}