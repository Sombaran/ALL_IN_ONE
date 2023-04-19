/*
 * final-keyword.cpp
 *
 *  Created on: 15-Mar-2022
 *      Author: Jotirling Swami
 */

// 		*** final keyword in C++ ***
//		1.	final keyword specifies that a virtual function cannot be overridden in a derived class.
//		2.	It also specifies that a class cannot be inherited from.
//		3.  It ensures that the function is virtual, otherwise a compile-time error is generated.
//		4.  It ensures that class cannot be used as base class by any other class.
//
// 	 Use of final specifier in C++
//		1. define function as final
//		2. define class as final

// 1. define function as final
//		a. Sometimes you don't want to allow derived class to override the base class' virtual function.
//		b. C++ 11 allows built-in facility to prevent overriding of virtual function using final specifier.


#include <iostream>
using namespace std;

class Base {
public:
	virtual void myfun() final {
		cout << "myfun() in Base";
	}
};
class Derived: public Base {
public:
	//error: virtual function 'virtual void Derived::myfun()' overriding final function
	void myfun() {
		cout << "myfun() in Derived\n";
	}
};

//	2. define class as final
//		a. final specifier in C++ 11 can also be used to prevent inheritance of class / struct.
//		b. If a class or struct is marked as final then it becomes non inheritable and
//		c. It cannot be used as base class/struct.

class Base_ final {
public:
	virtual void foo();
};

//error: cannot derive from 'final' class 'Base_' in derived type 'Derived_1'
class Derived_1: public Base_ {
public:
	void foo();
};

int main() {
	Derived d;
	Base &b = d;
	b.myfun();
	return 0;
}
