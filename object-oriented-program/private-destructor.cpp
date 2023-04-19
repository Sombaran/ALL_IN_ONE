/*
 * private-destructor.cpp
 *
 *  Created on: 17-Mar-2022
 *      Author: Jotirling Swami
 */

//   *** Private Destructor in C++ ***
//
//		1. Destructors with the access modifier as private are known as Private Destructors.
//		2. Whenever we want to prevent the destruction of an object, we can make the destructor private.
//
//	What is the use of private destructor?
//
//		1. Whenever we want to control the destruction of objects of a class, we make the destructor private.
//		2. For dynamically created objects, it may happen that you pass a pointer to the object to a function and
//			the function deletes the object. If the object is referred after the function call, the reference will become dangling.
//
//	Private Destructor :
//		1. The program compiles and runs fine. Hence, we can say that: It is not a compiler error to create private destructors.
#include <iostream>
using namespace std;

#ifndef NULL
class Test
{
private:
	~Test()
	{
	}
};
int main()
{
	return 0; // compile success
}
#endif

// 		2. What do you say about the below program?
//			The program fails in the compilation. The compiler notices that the local variable is cannot be destructed
//			because the destructor is private.

#include <iostream>
using namespace std;

#ifndef NULL
class Test
{
private:
	~Test()
	{
	}
};
int main()
{
	Test t; // error: 'Test::~Test()' is private within this context
	return 0;
}
#endif

// 		3.There is no object being constructed, the program just creates a pointer of type Test *t, so nothing is destructed.

#include <iostream>
using namespace std;

#ifndef NULL
class Test
{
private:
	~Test()
	{
	}
};
int main()
{
	cout << "start" << endl;
	Test *t; // compile success
	cout << "end" << endl;
}
#endif

//		4. The program also works fine. When something is created using dynamic memory allocation,
//		it is the programmers responsibility to delete it. So compiler doesn't bother.

#include <iostream>
using namespace std;

#ifndef NULL
class Test
{
private:
	~Test()
	{
	}
};
int main()
{
	cout << "start" << endl;
	Test *t = new Test; // compile success
	cout << "end" << endl;
}
#endif

//		5. In the case where the destructor is declared private, an instance of the class can also be
//		created using the malloc() function.

#include <iostream>
using namespace std;

#ifndef NULL
class Test
{
public:
	Test() // Constructor
	{
		cout << "Constructor called\n";
	}

private:
	~Test() // Private Destructor
	{
		cout << "Destructor called\n";
	}
};

int main()
{
	cout << "start" << endl;
	Test *t = (Test *)malloc(sizeof(Test)); // compile success
	cout << "end" << endl;
	return 0;
}
#endif

//		6. When class has a private destructor, The dynamic objects of that class can be created

#include <iostream>
using namespace std;

#ifndef NULL
class Test
{
private:
	~Test()
	{
	}
};

// Driver Code
int main()
{
	cout << "start" << endl;
	Test *t = new Test;
	//	delete t; // error: 'Test::~Test()' is private within this context
	cout << "end" << endl;
}
#endif

//		7. Following is a way to create classes with private destructors and have a function as a friend of the class.
//		The function can only delete the objects.

#include <iostream>

#ifndef NULL
class Test
{
private:
	~Test()
	{
	}

public:
	friend void destructTest(Test *);
};

// Only this function can destruct objects of Test
void destructTest(Test *ptr)
{
	delete ptr;
}

int main()
{
	cout << "start" << endl;
	Test *ptr = new Test;
	destructTest(ptr);
	cout << "end" << endl;
	return 0;
}
#endif
