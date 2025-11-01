#if 0
// std::lock example
#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <mutex>          // std::mutex, std::try_lock

std::mutex foo,bar;

void task_a () {
    std::unique_lock<std::mutex> lock1(foo, std::defer_lock);
    std::unique_lock<std::mutex> lock2(bar, std::defer_lock);
    //foo.lock();
    int x = std::try_lock(lock1, lock2); // Atomically lock both mutexes
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    if (x==-1) {
        std::cout << "task a\n";
    }
    
    //bar.lock();
    // ...
    //foo.unlock();
    //bar.unlock();
}

void task_b () {
    std::unique_lock<std::mutex> lock1(foo, std::defer_lock);
    std::unique_lock<std::mutex> lock2(bar, std::defer_lock);
    //foo.lock();
    //std::lock(lock1, lock2); // Atomically lock both mutexes
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    int x = std::try_lock(lock1,lock2);
    if (x==-1) {
        std::cout << "task b\n";
        // ...
        //bar.unlock();
        //foo.unlock();
}
//else {
//    std::cout << "[task b failed: mutex " << (x?"foo":"bar") << " locked]\n";
//}
}

int main ()
{
    std::thread th1 (task_a);
    std::thread th2 (task_b);   
    th1.join();
    th2.join(); 
    return 0;
}


#include <iostream>
#include <thread>
#include <mutex>

std::mutex foo, bar;

void task_a() {
    std::unique_lock<std::mutex> lock1(foo, std::defer_lock);
    std::unique_lock<std::mutex> lock2(bar, std::defer_lock);
    std::lock(lock1, lock2);

    std::cout << "task a\n";
    // Critical section
}


void task_b() {
    std::unique_lock<std::mutex> lock1(bar, std::defer_lock);
    std::unique_lock<std::mutex> lock2(foo, std::defer_lock);
    std::lock(lock1, lock2);  // Deadlock-free lock

    std::cout << "task b\n";
    // Critical section
    // Locks released automatically on scope exit
}


int main() {
    std::thread th1(task_a);
    std::thread th2(task_b);

    th1.join();
    th2.join();

    return 0;
}



#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex foo, bar;

void lock_both_mutexes(std::mutex& m1, std::mutex& m2) {
    while (true) {
        int ret = std::try_lock(m1, m2);
        if (ret == -1) {
            // Successfully locked both mutexes
            break;
        } else {
            // Unlock any mutexes locked before failure
            if (ret != 0) m1.unlock();
            if (ret != 1) m2.unlock();

            // Back off and retry to avoid busy waiting
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
}

void task_a() {
    lock_both_mutexes(foo, bar);
    std::cout << "task a\n";
    bar.unlock();
    foo.unlock();
}

void task_b() {
    lock_both_mutexes(bar, foo);
    std::cout << "task b\n";
    foo.unlock();
    bar.unlock();
}

int main() {
    std::thread th1(task_a);
    std::thread th2(task_b);

    th1.join();
    th2.join();

    return 0;
}

#endif


#if 0
// std::lock example
#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <mutex>          // std::mutex, std::try_lock

std::mutex foo,bar;

void task_a () {
    std::unique_lock<std::mutex> lock1(foo, std::defer_lock);
    std::unique_lock<std::mutex> lock2(bar, std::defer_lock);
    //foo.lock();
    int x = std::try_lock(lock1, lock2); // Atomically lock both mutexes
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    if (x==-1) {
        std::cout << "task a\n";
    }
    
    //bar.lock();
    // ...
    //foo.unlock();
    //bar.unlock();
}

void task_b () {
    std::unique_lock<std::mutex> lock1(foo, std::defer_lock);
    std::unique_lock<std::mutex> lock2(bar, std::defer_lock);
    //foo.lock();
    //std::lock(lock1, lock2); // Atomically lock both mutexes
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    int x = std::try_lock(lock1,lock2);
    if (x==-1) {
        std::cout << "task b\n";
        // ...
        //bar.unlock();
        //foo.unlock();
}
//else {
//    std::cout << "[task b failed: mutex " << (x?"foo":"bar") << " locked]\n";
//}
}

int main ()
{
    std::thread th1 (task_a);
    std::thread th2 (task_b);   
    th1.join();
    th2.join(); 
    return 0;
}


#include <iostream>
#include <thread>
#include <mutex>

std::mutex foo, bar;

void task_a() {
    std::unique_lock<std::mutex> lock1(foo, std::defer_lock);
    std::unique_lock<std::mutex> lock2(bar, std::defer_lock);
    std::lock(lock1, lock2);

    std::cout << "task a\n";
    // Critical section
}


void task_b() {
    std::unique_lock<std::mutex> lock1(bar, std::defer_lock);
    std::unique_lock<std::mutex> lock2(foo, std::defer_lock);
    std::lock(lock1, lock2);  // Deadlock-free lock

    std::cout << "task b\n";
    // Critical section
    // Locks released automatically on scope exit
}


int main() {
    std::thread th1(task_a);
    std::thread th2(task_b);

    th1.join();
    th2.join();

    return 0;
}

#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

// Function to lock two mutexes using std::try_lock with retry until success
void lock_two_mutexes(std::mutex& first, std::mutex& second, const std::string& task_name) {
    while (true) {
        int ret = std::try_lock(first, second);
        if (ret == -1) {
            // Successfully locked both mutexes
            std::cout << task_name << "\n";
            // Critical section here

            // Unlock in reverse order of locking for safety
            second.unlock();
            first.unlock();
            break;
        } else {
            // Unlock any mutex that was locked before failure
            if (ret != 0) first.unlock();
            if (ret != 1) second.unlock();

            // Back off a bit to avoid busy waiting
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
}

int main() {
    std::mutex foo, bar;

    // Launch two threads locking mutexes in opposite order
    std::thread th1(lock_two_mutexes, std::ref(foo), std::ref(bar), "task a");
    std::thread th2(lock_two_mutexes, std::ref(bar), std::ref(foo), "task b");

    th1.join();
    th2.join();

    return 0;
}
#endif



#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <memory>

std::mutex mtx;

int value = 1;
void someFunction() {
    std::unique_lock<std::mutex> lck(mtx, std::defer_lock);
    ++value;
    std::cout << __func__ << " 1 "  << value << std::endl;
    return;
}

void workerThread() {
    std::cout << __func__ << " 1 " << value << std::endl;
    ++value;
    std::unique_lock<std::mutex> lck(mtx, std::defer_lock);
    someFunction();
    ++value;
    std::cout << __func__ << " 2 " << value << std::endl;
}




int main() {

    std::unique_ptr<std::thread> t1= std::make_unique<std::thread>(workerThread);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));    
    std::cout << __func__ << " 1 " << std::endl;

    if (t1 -> joinable()) {
        std::cout << __func__ << " 2 " << std::endl;
        t1 -> join();
    }
    std::cout << __func__ << " 3 " << std::endl;
    return (0);
}