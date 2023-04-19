/////////////////////////////////////////////////////////////////////
//  initialization.cpp - demonstrate initialization sequence       //
//                                                                 //
//  Jotirling Swami  -    Object Oriented Design                   //
/////////////////////////////////////////////////////////////////////

#include <iostream>
using namespace std;

//  third_party class declaration
class third_party
{

private:
    std::string _name;

public:
    third_party()
    {
        cout << "Calling third_party Default Constructor" << endl;
    }
    third_party(std::string name) : _name(name)
    {
        cout << "Calling third_party Parametrized Constructor" << endl;
    }
    ~third_party()
    {
        cout << "Calling third_party Default destructor" << endl;
    }
};

//  Base class declaration
class Base
{
    // if use create pointer of the other  class like third_party
    // in class base class then while construction the Base class
    // Base class constructor not calling third_party constructor
    // third_party *t;

    // if use create object of the other class like third_party
    // in class base class then while construction the Base class
    // Base class constructor calling third_party constructor
    third_party t;

public:
    Base()
    {
        cout << "Calling Base Default Constructor" << endl;
    }
    Base(std::string name) : _name(name)
    {
        cout << "Calling Base Parametrized Constructor" << endl;
    }

    void show()
    {
        std::cout << "Base string : " << _name << endl;
    }
    ~Base()
    {
        cout << "Calling Base Destructor" << endl;
    }

private:
    std::string _name;
};

//  Derived class declaration
class Derived : public Base
{
    third_party t;

public:
    Derived()
    {
        cout << "Calling Derived Default Constructor" << endl;
    }
    Derived(std::string name) : Base(name)
    {
        cout << "Calling Derived Parametrized Constructor" << endl;
    }
    ~Derived()
    {
        cout << "Calling Derived Destructor" << endl;
    }
    void show()
    {
        Base::show();
    }
};

class A
{
public:
    A(int nValue)
    {
        cout << "A: " << nValue << endl;
    }
};

class B : public A
{
public:
    B(int nValue, double dValue)
        : A(nValue)
    {
        cout << "B: " << dValue << endl;
    }
};

class C : public B
{
public:
    C(int nValue, double dValue, char chValue)
        : B(nValue, dValue)
    {
        cout << "C: " << chValue << endl;
    }
};

int main()
{

    // if we call the derived object then it will call the base constructor also
    // if we are havind is-a relation with class Base then while construction the Derived Object
    // it will call first the Base class constructor and then it call the Derived class constructor

    Derived d("Jotipriya");
    d.show();

    C cClass(5, 4.3, 'R');

    return 0;
}