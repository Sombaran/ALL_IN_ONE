/*
 * difference-pointer-reference.cpp
 *
 *  Created on: 19-Mar-2022
 *      Author: Jotirling Swami
 */

// 1. Memory Address
//		The pointer variable returns the value whose address it is pointing to.
//		Value can be retrieved using the (*) operator.
//		The pointer variable in C++ has its own address in computer memory,
//		and it also occupies space in the stack.
//		The reference variable returns the address of the address it is referring to.
//		The address can be retrieved using the (&) operator.
//		The reference variable does not have its own memory address; instead,
//		it only points to the variable and shares the same address as the original variable.

// 	2. Declaring
//		In the case of declaring a pointer in a C++ program, (*) operator is used before the pointer name.
//		For example:
//		int *ptr;
//
//		The reference variable is declared by using the (&) operator before the reference variable,
//		For example:
//		Int a= 10;
//		int &ref = a;

// 	3. Arithmetic operations
//		All the arithmetic operations like addition, subtraction, increment, etc.,
//		are possible in the case of pointers in C++. This is known as Pointer arithmetic.
//
//		Arithmetic operations are not possible in the case of references. In C++,
//		it will throw a compiler time error when it tries to do so.

// 	4. Reassignment Is Not Possible With Reference
//		A pointer can be reassigned to point to another variable.
//			But the variable needs to be of the same type of variable.
//		For example:
//		int *p;
//		Int x, y;
//		p = &x;
//		p = &y;
//		Once the variable is referred to by the reference variable, it cannot be reassigned to refer to another variable.
//		Int x, y;
//		int &p;
//		p = &x;
//		p = &y;

// 	5. Indirection
//		Pointers allow multiple levels of indirection, which means that pointer to pointer to pointer assigning
//		and targeting is possible.
//
//		No multiple levels of indirection are possible in the case of references.
//		Only a single level is applicable in references. Implementing multiple levels in references in C++
//		throws a compiler error to the user.

// 	6. NULL value
//		Pointers in C++ can be assigned to NULL values.
//
//		References in C++ can never be NULL else; it will throw an exception.

#include <iostream>
using namespace std;

int main()
{

	{
		int a = 10;
		int b = 120;
		int &ref = a;
		ref = 10;

		cout << ref << endl;
	}
	{
		int i = 10;
		int &r = i;	 // reference
		int *p = &i; // pointer
		cout << "address of variable: " << '\t' << &i << endl;
		cout << "address of reference: " << '\t' << &r << endl;
		cout << "address of pointer: " << '\t' << &p << endl;
		cout << "address of pointer pointing to: " << '\t' << &(*p) << endl;
	}
	{
		int x = 10, y = 20;
		int &p = x;
		int &p1 = y;
		p = p1;
		p = y;
		cout << "p: " << p << " p1: " << p1 << endl;
	}
}
