/*
 * pure-virtual-function.cpp
 *
 *  Created on: 16-Mar-2022
 *      Author: Jotirling Swami
 */

//	 TOPIC: Pure Virtual Function
//	 	 SYNTAX:
//		 	 virtual return_type function_name(parameters...) = 0;
//	 NOTES:
//		 0. If some class can have only signature of function but not the body then we use pure virtual functions.
//		 1. Sometimes in Base class we know the function name but not the definition of the function,
//			so we want Derived classes to provide the definition of the function.
//		 2. We can actually give body of pure virtual function.
//		 3. If you have pure virtual function in your class, it means that class is an Abstract class
//		 4. We can't create an object of that class
//		 5. But we can create pointers and Reference of the abstract class.
//
//	 USE CASE:
//	 	 1. Used to create abstract classes, which helps in creating interfaces (APIs)
//
#include <iostream>
using namespace std;

#ifndef NULL
class Animal {
public:
	virtual void move() = 0;
};

void Animal::move() {
}

class Cow: public Animal {
public:
	void move() {
		Animal::move();
		cout << "i can walk and run" << endl;
	}
};

class Snack: public Animal {
public:
	void move() {
		cout << "i do rectilinear locomotion" << endl;
	}
};

int main() {
	Cow c;
	c.move();

	Animal *C = new Cow;
	C->move();

	Animal *S = new Snack;
	S->move();

	return 0;
}
#endif

//	Some Interesting Facts:
//		1) A class is abstract if it has at least one pure virtual function.
// pure virtual functions make a class abstract
//


#include <iostream>
using namespace std;
#ifndef NULL
class Test {
	int x;
public:
	virtual void show() = 0;
	int getX() {
		return x;
	}
};

int main(void) {
	// pure virtual functions make a class abstract
	Test t; // Compiler Error: cannot declare variable 't' to be of abstract
	 	 	// type 'Test' because the following virtual functions are pure
			// within 'Test': note:     virtual void Test::show()
	return 0;
}
#endif

//	2) We can have pointers and references of abstract class type.
//		For example the following program works fine.

#include<iostream>
using namespace std;
#ifndef NULL
class Base {
public:
	virtual void show() = 0;
};

class Derived: public Base {
public:
	void show() {
		cout << "In Derived \n";
	}
};

int main(void) {
	// In case of pointer
	Base *bp = new Derived();
	bp->show(); //In Derived

	// In case of reference
	Derived d;
	Base &rbp = d;
	rbp.show(); //In Derived

	return 0;
}
#endif

//	3) If we do not override the pure virtual function in derived class,
//		then derived class also becomes abstract class.

#include<iostream>
using namespace std;
#ifndef NULL
class Base {
public:
	virtual void show() = 0;
};

class Derived: public Base {
};

int main(void) {
	Derived d; // Compiler Error: cannot declare variable 'd' to be of abstract type
			   // 'Derived'  because the following virtual functions are pure within
			   //'Derived': virtual void Base::show()
	return 0;
}
#endif

//	4) An abstract class can have constructors.

#include<iostream>
using namespace std;
//#ifndef NULL
// An abstract class with constructor
class Base {
protected:
	int x;
public:
	virtual void fun() = 0;
	Base(int i) {
		x = i;
		cout << "Constructor of base called\n";
	}
};

class Derived: public Base {
	int y;
public:
	Derived(int i, int j) :
			Base(i) {
		y = j;
	}
	void fun() {
		cout << "x = " << x << ", y = " << y << '\n';
	}
};

int main(void) {
	Derived d(4, 5);
	d.fun();

	//object creation using pointer of base class
	Base *ptr = new Derived(6, 7);
	ptr->fun();
	return 0;
}
//#endif

