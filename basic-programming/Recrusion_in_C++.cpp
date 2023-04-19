#include <iostream>
using namespace std;

void print(int times)
{
    if (times == 0)
        return;

    --times;
    print(times);
    cout << "Reverse: " << times << endl;
}

int main()
{
    print(3);

    return 0;
}

/*
Output:

Reverse: 0
Reverse: 1
Reverse: 2

*/