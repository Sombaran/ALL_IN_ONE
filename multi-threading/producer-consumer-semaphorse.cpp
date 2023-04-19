
#include <iostream>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

int mutex = 1, full = 0, Empty = 3, x = 0, i = 1;
int wait(int s)
{
    return (--s);
}
int signal(int s)
{
    return (++s);
}

void consumer()
{
    mutex = wait(mutex);
    full = wait(full);
    Empty = signal(Empty);
    cout << "Consumer consumes item : " << x << endl;
    mutex = signal(mutex);
}
void producer()
{
    mutex = wait(mutex);
    full = signal(full);
    Empty = wait(Empty);
    x++;
    cout << "Producer produces the item :" << x << endl;
    mutex = signal(mutex);
    consumer();
}

int main()
{
    int n;
    cout << "1.Producer\n2.Consumer\n3.Exit" << endl;
    while(1)
    {
        cout << "XnEnter your choice" << endl;
        cin >> n;
        switch (n)
        {
        case 1:
            if ((mutex == 1) && (Empty!=0))
                producer();
            else
                cout << "Buffer is full" << endl;
            break;
        case 2:
            if ((mutex == 1) && (full!=0))
                consumer();
            else
                cout << "Buffere is Empty" << endl;
            break;
        case 0:
            exit(0);
            break;
        }
    }
    return 0;
}