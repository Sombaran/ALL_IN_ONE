/////////////////////////////////////////////////////////////////////
//  inheritance.cpp - implementing inheritance relationship        //
//                                                                 //
//  Jotirling Swami  -    Object Oriented Design                   //
/////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>

class Base
{

public:
    Base(const std::string &name);
    virtual ~Base() {}
    virtual void show();

private:
    std::string _name;
};

Base::Base(const std::string &name) : _name(name) {}

void Base::show()
{

    std::cout << "\n  I am " << _name << ", an object of the Base class";
}

class Derived : public Base
{

public:
    Derived(const std::string &name);
    virtual void show();

private:
    std::string _name;
};

Derived::Derived(const std::string &name) : _name(name), Base("Bob") {}

void Derived::show()
{

    std::cout << "\n  I am " << _name << ", derived from base, and use my internal base object.";
    Base::show();
}

int main()
{

    Base Billy("Billy");
    Billy.show();

    std::cout << std::endl;

    Derived Davey("Davey");
    Davey.show();

    std::cout << "\n\n";

    return 0;
}