/*
 * diff-pure-virtual-normal-virtual-function.cpp
 *
 *  Created on: 17-Mar-2022
 *      Author: Jotirling Swami
 */

//
//	Virtual Function in C++:
//		1. A virtual function is a member function which is declared within a base class
//		2. Is re-defined(Overriden) by a derived class.
//		3. When you refer to a derived class object using a pointer or a reference to the base class,
//		4. you can call a virtual function for that object and execute the derived class's version of the function.
//
//	Pure Virtual Functions in C++:
//		1. A pure virtual function (or abstract function) in C++ is a virtual function
//		2. It dosen't contant an implementation, we only declare it.
//		3. A pure virtual function is declared by assigning 0 in the declaration.
//
//	Similarities between virtual function and pure virtual function
//
//		These are the concepts of Run-time polymorphism.
//		Prototype i.e. Declaration of both the functions remains the same throughout the program.
//		These functions can't be global or static.
//
//	Difference between virtual function and pure virtual function in C++
//
//		A virtual function is a member function of base class which can be redefined by derived class.
//		A pure virtual function is a member function of base class whose only declaration is provided in base class
//			and should be defined in derived class otherwise derived class also becomes abstract.
//
//		Classes having virtual functions are not abstract.
//		Base class containing pure virtual function becomes abstract.
//
//		Syntax: virtual function
//		virtual<func_type><func_name>()
//		{
//			// code
//		}
//
//		Syntax: pure virtual function
//		virtual<func_type><func_name>() = 0;
//
//		Definition is given in base class.
//		No definition is given in base class.
//
//		Base class having virtual function can be instantiated i.e. its object can be made.
//		Base class having pure virtual function becomes abstract i.e. it cannot be instantiated.
//
//		If derived class do not redefine virtual function of base class, then it does not affect compilation.
//		If derived class do not redefine virtual function of base class, then no compilation error but derived class also becomes abstract just like the base class.
//
//		All derived class may or may not redefine virtual function of base class.
//		All derived class must redefine pure virtual function of base class otherwise derived class also becomes abstract just like base class.






