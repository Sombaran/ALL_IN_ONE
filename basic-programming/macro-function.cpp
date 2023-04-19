#include <iostream>
using namespace std;

#define Max(x, y) (((x) > (y)) ? x : y)

int main()
{
    int x = 3, y = 5;
    cout << "Max: (" << x << "," << y << ") = ";
    cout << max(x++, y++) << endl;
    cout << "x: " << x << " y: " << y << endl;
}