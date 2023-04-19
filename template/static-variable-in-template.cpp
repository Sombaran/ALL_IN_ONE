
#include <iostream>
using namespace std;

// Function Example
template <typename T>
void printval(const T x)
{
    static int var = 16;
    cout << "var: " << &var << endl;
    cout << ++var << endl;
}

// Class Example
template <typename T>
class Print
{
    static T var;

public:
    void printVal()
    {
        cout << "var: " << &var << endl;
        cout << ++var << endl;
    }
};

template <class T>
T Print<T>::var = 0;

int main()
{
    printval(1);
    printval(2);
    printval('x');
    printval(1.5);

    Print<int> p;
    p.printVal();

    Print<float> f;
    f.printVal();

    Print<float> f1;
    f1.printVal();

    cout << "obj p: " << &p << endl;
    cout << "obj f: " << &f << endl;
    cout << "obj f1: " << &f1 << endl;

    return 0;
}