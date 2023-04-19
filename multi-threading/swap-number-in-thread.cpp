// In thread execution arguments are copied to new threads stack so, if you need to pass references in common way i.e.
#include <iostream>
#include <thread>
using namespace std;
void swaping(int *a, int *b)
{
    int x = *b;
    *b = *a;
    *a = x;
}
int main()
{
    std::cout << "Main" << endl;
    int *p = new int(10);
    int *q = new int(20);
    std::cout << "Before Thread p: " << *p << " q: " << *q << std::endl;
    std::thread t(swaping, p, q);
    t.join();
    std::cout << "After Thread p: " << *p << " q: " << *q << std::endl;
    delete p, q;
    p, q = NULL;
    std::chrono::milliseconds dura(1000);
    std::cout << "End" << endl;
    return 0;
}
