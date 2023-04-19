/*
 * copy-constructor.cpp
 *
 *  Created on: 17-Mar-2022
 *      Author: Jotirling Swami
 */

/*
	When is  copy constructor called? 
1. When an object of the class is returned by value. 
2. When an object of the class is passed (to a function) by value as an argument. 
3. When an object is constructed based on another object of the same class. 
4. When the compiler generates a temporary object.
It is, however, not guaranteed that a copy constructor will be called in all these cases, because the C++ Standard allows the compiler to optimize the copy away in certain cases, one example is return value optimization (sometimes referred to as RVO) 	
*/
#include <iostream>
#include <cstring>
using namespace std;

class String
{
private:
	char *s;

public:
	String()
	{
		s = NULL;
	}

	String(const char *str)
	{
		int size = strlen(str);
		s = new char[size + 1];
		strcpy(s, str);
	}

	String(const String &old_str)
	{
		cout << "copy constructor" << endl;

		int size = strlen(old_str.s);
		s = new char[size + 1];
		strcpy(s, old_str.s);
	}

	// String &operator=(String &old_str)
	// {
	// 	cout << "assignment Operator" << endl;

	// 	int size = strlen(old_str.s);
	// 	s = new char[size + 1];
	// 	strcpy(s, old_str.s);

	// 	return *this;
	// }

	void print()
	{
		cout << s << endl;
	} // Function to print string
	void change(const char *str)
	{
		delete[] s;
		int size = strlen(str);
		s = new char[size + 1];
		strcpy(s, str);
	}

	~String()
	{
		delete[] s;
	} // destructor
};

int main()
{
	String str1("GeeksQuiz");
	String str2 = str1;

	str1.print();
	str2.print();

	cout << "after change" << endl;
	str2.change("GeeksforGeeks");

	str1.print();
	str2.print();

	str2 = str1;

	str1.print();
	str2.print();

	str2.change("GeeksforGeeks");
	cout << "after change" << endl;
	str1.print();
	str2.print();

	return 0;
}


/*
Can we make copy constructor private? 
	Yes, a copy constructor can be made private. 
	When we make a copy constructor private in a class, objects of that class become non-copyable. 
	This is particularly useful when our class has pointers or dynamically allocated resources. 
	In such situations, we can either write our own copy constructor like above String example or make a private copy constructor so that users get compiler errors rather than surprises at runtime.

Why argument to a copy constructor must be passed as a reference? 
	A copy constructor is called when an object is passed by value. 
	Copy constructor itself is a function. So if we pass an argument by value in a copy constructor, a call to copy constructor would be made to call copy constructor which becomes a non-terminating chain of calls. 
	Therefore compiler doesn’t allow parameters to be passed by value.

Why argument to a copy constructor should be const? 
	See https://www.geeksforgeeks.org/copy-constructor-argument-const/ 
*/