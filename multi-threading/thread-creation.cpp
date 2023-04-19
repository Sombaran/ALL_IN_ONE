// Thread Creation in C++
// Type of thread creation in c++
//      1. Function Pointer
//      2. Lambda functions
//      3. Function Objects

// 1. Function Pointer

#include <iostream>
#include <thread>
using namespace std;

#ifndef NULL
void fun(int x)
{
    while (x > 0)
    {
        cout << --x << " ";
    }
}
int main()
{
    void (*fun_ptr)(int);
    fun_ptr = fun;
    std::thread t(fun_ptr, 10);
    t.join();
    return 0;
}
#endif

// 2. Lambda functions

#ifndef NULL
int main()
{

    auto fun = [](int val)
    {
        while (val != 0)
            cout << --val << " ";
    };

    std::thread t1(fun, 10);
    t1.join();

    return 0;
}
#endif

//  3. Function object

#ifndef NULL
class simple
{
public:
    void operator()(int val)
    {
        while (val != 0)
            cout << --val << " ";
    }
};

int main()
{
    simple s1;
    std::thread t1(s1, 10);
    t1.join();

    return 0;
}
#endif

//  4. Non static member function

#ifndef NULL
class simple
{
public:
    void run(int val)
    {
        while (val != 0)
            cout << --val << " ";
    }
};
int main()
{
    simple s1;
    std::thread t1(&simple::run, &s1, 10);
    t1.join();

    return 0;
}
#endif

//  5. static member function

#ifndef NULL
class simple
{
public:
    static void fun(int val)
    {
        while (val != 0)
            cout << --val << " ";
    }
};
int main()
{
    std::thread t2(&simple::fun, 10);
    t2.join();

    return 0;
}
#endif

// Note:

void function()
{
    cout << std::this_thread::get_id() << endl;
}
int main()
{

    cout << std::this_thread::get_id() << endl;
    std::thread t1(function);
    t1.join();
    return 0;
}