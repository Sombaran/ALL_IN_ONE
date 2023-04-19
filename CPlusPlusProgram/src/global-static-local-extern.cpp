/*
 * global-static-local-extern.cpp
 *
 *  Created on: 17-Mar-2022
 *      Author: Jotirling Swami
 */

//	Local Variables
//		A local variable is one that occurs within a specific scope. They exist only in the function where they are created.
//		They are sometimes called automatic variables because they are automatically created when the function starts execution,
//		and automatically go away when the function is finished executing.
//		The keyword auto can be used to explicitly create these variables, but isn't necessary since auto is the default.
//		A variable defined inside a function (defined inside function body between braces) is called a local variable or automatic variable.
//		Its scope is only limited to the function where it is defined. In simple terms, local variable exists and
//		can be accessed only inside a function.
//		The life of a local variable ends (It is destroyed) when the function exits.

//	Global Variables and extern
//		A global variable is a variable that is defined outside all functions and available to all functions.
//		These variables are unaffected by scopes and are always available, which means that a global variable exists until the program ends.
//		It is possible to create a global variable in one file and access it from another file. In order to do this,
//		the variable must be declared in both files, but the keyword extern must precede the "second" declaration.
//		If a variable is defined outside all functions, then it is called a global variable.
//		The scope of a global variable is the whole program. This means, It can be used and changed at any part of the program
//		after its declaration.
//		Likewise, its life ends only when the program ends.

//	Static Variables
//		A static variable can be either a global or local variable. Both are created by preceding the variable
//		declaration with the keyword static.
//		A local static variable is a variable that can maintain its value from one function call to another and
//		it will exist until the program ends.
//		When a local static variable is created, it should be assigned an initial value. If it's not, the value will default to 0.
//		A global static variable is one that can only be accessed in the file where it is created.
//		This variable is said to have file scope.
//		A static local variable exists only inside a function where it is declared (similar to a local variable)
//		but its lifetime starts when the function is called and ends only when the program ends.
//		The main difference between local variable and static variable is that, the value of static variable persists
//		the end of the program.
//
//	Constant Variables
//		In C, the preprocessor directive #define was used to create a variable with a constant value.
//		This still works in C++, but problems could arise.
//		When #define is used, the preprocessor will go through the code and replace every instance of the
//		#defined variable with the appropriate value. Well, since the #defined variable exists only in the file
//		where it is created, it is possible to have the same definition in another file with a completely different value.
//		This could lead to disastrous consequences.
//		To overcome this problem, the concept of a named constant that is just like a variable was introduced to C++.
//		To create a constant variable in C++, precede the variable declaration with the keyword const.
//		This tells the compiler that "a variable has been created that has a value that cannot be changed"
//		When creating a constant variable, it MUST be assigned a value.


#include <iostream>

#ifndef NULL
int global; /* Uninitialized variable stored in bss*/
int global = 10; /* initialized global variable stored in DS*/
int main(void) {
	int global; /* Uninitialized variable stored in bss*/
	static int i = 100; /* Initialized static variable stored in DS*/
	return 0;
}
#endif




//	*** Static Variable : ***

//	1. A static int variable remains in memory while the program is running.
//		A normal or auto variable is destroyed when a function call where the variable was declared is over.
//
//	For example, we can use static int to count a number of times a function is called,
//		but an auto variable can't be used for this purpose.

#include<iostream>
#ifndef NULL
int fun()
{
  static int count = 0;
  count++;
  return count;
}

int main()
{
  printf("%d ", fun());
  printf("%d ", fun());
  return 0;
}
//	Output:
//		1 2
#endif
