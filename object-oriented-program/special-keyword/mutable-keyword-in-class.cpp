/*
 * mutable-keyword in class.cpp
 *
 *  Created on: 14-Mar-2022
 *      Author: Jotirling Swami
 */

//	Mutable-keyword in class
//		1. The mutable storage class specifier in C++ (or use of mutable keyword in C++)
//		2. auto, register, static and extern are the storage class specifiers in C.
//		3. typedef is also considered as a storage class specifier in C. C++ also supports all these storage class specifiers.
//		4. In addition to this C++, adds one important storage class specifier whose name is mutable.
//
//	What is the need of mutable?
//		1. We can change the class data member via const class object of class through the const member function.
//		2. By using mutable keyword in front of the variable in class we can update/change the value of variable in const member function.
// 		3. You cannot use the mutable specifier with names declared as static or const, or reference.

#include <iostream>

class test{
	mutable int bill;
	std::string name;
	public:
	test(std::string n, int b)
	: bill (b)
	, name (n) { 
		std::cout << __func__ << std::endl;
	}
	void changeName(std::string p) {
		this->name = p;
		}
		
	void changeBill (int q) const {
		this->bill = q;
	}
	
	void display() const {
		std::cout << "bill " << bill << std::endl << "name " << name << std::endl;
	}
};

int main() {
    
    test  t1("Bur", 223072904);
    //t1.changeName ("Gandu");
    //t1.changeBill(22);
    t1.display();
    test const t2("Ur",13564200);
    t2.changeBill(223072904);
    t2.display();
    return (0);
}