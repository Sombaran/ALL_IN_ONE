/* Program to implement a stack using
two queue */
#include <iostream>
#include <queue>
using namespace std;

class Stack
{
    // Two inbuilt queues
    queue<int> Queue1, Queue2;

    // To maintain current number of
    // elements

public:
    void push(int x)
    {

        // Push x first in empty Queue2
        Queue2.push(x);

        // Push all the remaining
        // elements in Queue1 to Queue2.
        while (!Queue1.empty())
        {
            Queue2.push(Queue1.front());
            Queue1.pop();
        }

        // swap the names of two queues
        queue<int> q;
        Queue1 = Queue2;
        Queue2 = q;
    }

    void pop()
    {

        // if no elements are there in Queue1
        if (Queue1.empty())
            return;
        Queue1.pop();
    }

    int top()
    {
        if (Queue1.empty())
            return -1;
        return Queue1.front();
    }
};

// Driver code
int main()
{
    Stack s;
    s.push(1);
    s.push(2);
    s.push(3);

    cout << s.top() << endl;
    s.pop();
    cout << s.top() << endl;
    s.pop();
    cout << s.top() << endl;

    s.push(4);
    s.push(5);

    cout << s.top() << endl;
    s.pop();
    cout << s.top() << endl;

    return 0;
}
// This code is contributed by Chhavi
