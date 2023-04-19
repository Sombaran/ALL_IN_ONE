/*
1. This is a special member function with same name as your class with ~ at the front.
2. Used to destruct the memory of the object which was created by constructor.
3. Destructor get called automatically (implicit call) as constructor gets call.
4. We can call destructor explicitly but its not good pratice.
5. The only place where you will call destructor explicitly is after placement new.
6. You can not pass the paramerter to Destructor, if you pass parameter then complier will through the error.
*/

#include <iostream>
using namespace std;

class Base
{
    int *value;

public:
    Base() : value{nullptr} { cout << "Default constructor" << endl; }
    Base(int *val) : value{val} { cout << "Parameterised constructor" << endl; }

    ~Base()
    {
        delete value;
        cout << "Default Destructor" << endl;
    }
};

int main()
{
    Base b1(new int(10));

    return 0;
}