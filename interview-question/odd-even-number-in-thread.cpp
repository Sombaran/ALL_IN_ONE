
#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>

// using namespace std;
std::mutex mtx;
std::condition_variable cond;
int value;

void printOdd()
{
    while (value < 100)
    {
        std::unique_lock<std::mutex> ulock(mtx);
        cond.wait(ulock, []()
                  { return value % 2 != 0; });
        std::cout << "printOdd: " << value++ << std::endl;
        ulock.unlock();
        cond.notify_one();
    }
}

void printEven()
{
    while (true)
    {
        std::unique_lock<std::mutex> ulock(mtx);
        cond.wait(ulock, []()
                  { return value % 2 == 0; });
        std::cout << "printEven: " << value++ << std::endl;
        ulock.unlock();
        cond.notify_one();
    }
}

int main()
{
    std::thread t1(printOdd);
    std::thread t2(printEven);

    t1.join();
    t2.join();

    return 0;
}