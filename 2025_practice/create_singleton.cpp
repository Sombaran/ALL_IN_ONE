/**
* @Brief: Creating singleton class using mutex
*/

#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtx;

class creatinalSingleton{
    private:
    static creatinalSingleton* mInstance;
    explicit creatinalSingleton() {
        std::cout << __func__  << " " << this << std::endl;
    }
    ~ creatinalSingleton() {
        std::cout << __func__ << std::endl;
    }

    public:
    creatinalSingleton (const creatinalSingleton& ) = delete;
	creatinalSingleton& operator = (const creatinalSingleton& ) = delete;
	creatinalSingleton& operator = (const creatinalSingleton&& ) = delete;

    static creatinalSingleton* getInstance () {
        std::unique_lock<std::mutex> gLock (mtx);
        //gLock.lock();
        if (nullptr == mInstance) {
            mInstance = new (std::nothrow) creatinalSingleton();
        }
        gLock.unlock();
        return mInstance;
    }

    static void deleteInstance() {
        if (mInstance) {
            delete mInstance;
            mInstance = nullptr;
        }
    }
    void printAddress() {
		std::cout << "My instance memory address is:" << this << std::endl;
	}
};

void clientCode () {
    creatinalSingleton::getInstance()->printAddress();
}

creatinalSingleton* creatinalSingleton::mInstance {nullptr};


int main(){
    std::cout << __func__ << std::endl;
    std::thread threadCaller1 (clientCode);
    std::thread threadCaller2 (clientCode);
    if (threadCaller1.joinable() and threadCaller2.joinable()) {
        threadCaller1.join();
        threadCaller2.join();
    }
    creatinalSingleton::deleteInstance();
}
