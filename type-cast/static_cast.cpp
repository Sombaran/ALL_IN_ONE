//============================================================================
// Name        : static_cast.cpp
// Author      : Jotirling Swami
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

//  *** static_cast ***
//		This is the simplest type of cast which can be used.
//		It is a compile time cast.
//		It does things like implicit conversions between types (such as int to float, or pointer to void*), and
//		It can also call explicit conversion functions (or implicit ones).

//	 BOTTOM LINE
//	 1. For compatible type conversion, such as float to int.
//	 2. For conversion operator and conversion constructors.
//	 3. To avoid unrelated pointer conversion.
//	 4. Avoids derived to private base pointer conversion.
//	 5. Use for all up-cast but never use for confused down-cast because there are no runtime checks performed
//			for static_cast conversions.
//	 6. Intension are more clear in C++ style cast (express your intent better and make code review easier)
//	 7. Finding is easy.
//	 8. Error found at compile time.

#include <iostream>
using namespace std;

class IntVariable {
	int value;

public:
	IntVariable(int x) :
			value { x } {
		cout << "Conversion Constuctor called" << endl;
	}
	operator string() {
		cout << "Conversion Operator" << endl;
		return to_string(value);
	}
	void print() {
		cout << value << endl;
	}
};

class Base {
};
class Derived: public Base {
};

int main() {
	//-----------------------------------------------------------------------
	float f = 3.5;
	int a = f; // C-Style Cast
	cout << a << endl;
	int b = static_cast<int>(f); // C++ Style Cast
	cout << b << endl;
	//-----------------------------------------------------------------------

	char c = 'A';

	int *q = (int*) &c; 			// works
	cout << *q << endl;
//	int *p = static_cast<int*>(&c); // error
//	cout << *p << endl;

	//-----------------------------------------------------------------------
	IntVariable obj(3); //	When obj is created then constructor is called which in our case is also a Conversion Constructor
	obj.print();

	// OR string str = obj;
	string str(obj); //	When you create str out of obj, compiler will not thrown an error as we have defined the Conversion operator.
	obj = 20; // When you make obj=20, you are actually calling the conversion constructor.
	obj.print();
	string str2 = static_cast<string>(obj); //When you make str2 out of static_cast, it is quite similar to string str=obj;, but with a tight type checking.
	obj = static_cast<IntVariable>(30); // When you write obj=static_cast<Int>(30), you are converting 30 into Int using static_cast.

	//-----------------------------------------------------------------------

	Derived d1;
	Base *b1 = (Base*) (&d1); // allowed
	Base *b2 = static_cast<Base*>(&d1); // Its works because the public inheritance in private it will through error

	int i = 10;
	void *v = static_cast<void*>(&i);
	int *ip = static_cast<int*>(v);
	cout << *ip << endl;
	//-----------------------------------------------------------------------
	return 0;
}

