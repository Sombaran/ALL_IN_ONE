/*
 * shared_ptr_type_casting.cpp
 *
 *  Created on: 11-Mar-2022
 *      Author: Jotirling Swamiriya
 */

#include <iostream>
#include <memory>
using namespace std;

class Base {
public:
	Base() {
		cout << "constructor Base" << endl;
	}
	virtual void function() {
		cout << "Base function called" << endl;
	}
	virtual ~Base() {
		cout << "destructor Base" << endl;
	}
};

class Derived1: public Base {
public:
	void function() override
	{
		cout << "Derived1 function called" << endl;
	}
	~Derived1() {
	}
};

class Derived2: public Base {
public:
	void function() override
	{
		cout << "Derived2 function called" << endl;
	}
	~Derived2() {
	}
};

int main() {
	shared_ptr<Base> basePtr = make_shared<Base>();
	cout << "Base pointer says: ";
	basePtr->function();

	cout << "basePtr use_cout: " << basePtr.use_count() << endl;
	//-----------------------------------------------------------------------

	shared_ptr<Derived1> derivedPtr = make_shared<Derived1>();
	cout << "Derived pointer says: ";
	derivedPtr->function();

	cout << "derivedPtr use_cout: " << derivedPtr.use_count() << endl;
	//-----------------------------------------------------------------------

	// static_pointer_cast to go up class hierarchy
	basePtr = static_pointer_cast<Base>(derivedPtr);
	cout << "Base pointer to derived says: ";
	basePtr->function();

	cout << "basePtr use_cout: " << basePtr.use_count() << endl;
	cout << "derivedPtr use_cout: " << derivedPtr.use_count() << endl;
	//-----------------------------------------------------------------------

	// dynamic_pointer_cast to go down/across class hierarchy
	shared_ptr<Derived1> downcastedPtr = dynamic_pointer_cast<Derived1>(
			basePtr);
	if (downcastedPtr) {
		cout << "Downcasted pointer says: ";
		downcastedPtr->function();
	}

//	 dynamic_pointer_cast check compile time that the casting is happening to same type of pointer
//	shared_ptr<Derived2> downcastedPtr1 = dynamic_pointer_cast<Derived1>(
//			basePtr);
//	if (downcastedPtr1 == NULL) {
//		cout << "downcastedPtr1 NULL";
//	}
	//-----------------------------------------------------------------------
	shared_ptr<Base> sharedBasePtr;

	// if the pointer is been casting to another pointer is NULL then static_cast return NULL Pointer
	shared_ptr<Derived1> downcastedPtr2 = static_pointer_cast<Derived1>(
			sharedBasePtr);
	if (downcastedPtr2) {
		cout << "Downcasted pointer is Not Null" << endl;
	} else {
		cout << "Downcasted pointer is Null" << endl;
	}
	//-----------------------------------------------------------------------
	shared_ptr<Base> sharedBasePtr1;

	// if the pointer is been casting to another pointer is NULL then dynamic_cast return NULL Pointer
	shared_ptr<Derived1> downcastedPtr3 = dynamic_pointer_cast<Derived1>(
			sharedBasePtr1);
	if (downcastedPtr3) {
		cout << "Downcasted pointer is Not Null" << endl;
	} else {
		cout << "Downcasted pointer is Null" << endl;
	}
	//-----------------------------------------------------------------------
}
