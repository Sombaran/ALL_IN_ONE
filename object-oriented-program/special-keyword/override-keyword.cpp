/*
 * override-keyword.cpp
 *
 *  Created on: 15-Mar-2022
 *      Author: Jotirling Swami
 */

//	What is override keyword in C++?
//		Answer 1: Testing become easy with this.. (easy maintenance)
//		Answer 2: Compile time check can be performed.. (future error could be reduced)

// Note:
// 		1. We can not override the non-virtual function Derived class from Base class.
// 		2. The name override in the method declaration expresses that the it should be virtual method of a base class.

#include <iostream>
class Base
{
	int b_var;

public:
	virtual void fun()
	{
		std::cout << "Base fun" << std::endl;
	}
};

class Derived : public Base
{
	int d_var;

public:
	void fun() override
	{
		std::cout << "Derived fun" << std::endl;
	}
};

int main()
{
	Base *b = new Derived();
	b->fun();
	return 0;
}
