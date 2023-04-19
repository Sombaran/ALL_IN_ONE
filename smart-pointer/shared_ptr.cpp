/*
 * shared_ptr.cpp
 *
 *  Created on: 11-Mar-2022
 *      Author: Jotirling Swami
 */

//	*** shared_ptr ***
//	shared pointer:
//		Its a container for raw pointers.
//		It is a reference counting ownership model i.e.
//		it maintains the reference count of its contained pointer in cooperation with all copies of the shared_ptr.
//		So, the counter is incremented each time a new pointer points to the resource
//		decremented when the destructor of the object is called.
//
//	Reference Counting:
//		It is a technique of storing the number of references, pointers or handles to a resource such as an object,
//		block of memory, disk space or other resources.
//
//		An object referenced by the contained raw pointer will not be destroyed until reference count is greater than zero i.e.
//		until all copies of shared_ptr have been deleted.
//		So, we should use shared_ptr when we want to assign one raw pointer to multiple owners.
//
//	When to use shared_ptr?
//		Use shared_ptr if you want to share ownership of a resource.
//		Many shared_ptr can point to a single resource. shared_ptr maintains reference count for this propose.
//		when all shared_ptr pointing to resource goes out of scope the resource is destroyed.
//	Notes:
//		 0. shared_ptr is a smart pointer which can share the ownership of object (managed object).
//		 1. Several shared_ptr can point to the same object (managed object).
//		 2. It keep a reference count to maintain how many shared_ptr are pointing to the same object.
//			and once last shared_ptr goes out of scope then the managed object gets deleted.
//		 3. shared_ptr is threads safe and not thread safe. [what is this??]
//				a. control block is thread safe
//				b. managed object is not
//		 4. There are three ways shared_ptr will destroyed managed object.
//				a. If the last shared_ptr goes out of scope.
//				b. If you initialize shared_ptr with some other shared_ptr.
//				c. If you reset shared_ptr.
//		 5. Reference count doesn't work when we use reference or pointer of shared_ptr.
//		 6. If we create multiple shared_ptr by passing one shared_ptr,
//		 it increment the count but the destructor will call only once
//   Functionality
//		1. reset() : Replaces the managed object with an object pointed to by ptr.
//		2. swap() : Exchanges the stored pointer values and the ownerships of *this and r. Reference counts, if any, are not adjusted.
//		3. get() : Returns the stored pointer.
//		4. use_cout() : Returns the number of different shared_ptr instances\
//		5. unique() : Checks if the only shared_ptr instance managing the current object, i.e. whether use_count() == 1.
//
#include <memory>
#include <iostream>
using namespace std;

class Base
{
	int x;

public:
	Base(int x) : x{x}
	{
		cout << "Base constructor for : " << x << endl;
	}
	int getX()
	{
		return x;
	}
	~Base()
	{
		cout << "Base destructor for : " << x << endl;
	}
};

int main()
{

	cout << "main" << endl;
	std::cout << "0) General" << endl;
	//-----------------------------------------------------------------------
	{
		Base *f = new Base(1);
		cout << f->getX() << endl;
		delete f;
	}
	cout << endl;
	{
		// std::shared_ptr<Base> spNew = std::shared_ptr<Base>(make_shared<Base>(Base(100)));
		std::shared_ptr<Base> spNew = std::shared_ptr<Base>(new Base(500));
		cout << spNew.use_count() << endl;
	}
	//-----------------------------------------------------------------------
	std::cout << "1) Reset" << endl;
	{
		// Reset the shared_ptr without handing it a fresh instance of Base.
		// The old instance will be destroyed after this call.

		std::shared_ptr<Base> sptr = std::make_shared<Base>(200);

		std::cout << "Base::bar = " << sptr->getX() << ", use_count() = "
				  << sptr.use_count() << '\n';

		sptr.reset(); // calls Base's destructor here

		std::cout << "After reset(): use_count() = " << sptr.use_count()
				  << ", sptr = " << sptr << '\n';
	}

	{
		// Reset the shared_ptr, hand it a fresh instance of Base.
		// The old instance will be destroyed after this call.

		std::shared_ptr<Base> sptr = std::make_shared<Base>(300);
		std::cout << "Base::bar = " << sptr->getX() << ", use_count() = "
				  << sptr.use_count() << '\n';

		sptr.reset(new Base{333}); // calls Base's destructor here

		std::cout << "Base::bar = " << sptr->getX() << ", use_count() = "
				  << sptr.use_count() << '\n';
	}
	//-----------------------------------------------------------------------
	{
		std::cout << "2) Swap" << endl;
		std::shared_ptr<Base> p1 = std::make_shared<Base>(22);
		std::shared_ptr<Base> p2 = std::make_shared<Base>(33);

		std::cout << "Base::bar p1= " << p1->getX() << ", use_count() = "
				  << p1.use_count() << '\n';
		std::cout << "Base::bar p2= " << p2->getX() << ", use_count() = "
				  << p2.use_count() << '\n';

		p1.swap(p2);

		std::cout << "Base::bar p1= " << p1->getX() << ", use_count() = "
				  << p1.use_count() << '\n';
		std::cout << "Base::bar p2= " << p2->getX() << ", use_count() = "
				  << p2.use_count() << '\n';

		cout << "reset p1" << endl;
		p1.reset();
		if (p1.get() == NULL)
			cout << "p1 NULL" << endl;

		cout << "swap p2 -> p1" << endl;
		p1.swap(p2);

		if (p2.get() == NULL)
			cout << "p2 NULL" << endl;

		std::cout << "Base::bar p1= " << p1->getX() << ", use_count() = "
				  << p1.use_count() << '\n';
		if (p1.get() != NULL)
			cout << "p1 Not NULL" << endl;
	}
	//-----------------------------------------------------------------------
	std::cout << "3) get" << endl;
	{
		std::shared_ptr<Base> p1 = std::make_shared<Base>(22);

		std::cout << "Base::bar p1= " << p1->getX() << ", use_count() = "
				  << p1.use_count() << '\n';
		p1.reset();
		if (p1.get() == NULL)
			cout << "Shared Pointer p1 Is NULL" << endl;
	}
	//-----------------------------------------------------------------------
	std::cout << "3) use_cout" << endl;
	{
		std::shared_ptr<Base> sp1(new Base(10));
		cout << "shared ptr sp1 getX: " << sp1->getX() << endl;
		cout << "shared ptr sp1 count: " << sp1.use_count() << endl
			 << endl;

		std::shared_ptr<Base> sp2 = sp1;
		cout << "shared ptr sp1 count: " << sp1.use_count() << endl;
		cout << "shared ptr sp2 count: " << sp2.use_count() << endl
			 << endl;

		std::shared_ptr<Base> &sp3 = sp1;
		cout << "shared ptr sp1 count: " << sp1.use_count() << endl;
		cout << "Reference shared ptr sp3 count: " << sp3.use_count() << endl
			 << endl;

		std::shared_ptr<Base> *sp4 = &sp1;
		cout << "shared ptr count: " << sp1.use_count() << endl;
		cout << "Pointer shared ptr count: " << sp4->use_count() << endl
			 << endl;
	}
	//-----------------------------------------------------------------------
	std::cout << "3) unique" << endl;
	{
		std::shared_ptr<Base> sp1 = make_shared<Base>(10);
		std::cout << std::boolalpha;
		std::cout << "sp1.unique() == " << sp1.unique() << '\n';

		std::shared_ptr<Base> sp2 = sp1;
		std::cout << "sp1.unique() == " << sp1.unique() << '\n';
	}
	//-----------------------------------------------------------------------
	cout << "End" << endl;
	return 0;
}
