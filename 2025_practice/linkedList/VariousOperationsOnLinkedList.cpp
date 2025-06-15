/**
 * @file VariousOperationsOnLinkedList.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-06-15
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include <iostream>
using namespace std;

struct Node {
    int data;
    Node *next;

    Node(int new_data)
    : data (new_data)
    , next (nullptr){
        std::cout << __func__ << std::endl;
    }
    ~Node () {
        std::cout << __func__ << std::endl;
    }
};

// Given the head of a list, reverse the list and 
// return the head of reversed list
Node *reverseList(Node *head) {
    Node* currentPtr = head;
    Node* prevPtr =  nullptr;
    Node* nextPtr =  nullptr;


    while(currentPtr != nullptr) {
        nextPtr = currentPtr -> next;
        currentPtr -> next = prevPtr;
        prevPtr = currentPtr;
        currentPtr = nextPtr;
    }
    return prevPtr;
}

void printList(Node *node) {
    while (node != nullptr) {
        cout << " " << node->data;
        node = node->next;
    }
    std::cout << std::endl;
}

Node * returnLastElement (Node* head ) {
    Node * lastElement =  head;

    while (lastElement != nullptr) {
        lastElement =  lastElement -> next;
        //std::cout << " " << lastElement -> data << " ";
        if (lastElement -> next == nullptr) {
            std::cout << __func__  << " " << lastElement -> data;
            break;
        }
    }
    std::cout << std::endl;
    return lastElement;
}

Node * middleElement(Node* head) {
    Node* fastPtr = head;
    Node* slowPtr = head;
    while (fastPtr != nullptr and fastPtr -> next != nullptr) {
        slowPtr = slowPtr -> next;
        fastPtr = fastPtr -> next -> next;
    }
    std::cout << __func__  << " " << slowPtr -> data << std::endl;
    return slowPtr;
}

Node* getIntersectionNode(Node* head, Node* head1) {
    Node* tempA =  head;
    Node* tempB =  head1;
    while (tempA != nullptr and tempB != nullptr) {
        //cout << "getIntersectionNode "<< __LINE__ << " " << tempA->data << " " << tempB->data << " ";
        tempA = tempA->next;
        tempB = tempB->next;

        if (tempA -> next == tempB -> next) {
            std::cout << tempA << " "<< tempB << " ";
            break;
        }
    }
    std::cout << std::endl;
    return (head);
}

constexpr size_t lengthOfList(Node* &head) {
        size_t counter {};
        if (head == nullptr) {
            return (counter);
        }
        Node* temp = head;
        while(temp != nullptr) {
            //std::cout << temp -> mData << "->";
            ++counter;
            temp = temp -> next;
        }
        return (counter);
}

Node* removeNthFromEnd(Node* head, int n) {
    int getLengthOfList = lengthOfList(head);
    std::cout << getLengthOfList << std::endl;
    int deletePostion =  ((getLengthOfList - n) + 1);
    std::cout << deletePostion << std::endl;

    if (deletePostion > getLengthOfList) {
        std::cerr << "Invalid position \n";
    }
    if (deletePostion != 1) {
        Node* current = head;
        Node* previous = nullptr;

        int counter {1};

        while (counter < deletePostion){
            previous = current;
            current = current -> next;
            counter++;
        }
        previous -> next = current -> next;
        current -> next =  nullptr;
        delete current;
    }
}

void printAlternateNodes(Node *node) {
    while (node != nullptr) {
        cout << " " << node->next->data;
        node = node->next->next;
    }
    std::cout << std::endl;
}

int main() {

    // Create a hard-coded linked list:
    // 1 -> 2 -> 3 -> 4 -> 5
    Node *head = new Node(1);
    head->next = new Node(2);
    head->next->next = new Node(3);
    head->next->next->next = new Node(4);
    head->next->next->next->next = new Node(5);

    Node *head1 = new Node(6);
    head1->next = new Node(7);
    head1->next->next = new Node(3);
    head1->next->next->next = new Node(8);
    head1->next->next->next->next = new Node(9);

    cout << "Given Linked list: \n";
    printAlternateNodes(head);

    returnLastElement(head);
    printList(head);
    middleElement(head);
    head = reverseList(head);
    cout << "\nReversed Linked List: \n";
    printList(head);
    returnLastElement(head);
    printList(head);
    middleElement(head);
    std::cout << __func__  << " " << getIntersectionNode(head, head1) << std::endl;
    std::cout << __func__  << " " << removeNthFromEnd(head, 2) << std::endl; 
    printList(head);
    /**
     * @brief 
     * i/p -> 5 4 3 2 1
     * o/p -> 5 4 3 1
     */
    delete head;
    return 0;
}