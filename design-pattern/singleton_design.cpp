/**
 * Implementing singleton design pattern.
 * It is type of Creational Pattern.
 * https://refactoring.guru/design-patterns/singleton
 * */


#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtx;

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
	static Singleton* getInstance (uint value) {
		std::lock_guard<std::mutex> gLock (mtx);
		if (nullptr == mInstance) {
			mInstance = new (std::nothrow)Singleton(value);
		}
		return mInstance;
	}

	static void deleteInstance () {
		if (mInstance) {
			delete mInstance;
			mInstance = nullptr;
		}
	}

	void printAddress() {
		std::cout << "My instance memory address is:" << this << std::endl;
	}
};

void printCalling () {
	std::cout << "Hi, I am a thread ID:[" << std::this_thread::get_id() << "]" << std::endl;
	//Singleton::getInstance(4200)->printAddress();
	Singleton::getInstance(1356)->printAddress();

}

Singleton* Singleton::mInstance {nullptr};

int main() {
	//Singleton* p = Singleton::getInstance(4200); 
	//Singleton* s = Singleton::getInstance(1356); //even we create many objects only one and the top object is instantiated
	//std::cout << "Address of s" << &s << std::endl;
	//std::cout << "Address of p" << &p << std::endl;

	std::thread t1(printCalling);
	std::thread t2(printCalling);

	if (t1.joinable() and t2.joinable()) {
		t1.join(); t2.join();
	}
	Singleton::deleteInstance();
	return (0);
}
