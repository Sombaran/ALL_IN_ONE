

#include <iostream>
#include <thread>
#include <algorithm>
#include <vector>

class WorkerThread
{
public:
    void operator()()
    {
        std::cout << "Worker Thread " << std::this_thread::get_id() << " is Executing" << std::endl;
    }
};
int main()
{
    std::vector<std::thread> threadList;
    for (int i = 0; i < 10; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        threadList.push_back(std::thread(WorkerThread()));
    }
    // Now wait for all the worker thread to finish i.e.
    // Call join() function on each of the std::thread object
    std::cout << "wait for all the worker thread to finish" << std::endl;

    std::for_each(threadList.begin(), threadList.end(), [](std::thread& t1) {
        if(t1.joinable()) 
        {
            t1.join();
        }
        });

    std::cout << "Exiting from Main Thread" << std::endl;
    return 0;
}