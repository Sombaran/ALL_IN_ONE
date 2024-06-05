#include <iostream>
#include <thread>
#include <functional> 

using namespace std;
void fun(int x)
{
    while (x > 0)
    {
        cout << --x << " ";
    }
    std::cout << std::endl;
}
std::function<void(int)> fn4 = [](int x)->void{
    std::cout <<"som " << x << std::endl;
    while (x > 0)
    {
        cout << --x << " ";
    }
    std::cout << std::endl;
};

class threadCreation {
    public:
    static void func() {
        std::cout << __func__  << " " <<__LINE__ << std::endl;
    }
    void fun() {
        std::cout << __func__  << " " <<__LINE__ << std::endl;
    }
};
int main()
{
    void (*fun_ptr)(int);
    fun_ptr = fun;
    std::thread t1(fun_ptr, 10);
    if (t1.joinable()) {
        t1.join();
    }
    auto fun = [&](int val)->void {
        while (val != 0)
            cout << --val << " ";
    };
    
    std::thread t2 (fun, 22);
    if (t2.joinable()) {
        t2.join();
    }
    std::cout << std::endl;
    std::function <void(int)> functionObj {fun};
    functionObj(11);
    std::cout << std::endl;
    std::thread t3(functionObj, 21);
    if (t3.joinable()) {
        t3.join();
    }
    std::cout << std::endl;
    int value{5};
    std::thread t4(fn4,5);
    if (t4.joinable()) {
        t4.join();
    }
    std::thread t5 (&threadCreation::func);
    if (t5.joinable()) {
        t5.join();
    }
    // create object for non station data member
    threadCreation threadCreationObject;
    std::thread t6 (&threadCreation::fun,&threadCreationObject);
    if (t6.joinable()) {
        t6.join();
    }
    return (0);
}