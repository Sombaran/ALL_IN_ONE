

// 1. There are two places where we can apply templates.
// a. function templates
// b. class templates
// 2. Templates are the way to write generic programs.
// 3. The main point is we pass data type as parameter to function or class.
// 4. Compiler will create internally copy of the function with the respective datatypes in compilation time.

// Example of max function.
#include <iostream>
using namespace std;

template <class T>
class base
{
    T value;

public:
    base(T val)
    {
        value = val;
    }
    void print()
    {
        cout << value << endl;
    }

    T add(T x, T y)
    {
        return x + y;
    }
};

template <typename T>
T getMax(T x, T y)
{
    return x > y ? x : y;
}

int main()
{
    int x = 10, y = 30;
    cout << getMax<int>(x, y) << endl;

    char a = 'A', b = 'Z';
    // we don't need to specify explicity that type-cast to char and also no need to pass the char template arg
    // getMax<char>(a, b) // this also works and output: Z
    cout << static_cast<char>(getMax<char>(a, b)) << endl;

    base b1("swami");
    b1.print();

    base b2(100);
    b2.print();

    cout << b2.add(10, 20) << endl;

    return 0;
}
