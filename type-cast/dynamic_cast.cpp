//============================================================================
// Name        : dynamic_cast.cpp
// Author      : Jotirling Swami
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

//	*** dynamic_cast ***
// 1. dynamic_cast is used at run time to find out correct down-cast.
//	 Notes :
//		0. Its run time type cast
//		1. Need at least one virtual function in base class.
//		2. If the cast is successful, dynamic_cast returns a value of type new_type.
//		3. If the cast fails and new_type is a pointer type, it returns a null pointer of that type.
//		4. If the cast fails and new_type is a reference type, it throws an exception that matches a handler of type std::bad_cast
//	 BOTTOM LINE:
//		1. work only on polymorphic base class (at least one virtual function in base class)
//				because it uses this information to decide about wrong down-cast.
//		2. it is used for up-cast (D->B) and downscast (B->D), but it is mainly used for correct downcast.
//		3. using this cast has run time overhead, because it checks object types at run time using RTTI (Run Time Type Information).
//		4. if we are sure that we will never cast to wrong object then we should always avoid this cast and use static_cast.
//	RTTI [Run Time Type Information]
//		  In C++ the RTTI is a mechanism, that exposes information about an object’s datatype during runtime.
//		  This feature can be available only when the class has at least one virtual function.
//		  It allows the type of an object to be determined when the program is executing.
//	Runtime Casts :
//
//		The runtime cast, which checks that the cast is valid, is the simplest approach to ascertain the runtime type
//		of an object using a pointer or reference. This is especially beneficial
//		when we need to cast a pointer from a base class to a derived type.
//		When dealing with the inheritance hierarchy of classes, the casting of an object is usually required.
//
//	There are two types of casting:
//
//	Upcasting:
//			When a pointer or a reference of a derived class object is treated as a base class pointer.
//	Downcasting:
//			When a base class pointer or reference is converted to a derived class pointer.
//
//	Using dynamic_cast:
//			In an inheritance hierarchy, it is used for downcasting of a base class pointer to a child class.
//			If casting successful, it returns a pointer of the converted type and,
//			It casting fails, it return NULL pointer in case of pointer and std::bad_cast in case of reference.
#include <iostream>
using namespace std;

class Base {
public:
	virtual void print() {
		cout << "Base" << endl;
	}
	virtual ~Base() {
		cout << "~Base" << endl;
	}
};

class Derived1: public Base {
public:
	void print() {
		cout << "Derived1" << endl;
	}
};

class Derived2: public Base {
public:
	void print() {
		cout << "Derived1" << endl;
	}
};

int main() {
	//-----------------------------------------------------------------------
	// In Case Of Pointer

	Base *B = dynamic_cast<Base*>(new Derived1());

	Derived1 *D1 = dynamic_cast<Derived1*>(B);

	if (D1 == NULL)
		cout << "Null" << endl;
	else
		cout << "Not null" << endl;

	Derived2 *D2 = dynamic_cast<Derived2*>(B);

	if (D2 == NULL)
		cout << "Null" << endl;
	else
		cout << "Not Null" << endl;
	//-----------------------------------------------------------------------
	// In Case Of Reference

	Derived1 d1;
	Base &b1 = dynamic_cast<Base&>(d1);
	b1.print();

	try {
		Derived2 &d = dynamic_cast<Derived2&>(b1);
		d.print();
	} catch (std::exception &e) {
		cout << e.what() << endl; // working
	}
	//-----------------------------------------------------------------------
	return 0;
}

