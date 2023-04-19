/*
 * initializer-list.cpp
 *
 *  Created on: 17-Mar-2022
 *      Author: Jotirling Swami
 */

// 	What is Initializer List in C++
//		1. Initializing NON STATIC CONST data member of the class
//			a. Narrowing conversion whiling assigning the value to the class data member in parametrised constructor
//		2. Initializing reference data member of the  class
//		3. To initilize Base class data member by passing base class object to the derived class without defining default
//		4. To initilize Base class data member by passing vairable the derived class without defining default
//		5. Can not assign the same variable name of base and temp variable in parametrize constructor
//		1. Initializing NON STATIC CONST data member of the class.
//
//
#include <iostream>
using namespace std;

#ifndef NULL
class Base {
	int x;

public:
	Base(int a) {
		x = a;
	}  // Parametrized constructor value assignment to class data member
	Base(int a) : x(a);// Parametrized constructor value assignment to class data member
};
int main() {
	Base b(10);
	return 0;
}
#endif

// 2. Initializing reference data member of the  class
#include <iostream>
using namespace std;

#ifndef NULL
class Base {
	int x;

public:
	Base(int a) :
			x { a } {
	} // Initializer list
};
int main() {
	Base b(10);
	return 0;
}
#endif

// 2. initializing NON STATIC CONST data member of the class
#include <iostream>
using namespace std;

#ifndef NULL
class Base {
	const int _x;

public:Base(int x) : {_x = x} {} // error in assignment of value
	Base(int x) :
			_x { x } {
	}    // success
	void print() {
		cout << _x << endl;
	}
};
int main() {
	Base b(10);
	b.print();
	Base b1(12);
	b1.print();
	return 0;
}
#endif

// 2. initializing NON STATIC CONST data member of the class
#include <iostream>
using namespace std;

#ifndef NULL
class Base {
	int &_x;

public:Base(int x) : {_x = x} {} // error in assignment of value
	Base(int x) :
			_x { x } {
	}    // success
	void print() {
		cout << _x << endl;
	}
};
int main() {
	Base b(10);
	b.print();
	Base b1(12);
	b1.print();
	return 0;
}
#endif

// 3. To initilize Base class data member by passing base class object to the derived class without defining default

// Without initilizer list - need to define the default constructor in Base class
#include <iostream>
using namespace std;

#ifndef NULL
class One {
	int _x;

public:
	One() {
	}
	One(int x) :
			_x { x } {
	}
};
class Two {
	One a;

public:
	Two(One x) {
		a = x;
	}
};
int main() {
	One one(10);
	Two two(one);
	return 0;
}
#endif

// With initilizer list - no need to define the default constructor in Base class

#include <iostream>
using namespace std;

#ifndef NULL
class One {
	int _x;

public:
	One(int x) :
			_x { x } {
	}
};
class Two {
	One a;

public:
	Two(One x) :
			a { x } {
	}
};
int main() {
	One one(10);
	Two two(one);
	return 0;
}
#endif

// 4. To initilize Base class data member by passing vairable the derived class without defining default

#include <iostream>
using namespace std;

#ifndef NULL
class One {
	int _x;

public:
	One(int x) :
			_x { x } {
	}
};
class Two {
	One a;

public:
	Two(One x) :
			a { x } {
	}  // Initializer list
	Two(One x) {
		a = x;
	} // error so comment and check for solutuion
};
int main() {
	Two two(10);
	return 0;
}
#endif

// 5. Can not assign the same variable name of base and temp variable in parametrize constructor

#include <iostream>
using namespace std;

#ifndef NULL
class Base {
	int x;

public:
	// Base(int x) : x{x} {} // x = 10 // Using Initializer list
	Base(int x) {
		x = x;
	} // x =32765
	void print() {
		std::cout << x << std::endl;
	}
};
int main() {
	Base b(10);
	b.print();
	return 0;
}
#endif

// 6. Initilizer list helps to optimise the constructor call

#include <iostream>
using namespace std;

#ifndef NULL
class Base {
	int _x;

public:
	Base() {
		cout << "base default" << endl;
	}
	Base(int x) {
		_x = x;
		cout << "base parameter constructor" << endl;
	}
	Base(const Base &obj) {
		this->_x = obj._x;
		cout << "base copy constructor" << endl;
	}
	Base& operator =(const Base &obj) {
		cout << "base assignment operator" << endl;
	}
};
class Myclass {
	Base _b;

public:
	Myclass() {
		cout << "myclass default" << endl;
	}
	Myclass(Base b) {
		_b = b;
		cout << "myclass parameter" << endl;
	}
};
int main() {
	Base b(10);
	Myclass mc(b);
	return 0;
}

// Output:
//     base parameter constructor
//     base copy constructor
//     base default
//     base assignment operator
//     myclass parameter
#endif
