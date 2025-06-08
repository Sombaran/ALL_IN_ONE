/**
* @Brief: Concurrency using thread C++
*/


#include <iostream>
#include <thread>
#include <chrono>
#include <functional>


using ull = unsigned long long;
ull evenSum {0};
ull oddSum {0};

void findEven(ull, ull );
void findOdd(ull , ull );
void fun(int);


void findEven(ull start, ull end) {
    for (int i=start;i<end;++i) {
        if ((i&1) == 0) {
            evenSum+= i;
        }
    }
}

void findOdd(ull start, ull end) {
    for (int i=start;i<end;++i) {
        if ((i&1) == 1) {
            oddSum+= i;
        }
    }
}

void fun(int x)
{
    while (x > 0)
    {
        std::cout << --x << " ";
    }
    std::cout << std::endl;
}

int main() {
    ull start {}, end {1900000000};

    auto startTime = std::chrono::high_resolution_clock::now();    
    //findEven(start, end);
    //findOdd(start, end);
    std::thread t1 (findEven,start, end);
    std::thread t2 (findOdd,start, end);
    if (t1.joinable() and t2.joinable()) {
        std::cout << "Thread for t1 " << t1.get_id() << std::endl; 
        std::cout << "Thread for t2 " << t2.get_id() << std::endl; 
        t1.join();
        t2.join();
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    auto totalDuration = std::chrono::duration_cast<std::chrono::seconds> (endTime - startTime);
    std::cout << "Even sum " << evenSum << std::endl;
    std::cout << "Odd sum " << oddSum << std::endl;
    std::cout << "Total time taken to execute the code is " << totalDuration.count() << " seconds " << std::endl; 

    auto unNamedFunction = [&](int val)->void {
        while (val != 0)
        std::cout << --val << " ";
    };
    std::thread lambdaThread (fun, 10);
    if (lambdaThread.joinable()) {
        std::cout << "Thread for threadLambda " << lambdaThread.get_id() << std::endl;
        lambdaThread.join();
    }
    std::function <void(int)> functionObj {fun};
    //functionObj(11);
    std::cout << std::endl;
    std::thread t3(functionObj, 21);
    if (t3.joinable()) {
        std::cout << "Thread for function " << t3.get_id() << std::endl;
        t3.join();
    }
    return (0);
}
