/*
 * const-object-const-function.cpp
 *
 *  Created on: 14-Mar-2022
 *      Author: Jotirling Swami
 */

//	Constant Object :
//
//		1. An object declared as const cannot be modified.
//		2. It can invoke only const member functions as these functions ensure not to modify the object.
//		3. A const object can be created by prefixing the const keyword to the object declaration.
//		4. Any attempt to change the data member of const objects results in a compile-time error.
//		5. Whenever an object is declared as const, it needs to be initialized at the time of declaration.
//		6. The object initialization while declaring is possible only with the help of constructors.
//
//		7. Syntax:
//
//			const Class_Name Object_name;

//	Constant member function :

//		1. The member functions and member function arguments, the objects of a class can also be declared as const.
//		2. When a function is declared as const, it can be called on any type of object, const object as well as non-const objects.
//		3. A function becomes const when the const keyword is used in the function's declaration.
//		4. The idea of const functions is not to allow them to modify the object on which they are called.
//		5. It is recommended the practice to make as many functions const as possible so that accidental changes to objects are avoided.
//		7. constant member function of a class that never changes any class data members, and it also does not call any non-const function.
//		8. It is also known as the read-only function.
//		6. Syntax:
//
//			return_type function_name(arg..) const{}

#include<iostream>
using namespace std;
class Fraction {
private:

	int numerator;
	int denominator;

public:

	Fraction() {
	}

	Fraction(int numerator, int denominator) {
		this->numerator = numerator;
		this->denominator = denominator;
	}

	int getNumerator() const {
		return numerator;
	}

	int getDenominator() const {
		return denominator;
	}

	void setNumerator(int n) {
		numerator = n;
	}

	void setDenominator(int d) {
		denominator = d;
	}

	void print() const {
		cout << this->numerator << " / " << denominator << endl;
	}

};

int main() {
	Fraction f1(100, 50);
	f1.print();
	Fraction f2(150, 40);
	f2.print();
	Fraction const f3;
	cout << f3.getNumerator() << " " << f3.getDenominator();

//	f3.setNumerator(10); // here it will through because we can change the value of const object.
	return 0;
}

