/*
 * virtual-function.cpp
 *
 *  Created on: 17-Mar-2022
 *      Author: Jotirling Swami
 */

//	 TOPIC: Introduction To Virtual Function
//
//	 NOTES:
//	 1. WHY to use virtual function?
//		a. To achieve dynamic polymorphism. Which is the ability to call Derived class function using
//			Base class pointer or reference.
//
//	 2. HOW to use virtual function?
//		a. By declaring function as virtual in Base class and overriding that function in Derived class.
//		   (Function signature should be same in Base and Derived class)
//		b. Declaring a function as virtual in Base class is enough, Derived class function need not to be declared virtual.
//		c. Virtual functions should be accessed using pointer(*) or reference(&) of Base class type to
//			achieve run time polymorphism.
//
//	 ADITIONAL NOTES:
//	 1. Virtual functions cannot be static and also cannot be a friend function of another class.
//	 2. A class can have virtual destructor but can't have virtual constructor.
//	 3. There are two types of virtual functions in C++.
//		a. Virtual Function
//		b. Pure Virtual Function
//
// 		*** Virtual Function :
//
//			1.	A virtual function is a member function which is declared within a base class and
//				is re-defined (overridden) by a derived class.
//			2. 	When you refer to a derived class object using a pointer or a reference to the base class,
//				you can call a virtual function for that object and execute the derived class's version of the function.
//
//			3. 	Virtual functions ensure that the correct function is called for an object, regardless of the type
//				of reference (or pointer) used for function call.
//
//			4. 	They are mainly used to achieve Runtime polymorphism.
//			5.  Functions are declared with a virtual keyword in base class.
//			6.  The resolving of function call is done at runtime.
//
//		Rules for Virtual Functions
//
//			1. Virtual functions cannot be static.
//			2. A virtual function can be a friend function of another class.
//			3. Virtual functions should be accessed using pointer or reference of base class type to achieve
//				runtime polymorphism.
//			4. The prototype of virtual functions should be the same in the base as well as derived class.
//			5. They are always defined in the base class and overridden in a derived class.
//			6. It is not mandatory for the derived class to override (if it not pure virtual function)
//				(or re-define the virtual function), in that case, the base class version of the function is used.
//			7. A class may have virtual destructor but it cannot have a virtual constructor.
//			8. Compile time (early binding) VS runtime (late binding) behavior of Virtual Functions
//
//		Special Notes:
//				1. If you define the pure virtual function in the base call and you are not overriding into derived class,
//					and you are trying to assign object of that derived (which is not having the base class
//					virtual function overridden into derived class) base class pointer then we will have compile time error.
//						Animal *a = new Cat; // error: invalid new-expression of abstract class type 'Cat'
//				2. If you define the pure virtual function in the base call and you are not overriding into derived class,
//					and you are trying to assign object of that derived class which has overridden the virtual function to base
//					class pointer then we will not have compile time error.
//						Animal *a = new Cow; // no error
//
//
#include <iostream>
using namespace std;

class Animal {
public:
	virtual void eat() = 0;
};

class Cat: public Animal {
public:
	void eat1() {
		cout << "Eating Non-veg" << endl;
	}
};

class Cow: public Animal {
public:
	void eat() {
		cout << "Eating Veg" << endl;
	}
};

//		Working of virtual functions (concept of VTABLE and VPTR)
//				1. if a class contains a virtual function then compiler itself does two things.
//				2. If object of that class is created then a virtual pointer (VPTR) is inserted as
//					a data member of the class to point to VTABLE of that class.
//				3. For each new object created, a new virtual pointer is inserted as a data member of that class.
//				4. Irrespective of object is created or not, class contains as a member a static array
//				   of function pointers called VTABLE.
//				5. Cells of this table store the address of each virtual function contained in that class.

// CPP program to illustrate
// working of Virtual Functions

#include<iostream>
using namespace std;

class base {
public:
	void fun_1() {
		cout << "fun_1: base-1" << endl;
	}
	virtual void fun_2() {
		cout << "fun_2: base-2" << endl;
	}
	virtual void fun_3() {
		cout << "fun_3: base-3" << endl;
	}
	virtual void fun_4() {
		cout << "fun_4: base-4" << endl;
	}
};

class derived: public base {
public:
	void fun_1() {
		cout << "fun_1: derived-1" << endl;
	}
	void fun_2() {
		cout << "fun_2: derived-2" << endl;
	}
	void fun_4(int x) {
		cout << "fun_4: derived-4" << endl;
	}
};

int main() {

//	Animal *a = new Cat; // error because it not overriding the base class virtual function and still program compiles
	Animal *a = new Cow;
	a->eat();

	base *p;
	derived obj1;
	p = &obj1;

	// Early binding because fun1() is non-virtual
	// in base
	p->fun_1(); // Call base class function

	// Late binding (RTP)
	p->fun_2(); // Call Derived class function

	// Late binding (RTP)
	p->fun_3(); // Call base class function because not overriding exact prototype function in derived

	// Late binding (RTP)
	p->fun_4(); // Call base class function because not overriding exact prototype function in derived

	// Early binding but this function call is
	// illegal (produces error) because pointer
	// is of base type and function is of
	// derived class
	//p->fun_4(5); // error: because same prototype of of base class virtual function is not overridden in derived class

	p->fun_4(); // compile success and calls the base class function even though its virtual function
				//	it will call base class function because its not overridden into the derived class.

	return 0;
}
