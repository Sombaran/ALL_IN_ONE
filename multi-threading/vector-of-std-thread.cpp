#include <thread>
#include <vector>
#include <iostream>
#include <functional>
int main()
{
    // Create a vector of threads
    std::vector<std::thread> vecOfThreads;
    // Create a function object
    std::function<void()> func = []()
    {
        // Do Some Important Work
        //  .....
        // Print Thread ID
        std::cout << "From Thread ID : " << std::this_thread::get_id() << "\n";
    };
    // Add a Thread object to vector
    vecOfThreads.push_back(std::thread(func));
    // Create 3 differet thread objects
    std::thread th1(func);
    std::thread th2(func);
    std::thread th3(func);
    // Move all three thread objects to vector
    vecOfThreads.push_back(std::move(th1));
    vecOfThreads.push_back(std::move(th2));
    vecOfThreads.push_back(std::move(th3));
    // Do some important work in main thread.
    /** Wait for all the threads in vector to join **/
    // Iterate over the thread vector
    for (std::thread &th : vecOfThreads)
    {
        // If thread Object is Joinable then Join that thread.
        if (th.joinable())
            th.join();
    }
    return 0;
}