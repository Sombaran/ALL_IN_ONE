/*
 * uniqe_ptr.cpp
 *
 *  Created on: 11-Mar-2022
 *      Author: Jotirling Swami
 */

//		*** unique_ptr ***
//	Notes:
//		 0. unique_ptr is a class template.
//		 1. unique_ptr is one of the smart pointer provided by c++11 to prevent memory leaks.
//		 2. unique_ptr wraps a raw pointer in it, and de-allocates the raw pointer, when unique_ptr object goes out of scope.
//		 3. similar to actual pointers we can use -> and * on the object of unique_ptr,
//			because it is overloaded in unique_ptr class.
//		 4. When exception comes then also it will de-allocate the memory hence no memory leak.
//		 5. Not only object we can create array of objects of unique_ptr.
//	When to use unique_ptr?
//			Use unique_ptr when you want to have single ownership(Exclusive) of the resource.
//			Only one unique_ptr can point to one resource. Since there can be one unique_ptr for single resource
//			Its not possible to copy one unique_ptr to another.
//		 Operation:
//		 release, reset, swap, get, get_deleter.
//
#include <iostream>
#include <memory>
using namespace std;

class Base
{
	int x;

public:
	Base()
	{
		x = 10;
	}
	Base(int x) : x{x}
	{
		cout << "Base Const" << endl;
	}
	int getX()
	{
		return x;
	}
	void show()
	{
		cout << "value : " << x << endl;
	}
	~Base()
	{
		cout << "Base Dest" << endl;
	}
};

int main()
{
	//-----------------------------------------------------------------------
	cout << "Main Start" << endl;

	int data1 = {10};
	cout << data1 << endl;

	int *ptrr = new int(99);
	cout << *ptrr << endl;

	int var = 30;
	int *ptr = &var;

	std::unique_ptr<int> uPtr1(ptr);
	cout << "uPtr: " << *uPtr1 << endl;

	//	 Its possible that multiple unique pointer can point to the same pointer But
	std::unique_ptr<int> uPtr2(ptr);
	cout << "uPtr2: " << *uPtr2 << endl;

	// we can not assign once unique pointer to another unique pointer
	//	std::unique_ptr<int> uPtr2(uPtr);


	std::unique_ptr<int> uniqint_ptr = make_unique<int>(10);

	std::unique_ptr<Base> uniqBase_ptr = make_unique<Base>( Base(200));

	//-----------------------------------------------------------------------
	unique_ptr<Base> b1(new Base(100));
	b1->show();
	unique_ptr<Base> b2 = std::move(b1);
	if (b1.get() != NULL)
		b1->show();
	b2->show();
	//-----------------------------------------------------------------------
	{
		Base *f = new Base(10);
		cout << "getX: " << f->getX() << endl;
		delete f;
	}
	{
		std::unique_ptr<Base> p(new Base(100));
		cout << "getX: " << p->getX() << endl;
	}

	//	 it will fail due to unique_ptr can assign to another unique_ptr
	//	 std::unique_ptr<Base> p1 = p; // Error beacuse we can assign unique_ptr to another ptr.
	//-----------------------------------------------------------------------

	cout << "Main End" << endl;
	return 0;
}
