#include <iostream>
#include <unordered_set>
using namespace std;

// A Linked List Node
struct Node
{
    int data;
    Node *next;
};

// Utility function to create a new node with the given data and
// pushes it onto the list's front
void push(Node *&headRef, int data)
{
    // create a new linked list node from the heap
    Node *newNode = new Node;

    newNode->data = data;
    newNode->next = headRef;
    headRef = newNode;
}

// Function to find the intersection point of two linked lists using hashing
Node *findIntersection(Node *first, Node *second)
{
    while (second != NULL)
    {
        Node *temp = first;
        while (temp != NULL)
        {
            // if both nodes are same
            if (temp == second)
                return second;
            temp = temp->next;
        }
        second = second->next;
    }
    // intersection is not present between the lists return null
    return NULL;
}

int main()
{
    // construct the first linked list (1 —> 2 —> 3 —> 4 —> 5 —> null)
    Node *first = nullptr;
    for (int i = 5; i > 0; i--)
    {
        push(first, i);
    }

    // construct the second linked list (1 —> 2 —> 3 —> null)
    Node *second = nullptr;
    for (int i = 3; i > 0; i--)
    {
        push(second, i);
    }

    // link tail of the second list to the fourth node of the first list
    second->next->next->next = first->next->next->next;

    Node *addr = findIntersection(first, second);
    if (addr)
    {
        cout << "The intersection point is " << addr->data << endl;
    }
    else
    {
        cout << "The lists do not intersect." << endl;
    }

    return 0;
}