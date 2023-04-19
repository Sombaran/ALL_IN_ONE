/*
 * virtual-destructor.cpp
 *
 *  Created on: 19-Mar-2022
 *      Author: Jotirling Swami
 */

//   *** Virtual Destructor ***
//		1. Deleting a derived class object using a pointer of base class type that has a non-virtual destructor
//				results in undefined behavior.
//		2. To correct this situation, the base class should be defined with a virtual destructor.
//				For example, following program results in undefined behavior.
#include <iostream>
using namespace std;

#ifndef NULL
class base {
public:
	base() {
		cout << "Constructing base\n";
	}
	~base() {
		cout << "Destructing base\n";
	}
};

class derived: public base {
public:
	derived() {
		cout << "Constructing derived\n";
	}
	~derived() {
		cout << "Destructing derived\n";
	}
};

int main() {
	cout << "--------------------------------------" << endl;
	derived *d = new derived();
	cout << "--------------------------------------" << endl;
	base *b = d;
	cout << "--------------------------------------" << endl;
	delete b;
	cout << "--------------------------------------" << endl;
	return 0;
}

//	Output:
//		--------------------------------------
//		Constructing base
//		Constructing derived
//		--------------------------------------
//		--------------------------------------
//		Destructing base
//		--------------------------------------
#endif

// Solution : virtual destructor

#include <iostream>
using namespace std;

#ifndef NULL
class base {
public:
	base() {
		cout << "Constructing base\n";
	}
	virtual ~base() {
		cout << "Destructing base\n";
	}
};

class derived: public base {
public:
	derived() {
		cout << "Constructing derived\n";
	}
	~derived() {
		cout << "Destructing derived\n";
	}
};

int main() {
	cout << "--------------------------------------" << endl;
	derived *d = new derived();
	cout << "--------------------------------------" << endl;
	base *b = d;
	cout << "--------------------------------------" << endl;
	delete b;
	cout << "--------------------------------------" << endl;
	return 0;
}

//	Output:
//		--------------------------------------
//		Constructing base
//		Constructing derived
//		--------------------------------------
//		--------------------------------------
//		Destructing derived
//		Destructing base
//		--------------------------------------

#endif
