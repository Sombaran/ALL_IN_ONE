/*
 * weak_ptr.cpp
 *
 *  Created on: 16-Mar-2022
 *      Author: Jotirling Swami
 */

// NOTES:
// 0. If we say unique_ptr is for unique ownership and shared_ptr is for shared ownership then
//    weak_ptr is for non-ownership smart pointer.
// 1. It actually reference to an object which is managed by shared_ptr.
// 2. A weak_ptr is created as a copy of shared_ptr.
// 3. We have to convert weak_ptr to shared_ptr in order to use the managed object.
// 4. It is used to remove cyclic dependency between shared_ptr.

#include <iostream>
#include <memory>
using namespace std;

#ifndef NULL

class Base {
	int val;
public:
	Base(int _val) :
			val(_val) {
		std::cout << "Base...\n";
	}
	~Base() {
		std::cout << "~Base...\n";
	}
	void print(string caller) {
		cout << caller << " Base val: " << val << endl;
	}
};

int main() {

	//-----------------------------------------------------------------------
	{
		std::shared_ptr<Base> sp1 = std::make_shared<Base>(100);
		std::shared_ptr<Base> sp2 = std::make_shared<Base>(200);
		std::weak_ptr<Base> wp1 = sp1;
		std::weak_ptr<Base> wp2 = sp2;
		cout << "before swap:" << endl;
		wp1.lock().get()->print("wp1");
		wp2.lock().get()->print("wp2");
		wp1.swap(wp2);
		cout << "after swap:" << endl;
		wp1.lock().get()->print("wp1");
		wp2.lock().get()->print("wp2");
	}
	//-----------------------------------------------------------------------

	{
		shared_ptr<int> sharedPtr = std::make_shared<int>(100);
		std::weak_ptr<int> weakPtr(sharedPtr);

		std::cout << "weakPtr.use_count(): " << weakPtr.use_count()
				<< std::endl;
		std::cout << "sharedPtr.use_count(): " << sharedPtr.use_count()
				<< std::endl;
		std::cout << "weakPtr.expired(): " << weakPtr.expired() << std::endl; // 0 means not expired

		if (std::shared_ptr<int> sharedPtr1 = weakPtr.lock()) {

			std::cout << "*sharedPtr: " << *sharedPtr << std::endl;
			std::cout << "*sharedPtr1: " << *sharedPtr1 << std::endl;
			std::cout << "sharedPtr1.use_count(): " << sharedPtr1.use_count()
					<< std::endl;
			std::cout << "sharedPtr.use_count(): " << sharedPtr.use_count()
					<< std::endl;
		} else {
			std::cout << "Don't get the resource!" << std::endl;
		}

		weakPtr.reset();
		std::cout << "weakPtr.expired(): " << weakPtr.expired() << std::endl; // 1 means its expired
		if (std::shared_ptr<int> sharedPtr1 = weakPtr.lock()) {
			std::cout << "*sharedPtr: " << *sharedPtr << std::endl;
			std::cout << "*sharedPtr1: " << *sharedPtr1 << std::endl;
			std::cout << "sharedPtr1.use_count(): " << sharedPtr1.use_count()
					<< std::endl;
			std::cout << "sharedPtr.use_count(): " << sharedPtr.use_count()
					<< std::endl;
		} else {
			std::cout << "Don't get the resource!" << std::endl;
		}
	}
	//-----------------------------------------------------------------------
	return 0;
}
#endif

class Son;
class Daughter;

class Mother {
private:
	std::weak_ptr<Son> mySon;
	std::shared_ptr<Daughter> myDaughter;

public:
	~Mother() {
		std::cout << "Mother gone" << std::endl;
	}
	void setSon(const std::weak_ptr<Son> s) {
		mySon = s.lock();
	}
	void setDaughter(const std::shared_ptr<Daughter> d) {
		myDaughter = d;
	}
};

class Son {
private:
	std::shared_ptr<const Mother> myMother;

public:
	Son(std::shared_ptr<Mother> m) :
			myMother(m) {
	}
	~Son() {
		std::cout << "Son gone" << std::endl;
	}
};

class Daughter {
private:
	std::shared_ptr<const Mother> myMother;

public:
	Daughter(std::shared_ptr<Mother> m) :
			myMother(m) {
	}
	~Daughter() {
		std::cout << "Daughter gone" << std::endl;
	}
};

int main() {
	{
		std::shared_ptr<Mother> mother = std::shared_ptr<Mother>(new Mother);

		std::shared_ptr<Son> son = std::shared_ptr<Son>(new Son(mother));
		std::shared_ptr<Daughter> daughter = std::shared_ptr<Daughter>(
				new Daughter(mother));

		mother->setSon(son);
		mother->setDaughter(daughter);
	}
	return 0;
}
