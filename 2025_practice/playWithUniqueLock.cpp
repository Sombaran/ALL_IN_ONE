#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtx;
int sharedData = 1;



void logMessage(const std::string& msg) {
    std::unique_lock<std::mutex> lock(mtx); // Locks immediately
    ++sharedData;
    std::cout << msg << " " << sharedData << std::endl;
}

int main() {
    std::thread t1(logMessage, "Thread 1");
    std::thread t2(logMessage, "Thread 2");

    t1.join();
    t2.join();
}