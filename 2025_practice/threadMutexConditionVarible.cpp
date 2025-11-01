


#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mu;
std::condition_variable cond;
int i = 1;

void printOdd(int start, int end) {
    while (i<100)
    {
        std::unique_lock<std::mutex> locker(mu);
        while (not (i%2 == 1 )) {
            cond.wait(locker);
        }
        //cond.wait(locker,[&](){ return (i%2 == 1); });
        //std::cout << "From Even: " << i << std::endl;
        std::cout << i << " ";
        ++i;
        locker.unlock();
        cond.notify_all();
    }
    //std::cout << std::endl;
}

void printEvn(int start, int end) {
    //for(; i < 100;)
    while (i<100)
    {
        std::unique_lock<std::mutex> locker(mu);
        while (not (i%2 ==0 )) {
            cond.wait(locker);
        }
        //cond.wait(locker,[&](){ return (i%2 == 0); });
        //std::cout << "From Even: " << count << std::endl;
        std::cout << i << " ";
        ++i;
        locker.unlock();
        cond.notify_all();
    }
	//std::cout << std::endl;
}


int main() {

	int start = 1;
	int end   = 100;
	std::thread odd (printOdd, start, end);
	std::thread evn (printEvn, start, end);


	if (odd.joinable() and evn.joinable()) {
        odd.join();
        evn.join();
	}
	return(0);
}