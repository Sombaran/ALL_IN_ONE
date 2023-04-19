//============================================================================
// Name        : const_cast.cpp
// Author      : Jotirling Swami
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

// 		*** const_cast ***
//		1. const_cast is used to cast away the constness of variables.
//		Following are some interesting facts about const_cast.
//
//		(a) const_cast can be used to change non-const class members inside a const member function.
//		(b) const_cast can be used to pass const data to a function that doesnt receive const.
//		(c) It is undefined behavior to modify a value which is initially declared as const.
//		 Notes:
//
//		 The expression constmcast<T>(v) can be used to change the const or
//		 volatile qualifiers of pointers or references.
//		 Where T must be a pointer, reference, or pointer-to-member type.
//		 BOTTOM LINE:
//
//		 NEVER USE THIS!!!
//		 1 Use only when you have to.
//		 2 Use only When the actual refered object/variable is not const.
//		 3 Use when we are dealing with 3'rd party library and some API want data in non~const form but we have it in const.
//		 (actally we can not do anything in that case, but make sure that API is not changing our variable value)


#include <iostream>
using namespace std;

class student {
private:
	int roll;
public:
	student(int r) :
			roll { r } {
	}

	// A const function that changes roll with the help of const_cast
	void fun(int value) const {

		const_cast<student*>(this)->roll = value;
	}

	int getRoll() {
		return roll;
	}
};

int fun(int *ptr) {
	return (*ptr + 10);
}

int main(void) {

//	(a) const_cast can be used to change non-const class members inside a const member function.
	student s(3);
	cout << "Old roll number: " << s.getRoll() << endl;
	s.fun(100);
	cout << "New roll number: " << s.getRoll() << endl;
	//-----------------------------------------------------------------------

//	(b) const_cast can be used to pass const data to a function that doesnt receive const.

	const int val = 10;
	const int *ptr = &val;
	int ival = fun(const_cast<int*>(ptr));
	cout << "value: " << ival << endl;
	//-----------------------------------------------------------------------

//	(c) It is undefined behavior to modify a value which is initially declared as const.

	const int value = 10;
	const int *ptr1 = &value;
	int *ptr2 = const_cast<int*>(ptr1);
	fun(ptr2);
	cout << "value1: " << value << endl;
	//-----------------------------------------------------------------------

//	(d) It is defined behavior to modify a value which is initially declared as non const.

	int value1 = 10;
	const int *ptr3 = &value1;
	value1 = fun(const_cast<int*>(ptr3));
	cout << "value2: " << value1 << endl;
	return 0;
}
