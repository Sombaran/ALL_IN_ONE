
//============================================================================
// Name        : CPlusPlusProgram.cpp
// Author      : Jotirling Swami
// Version     :
// Copyright   : Your copyright notice
// Description : singleton-design-pattern.cpp
//============================================================================

#include <iostream> // std::cout
#include <mutex>    // std::mutex
#include <thread>   // thread

std::mutex mtx;

class SingleInstance
{
private:
    // Construct and analyze it as private, prohibit external structure and Deconstruction
    SingleInstance()
    {
        std::cout << "Constructor Called" << std::endl;
    }

    ~SingleInstance()
    {
        std::cout << "Destructor Called" << std::endl;
    }

    // The forbid external copy and assignment into private functions:

    // Copy construction
    SingleInstance(const SingleInstance &signal);

    // Assignment Operator
    const SingleInstance &operator=(const SingleInstance &signal);

private:
    // Unique singleton object pointer
    static SingleInstance *m_SingleInstance;

public:
    // Getting singleton objects
    static SingleInstance *GetInstance()
    {
        if (m_SingleInstance == NULL)
        {
            std::lock_guard<std::mutex> g_lock(mtx); // lock
            if (m_SingleInstance == NULL)
            {
                m_SingleInstance = new (std::nothrow) SingleInstance; // Threads are insecure without locking, and multiple instances are created when threads are concurrent
            }
        }
        return m_SingleInstance;
    }
    // Release the singleton and call it when the process exits
    static void deleteInstance()
    {
        if (m_SingleInstance)
        {
            delete m_SingleInstance;
            m_SingleInstance = NULL;
        }
    }
    // Print singleton address
    void Print()
    {
        std::cout << "My instance memory address is:" << this << std::endl;
    }
};

// Initialize static member variables
SingleInstance *SingleInstance::m_SingleInstance = NULL;

// Thread function
void PrintHello()
{
    std::cout << "Hi, I am a thread ID:[" << std::this_thread::get_id() << "]" << std::endl;

    // Print instance address
    SingleInstance::GetInstance()->Print();
}

#define NUM_THREADS 5 // Number of Threads

int main(void)
{

    std::cout << "main() : Start! " << std::endl;

    std::thread thread1(PrintHello);
    std::thread thread2(PrintHello);

    thread1.join();
    thread2.join();

    // Manual release of single instance resources
    SingleInstance::deleteInstance();
    std::cout << "main() : End! " << std::endl;
    return 0;
}
// Output:
//     main() : Start!
//     Hi, I am a thread ID:[0x800083dd0]
//     Hi, I am a thread ID:[0x800083ed0]
//     Constructor Called
//     My instance memory address is:0x800061900
//     My instance memory address is:0x800061900
//     Destructor Called
//     main() : End!