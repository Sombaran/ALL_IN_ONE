/**
 * Implementing singleton design pattern.
 * It is type of Creational Pattern.
 * https://refactoring.guru/design-patterns/singleton
 * */


#include <iostream>

class Singleton {
protected:
	uint mSomeValue;
private:
	static Singleton* mInstance;
	Singleton(uint value) 
	: mSomeValue	{value} {
		std::cout << "Singleton paramterized constructor " << mSomeValue << std::endl;
	}
public:
	Singleton (const Singleton& other) = delete;
	void operator = (const Singleton& other) = delete;
	static Singleton* getInstance (uint value);
};


Singleton* Singleton::mInstance {nullptr};

Singleton* Singleton::getInstance(uint value) {
	if (nullptr == mInstance) {
		mInstance = new Singleton(value);
	}
	return mInstance;
}

int main() {
	Singleton* p = Singleton::getInstance(4200); 
	Singleton* s = Singleton::getInstance(1356); //even we create many objects only one and the top object is instantiated
	std::cout << "Address of s" << &s << std::endl;
	std::cout << "Address of p" << &p << std::endl;
	return (0);
}
