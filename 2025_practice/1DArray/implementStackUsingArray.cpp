#include <iostream>
#include <memory>

using namespace std;

class myStack {
    
    // array to store elements
    int *arr;       
    
    // maximum size of stack
    int capacity;   
    
    // index of top element
    int top;        

public:

    // constructor
    myStack(int cap) 
    : capacity(cap)
    , top(-1)
    , arr(new int[cap]){
        std::cout << "Stack of capacity " << capacity << " created in constructor \n";  
    }

    // push operation
    void push(int x) {
        if (top == capacity - 1) {
            cout << "Stack Overflow\n";
            return;
        }
        arr[++top] = x;
    }

    // pop operation
    int pop() {
        if (top == -1) {
            cout << "Stack Underflow\n";
            return -1;
        }
        return arr[top--];
    }

    // peek (or top) operation
    int peek() {
        if (top == -1) {
            cout << "Stack is Empty\n";
            return -1;
        }
        return arr[top];
    }

    // check if stack is empty
    bool isEmpty() {
        return top == -1;
    }

    // check if stack is full
    bool isFull() {
        return top == capacity - 1;
    }
};

int main() {
    //myStack st(4);
    std::shared_ptr<myStack> stackPtr = std::make_shared<myStack>(4);
    // pushing elements
    stackPtr->push(1);
    stackPtr->push(2);
    stackPtr->push(3);
    stackPtr->push(4);

    // popping one element
    cout << "Popped: " << stackPtr->pop() << "\n";
    // checking top element
    cout << "Top element: " << stackPtr->peek() << "\n";

    // checking if stack is empty
    cout << "Is stack empty: " << (stackPtr->isEmpty() ? "Yes" : "No") << "\n";

    // checking if stack is full
    cout << "Is stack full: " << (stackPtr->isFull() ? "Yes" : "No") << "\n";

    return 0;
}