/*
 * make-final-without-final-keyword.cpp
 *
 *  Created on: 15-Mar-2022
 *      Author: Jotirling Swami
 */

//	How To Make A Class Non Inheritable Without Using final Keyword In C++
//	REQUIRMENT:
//		1. We need one class which will make our class as final class.
//		   Lets call that class Final class.
//
//	SOLUTION:
//		a. Make default constructor of Final class as private.
//		b. Inherite Final class as virtual in our class which we want to make non'inheritable.
//		c. Make our class as friend inside Final class.
//			(so that only our class can call the constructor of Final class, not the derived class)
//	Point a - 	By making the Final class constructor private we will avoid
//				Base class calling the Final class constructor.
//	Point b -   Due to virtual inheritance : we can avoid the derived class calling Final class via the Base class
//				constructor. so Derived class only call the Final class Constructor but
//				its in private so it wont allow
//				What happens in normal inheritance : Derived class call the Base class constructor and base class call the
//				Final class constuctor.
//	point c -   By making base class as friend inside the Final class we can still create object of the base class.
#include <iostream>
using namespace std;

class Final {
private:
	Final() {
	}
	friend class Base;
};

class Base: virtual public Final {
public:
	Base() {
	}
};

class Derived: public Base {
public:
	//error: 'Final::Final()' is private within this context
	Derived() {
	}
};

int main() {
	Derived d;
	Base b;

	return 0;
}

