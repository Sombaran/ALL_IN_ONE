/*
 * explicitly-call-constructor-destructor.cpp
 *
 *  Created on: 17-Mar-2022
 *      Author: Jotirling Swami
 */

//  *** Is it possible to call constructor and destructor explicitly? ***
//		1. Constructor is a special member function that is automatically called by compiler when object is created
//		2. destructor is also special member function that is also implicitly called by compiler when object goes out of scope.
//		3. They are also called when dynamically allocated object is allocated and destroyed,
//		4. new operator allocates storage and calls constructor
//		5. delete operator calls destructor and free the memory allocated by new.
//
//	*** Is it possible to call constructor and destructor explicitly?
//			Yes, it is possible to call special member functions explicitly by programmer.
//
//	Following program calls constructor and destructor explicitly.
//
#include <iostream>
using namespace std;
#ifndef NULL
class Test {
public:
	Test() {
		cout << "Constructor is executed\n";
	}
	~Test() {
		cout << "Destructor is executed\n";
	}
};

int main() {
	Test(); // Explicit call to constructor
	Test t; // local object
	t.~Test(); // Explicit call to destructor
	return 0;
}
//Output :
//		Constructor is executed
//		Destructor is executed
//		Constructor is executed
//		Destructor is executed
//		Destructor is executed

//	Note :
//			When the constructor is called explicitly the compiler creates a nameless temporary object
//			It is immediately destroyed. That's why 2nd line in the output is call to destructor.
#endif

//	1. Explicit call to destructor is only necessary when object is placed at particular location
//		in memory by using placement new.
//	2. Destructor should not be called explicitly when the object is dynamically allocated because
//		delete operator automatically calls destructor.

#include <iostream>
using namespace std;
#ifndef NULL
class Test {
public:
	Test() {
		cout << "Constructor is executed\n";
	}
	~Test() {
		cout << "Destructor is executed\n";
	}
	friend void fun(Test t);
};
void fun(Test t) {
	cout << "fun start" << endl;
	Test();
	t.~Test();
	cout << "fun end" << endl;
}
int main() {
	cout << "main start" << endl;
	Test();
	Test t;
	fun(t);
	cout << "main end" << endl;
	return 0;
}
#endif
