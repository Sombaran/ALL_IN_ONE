//============================================================================
// Name        : auto_ptr.cpp
// Author      : Jotirling Swami
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

//	*** auto_ptr ***

//		This class template is deprecated as of C++11.
//		unique_ptr is a new facility with a similar functionality, but with improved security.
//		auto_ptr is a smart pointer that manages an object obtained via new expression and
//		deletes that object when auto_ptr itself is destroyed.
//		An object when described using auto_ptr class it stores a pointer to a single allocated object
//		which ensures that when it goes out of scope, the object it points to must get automatically destroyed.
//		It is based on exclusive ownership model
//		i.e. two pointers of the same type can’t point to the same resource at the same time.
//
//			As shown in the below program, copying or assigning of pointers changes the ownership
//		i.e. source pointer has to give ownership to the destination pointer.

// C++ program to illustrate the use of auto_ptr
#include <iostream>
#include <memory>
using namespace std;

class Base {
public:
	void show() {
		cout << "A::show()" << endl;
	}
};

int main() {
	cout
			<< "auto_ptr is deprecated from the c++11 \nUse unique_cast instead of auto_ptr"
			<< endl;

//	// p1 is an auto_ptr of type A
//	auto_ptr<Base> p1(new Base);
//	p1->show();
//
//	// returns the memory address of p1
//	cout << p1.get() << endl;
//
//	// copy constructor called, this makes p1 empty.
//	auto_ptr<Base> p2(p1);
//	p2->show();
//
//	// p1 is empty now
//	cout << p1.get() << endl;
//
//	// p1 gets copied in p2
//	cout << p2.get() << endl;

	return 0;
}
