/*
 * pure-virtual-destructor.cpp
 *
 *  Created on: 17-Mar-2022
 *      Author: Jotirling Swami
 */

//    ***  Pure Virtual Destructor in C++  ***
//
//		When destroying instances of a derived class using a base class pointer object,
//		a virtual destructor is used to free up memory space allocated by the derived class object or instance.
//
//	Note:
//		Only Destructors can be Virtual. Constructors cannot be declared as virtual.
//
//	Pure Virtual Destructor
//
//		1. A pure virtual destructor can be declared. After the destructor has been created as a pure virtual object,
//		2. the destructor body must be provided.
//		3. This is due to the fact that destructors will not be overridden in derived classes, but will instead be called in reverse order.
//		4. As a result, for a pure virtual destructor, you must specify a destructor body.
//
//	Can a destructor be pure virtual in C++?
//
//			Yes, it is possible to have a pure virtual destructor.
//			Pure virtual destructors are legal in standard C++ and one of the most important things to remember is that
//			if a class contains a pure virtual destructor, it must provide a function body for the pure virtual destructor.
//
//	Why a pure virtual function requires a function body?
//
//		The reason is that destructors (unlike other functions) are not actually 'overridden',
//			rather they are always called in the reverse order of the class derivation.
//		This means that a derived class' destructor will be invoked first, then base class destructor will be called.
//		If the definition of the pure virtual destructor is not provided,
//			then what function body will be called during object destruction?
//		Therefore the compiler and linker enforce the existence of a function body for pure virtual destructors.
// CPP Program to demonstrate pure virtual destructor\
//
#include <iostream>
using namespace std;

#ifndef NULL
class Base {
public:
	virtual ~Base() = 0;
	// Pure virtual destructor
};

class Derived: public Base {
public:
	~Derived() { // error : undefined reference to `Base::~Base()'
		cout << "~Derived() is executed";
	} //
};

// Driver Code
int main() {
	Base *b = new Derived();
	delete b;
	return 0;
}
#endif

//	If you provide the definition for pure virtual destructor then it will compile properlly

#ifndef NULL
class Base {
public:
	virtual ~Base() = 0;
	// Pure virtual destructor
};

Base::~Base() {
	cout << "~Base() Destructor executed" << endl;
}

class Derived: public Base {
public:
	~Derived() {
		cout << "~Derived() Destructor executed" << endl;
	} //
};

// Driver Code
int main() {
	Base *b = new Derived();
	delete b;
	return 0;
}
#endif

//
//	Note :
//		It is important to note that a class becomes an abstract class when it contains a pure virtual destructor.
//		We can create pointer to Test class but we can not create any object of the class.

#include <iostream>
#ifndef NULL
class Test {
public:
	virtual ~Test() = 0;
	// Test now becomes abstract class
};
Test::~Test() {
	cout << "~Test() Destructor called" << endl;
}

// Driver Code
int main() {
	Test p; //error: cannot declare variable 'p' to be of abstract type 'Test'
	Test *t1 = new Test; //error: invalid new-expression of abstract class type 'Test'
	return 0;
}
#endif
