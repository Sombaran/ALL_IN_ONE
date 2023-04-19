/*
 * virtual-keyword.cpp
 *
 *  Created on: 17-Mar-2022
 *      Author: Jotirling Swami
 */
// 	*** virtual keyword ***
//	Definitions :
//		Without Virtual Functions
//		Using Virtual Functions
//	Definitions:
//		A virtual function is a member function which is declared in the base class using the keyword virtual
//		Is re-defined (Overridden) by the derived class.
//		The term Polymorphism means the ability to take many forms.
//		It occurs if there is a hierarchy of classes which are all related to each other by inheritance.
//
//	Note:
//		In C++ what this means is that if we call a member function then it could cause a different function
//		to be executed instead depending on what type of object invoked it.
//
//	Example:
//		Consider the following simple program as an example of runtime polymorphism.
//		The main thing to note about the program is that the derived class's function is called using a base class pointer.
//		The idea is that virtual functions are called according to the type of the object instance pointed to or referenced,
//			not according to the type of the pointer or reference.
//		In other words, virtual functions are resolved late, at runtime.
//
//
#include <iostream>
using namespace std;

// Base class
class Shape {
public:
	Shape(int l, int w) {
		length = l;
		width = w;
	} // parameterized constructor
	int get_Area() {
		cout << "This is call to parent class area\n";
		return 1;
	}

protected:
	int length, width;
};

// Derived class
class Square: public Shape {
public:
	Square(int l = 0, int w = 0) :
			Shape(l, w) {
	} // declaring and initializing derived class
	  // constructor
	int get_Area() {
		cout << "Square area: " << length * width << '\n';
		return (length * width);
	}
};
// Derived class
class Rectangle: public Shape {
public:
	Rectangle(int l = 0, int w = 0) :
			Shape(l, w) {
	} // declaring and initializing derived class
	  // constructor
	int get_Area() {
		cout << "Rectangle area: " << length * width << '\n';
		return (length * width);
	}
};

int main() {

	Shape *s;
	// Making object of child class Square
	Square sq(5, 5);
	// Making object of child class Rectangle
	Rectangle rec(4, 5);

	s = &sq;
	s->get_Area(); // This is call to parent class area
	s = &rec;
	s->get_Area(); // This is call to parent class area

	return 0;
}

//	Note:
//		By making int get_Area() function as virtual in the base then i will call the respective derived object functions.
