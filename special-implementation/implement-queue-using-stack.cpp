#include <iostream>
#include <stack>
using namespace std;

class Queue
{
    std::stack<int> stack1, stack2;

public:
    void enQueue(int x)
    {
        // Move all elements from stack1 to stack2
        while (!stack1.empty())
        {
            stack2.push(stack1.top());
            stack1.pop();
        }

        // Push item into stack1
        stack1.push(x);

        // Push everything back to stack1
        while (!stack2.empty())
        {
            stack1.push(stack2.top());
            stack2.pop();
        }
    }

    // Dequeue an item from the queue
    int deQueue()
    {
        // if first stack is empty
        if (stack1.empty())
        {
            cout << "Q is Empty";
            exit(0);
        }

        // Return top of stack1
        int x = stack1.top();
        stack1.pop();
        return x;
    }
};

int main()
{

    Queue Q;

    Q.enQueue(10);
    Q.enQueue(20);
    Q.enQueue(30);

    cout << "DeQueue 1: " << Q.deQueue() << endl;
    cout << "DeQueue 2: " << Q.deQueue() << endl;
    cout << "DeQueue 3: " << Q.deQueue() << endl;

    Q.enQueue(40);
    Q.enQueue(50);

    cout << "DeQueue 4: " << Q.deQueue() << endl;
    cout << "DeQueue 5: " << Q.deQueue() << endl;
    return 0;
}
