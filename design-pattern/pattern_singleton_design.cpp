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
private:
	static Singleton* mInstance;
	Singleton() 
	{
		std::cout << "I am a Singleton constructor " << std::endl;
	}
public:
	Singleton (const Singleton& other) = delete;
	Singleton& operator = (const Singleton& other) = delete;
	static Singleton* getInstance () {
		std::lock_guard<std::mutex> gLock (mtx);
		if (nullptr == mInstance) {
			mInstance = new (std::nothrow)Singleton();
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
	Singleton::getInstance()->printAddress();

}

Singleton* Singleton::mInstance {nullptr};

int main() {
	std::thread t1(printCalling);
	std::thread t2(printCalling);

	if (t1.joinable() and t2.joinable()) {
		t1.join(); t2.join();
	}
	Singleton::deleteInstance();
	return (0);
}
