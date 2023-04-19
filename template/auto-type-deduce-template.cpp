#include <iostream>
using namespace std;

template <class RT, class T1, class T2>
RT getMax(T1 x, T2 y)
{
    cout << "x:" << x << " y:" << y << endl;
    return x > y ? x : y;
}

int const &max(int const &a, int const &b)
{
    cout << "max(int, int)" << endl;
    cout << "a: " << a << " b: " << b << endl;
    return a > b ? a : b;
}

template <typename T>

T const &max(T const &a, T const &b)
{
    cout << "max(T, T)" << endl;
    cout << "a: " << a << " b: " << b << endl;
    return a > b ? a : b;
}

template <typename T>

T const &max(T const &a, T const &b, T const &c)
{
    cout << "max(T. T. T)" << endl;
    cout << "a: " << a << " b: " << b << " c: " << c << endl;
    return max(max(a, b), c);
}

int new_main()

{
    // ::max(10.6, 26.6);
    // ::max('a', 'b');
    // ::max(10, 20);
    // ::max<>(10, 20);
    // ::max<double>(10, 20);
    ::max(10, 20, 30);

    return 0;
}

int main()
{
    // here double is first parameter as RT and T1 match to 20 and T2 match to 30.5
    cout << getMax<double>(20, 30.5) << endl;

    cout << getMax<double, int, double>(30, 20.5) << endl;

    new_main();
    return 0;
}