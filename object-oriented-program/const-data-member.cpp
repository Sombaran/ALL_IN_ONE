/*
 * const-data-member.cpp
 *
 *  Created on: 14-Mar-2022
 *      Author: Jotirling Swami
 */

//	constant data member of the class :

//		1. The constant data member is initialized using the const keyword before the data type inside the class.
//		2. Data members are like the variable that is declared inside a class.
//		3. But once the data member is initialized, it never changes, not even in the constructor or destructor.
//		4. The const data members must be assigned the values during its declaration.
//		5. We can assign values to const data member in the constructor or initialize list only.
//		6. If all const data member is not initialized in class declaration. Then in Initializer list we can not initialize few const data member.
//		7. Then in default constructor we can not initialize few const data member. We should initialize all the const data memeber.



#include<iostream>
using namespace std;
class Fraction {
private:

	const int numerator = 10;
	const int denominator = 20;

public:
	Fraction() {
	}
	Fraction(int n, int d) :
			numerator { n }, denominator { d } {
	}

	int getNumerator() const {
		return numerator;
	}

	int getDenominator() const {
		return denominator;
	}

	void print() const {
		cout << this->numerator << " / " << this->denominator << endl;
	}

};

int main() {
	Fraction f1;
	f1.print();

	return 0;
}

