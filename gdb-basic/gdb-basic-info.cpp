// gdb basic:
/*
Commands
    breakpoint  b
    list        l
    frame       f
    next        n
    step        s
    backtrace   b
    print       p
    info        i
    watch
    */

#include <iostream>
using namespace std;
void myswap(int x, int y)
{
    int t = x;
    x = y;
    y = t;
}
int main()
{
    int a = 10, b = 5;
    myswap(a, b);
    cout << a << " " << b << endl;
    return 0;
}