/*
 * call-virtual-function-in-constructor-destructor.cpp
 *
 *  Created on: 17-Mar-2022
 *      Author: Jotirling Swami
 */

//  *** Calling virtual methods in constructor/destructor  ***
//		1. Calling virtual functions from a constructor or destructor is considered dangerous most of the times
//		2. We must be avoided whenever possible.
//		3. All the C++ implementations need to call the version of the function defined at the level of the
//			hierarchy in the current constructor and not further.
//		4. You can call a virtual function in a constructor. The Objects are constructed from the base up, 'base before derived'.
//
// 	 Example : CPP program to illustrate calling virtual methods in constructor/destructor
//
//		1. Base method in Base class is invoked in constructor as well as destructor.
//		2. When the object of Derived is created, constructor of Base and then constructor of Derived is called
//			and reverse order of calling for destructor.
//		3. Although, Base print method is virtual method but when it is called inside base constructor it will behave as 			non-virtual method because by the time constructor of Base(base) class is called as in above code,
//			Derived(derived) class is not constructed by that time.
//		4. Therefore, it is dangerous to call the member function of class whose object is not constructed yet
//			and compiler calls the Base class version of Base method.
//		5. Same is with the destructor, when object 'd' of Derived gets destroyed, destructor of Derived class is called first
//			and then destructor for Base class is called but by this time Derived is already destroyed,
//			hence Base class version of print Base is called.
//
#include <iostream>
using namespace std;
//#ifndef NULL
class Base
{
public:
	static int value;
	Base()
	{
		cout << "Base class Constructor called" << endl;
		print();
	}

	~Base()
	{
		cout << "Base class Destructor called" << endl;
		print();
	}

	virtual void print()
	{
		cout << "Base class Virtual method print called" << endl;
	}

	void display()
	{
		cout << "Base class display called" << endl;
		print();
	}
};

class Derived : public Base
{
public:
	Derived()
	{
		cout << "Derived class Constructor called" << endl;
	}
	void print()
	{
		cout << "Derived class Virtual print method called" << endl;
	}
	~Derived()
	{
		cout << "Derived class Destructor called" << endl;
	}
};

int fun()
{
	cout << "calling fun" << endl;
	return 10;
}

int Base::value = fun();

int main()
{
	cout << "main start" << endl;
	{
		Derived d;
		d.display();
	}
	cout << "main end" << endl;
}

// Output :
//		main start
//		Base class Constructor called
//		Base class Virtual method print called
//		Derived class Constructor called
//		Base class display called
//		Derived class Virtual print method called
//		Derived class Destructor called
//		Base class Destructor called
//		Base class Virtual method print called
//		main end

//#endif
