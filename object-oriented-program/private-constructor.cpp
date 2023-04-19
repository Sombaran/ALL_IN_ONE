/*
 * private-constructor.cpp
 *
 *  Created on: 17-Mar-2022
 *      Author: Jotirling Swami
 */

//  *** Can a constructor be private in C++ ? ***
//
//	1. A constructor is a special member function of a class which initializes objects of a class.
//	2. In C++, constructor is automatically called when object of a class is created.
//
//	By default, constructors are defined in public section of class.
//			So, question is can a constructor be defined in private section of class ?
//	Answer : Yes, Constructor can be defined in private section of class
//
//
//		1. Using Friend Class :
//			If we want that class should not be instantiated by anyone else but only by a friend class.
//
#include <iostream>
using namespace std;

#ifndef NULL
// class A
class A
{
private:
	A()
	{
		cout << "constructor of A" << endl;
	}
	friend class B;
};

// class B, friend of class A
class B
{
public:
	B()
	{
		A a1;
		cout << "constructor of B" << endl;
	}
};

// Driver program
int main()
{
	B b1;
	return 0;
}
//	Output :
//		constructor of A
//		constructor of B
#endif

//
//		2. Using Singleton design pattern:
//				When we want to design a singleton class.
//				This means instead of creating several objects of class,
//				The system is driven by a single object.

//		3. Named Constructor Idiom :
//				Since constructor has same name as of class, different constructors are differentiated by their parameter list,
//					but if numbers of constructors is more, then implementation can become error prone.
//				With the Named Constructor Idiom, you declare all the class constructors in the private or protected sections,
//				and then for accessing objects of class, you create public static functions.

// Constructor Idiom :: ambiguous nature of constructor with same no of parameters of same type

#include <iostream>
using namespace std;

#ifndef NULL
class Base
{
public:
	// Rectangular coordinates
	Base(int x, int y);

	// Polar coordinates (radius and angle)
	Base(int r, int a);

	// error: 'Base::Base(int, int)' cannot
	// be overloaded
};
int main()
{
	// Ambiguous: Which constructor to be called ?
	Base p = Base(5.7, 1.2);
	return 0;
}
#endif

//		Solution:
//			This problem can be resolved by Named Constructor Idiom.

#include <iostream>
using namespace std;

#ifndef NULL
class Base
{
private:
	int x1, y1;
	Base(int x, int y)
	{
		x1 = x;
		y1 = y;
	}

public:
	static Base Add(int, int);
	static Base substract(int, int);
	void display();
};

// utility function for displaying of coordinates
void Base::display()
{
	cout << "\tx - " << this->x1 << endl;
	cout << "\ty - " << this->y1 << endl;
}

// return polar coordinates
Base Base::Add(int x, int y)
{
	return Base(x + 10, y + 10);
}

// return rectangular coordinates
Base Base::substract(int x, int y)
{
	return Base(x - 10, y - 10);
}
int main()
{
	Base pp = Base::Add(10, 10);
	cout << "add : " << endl;
	pp.display();

	Base pr = Base::substract(100, 100);
	cout << "substract : " << endl;
	pr.display();
	return 0;
}
#endif
