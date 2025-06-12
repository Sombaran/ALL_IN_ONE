#include <iostream>

struct doublyList {
    int mData;
    doublyList* mPrev;
    doublyList* mNext;

    doublyList() = default;

    doublyList(int data)
    : mData (data)
    , mPrev (nullptr)
    , mNext (nullptr) {
        std::cout << __func__ << std::endl;
    }

    virtual ~doublyList() {
        std::cout << __func__ << std::endl;
        if (mPrev != nullptr and mNext != nullptr) {
            delete mPrev;
            delete mNext;
            mNext = mPrev = nullptr;
        }
    }
        void printNode(doublyList* &head) {
        std::cout << __func__ << std::endl;
        if (nullptr == head) {
            std::cerr << "List is empty \n";
            return;
        }
        doublyList* temp =  head;

        while(temp != nullptr) {
            std::cout << temp -> mData << "->";
            temp = temp -> mNext;
        }
        std::cout << std::endl;
    }

    void insertAtHead(doublyList* &tail, doublyList* &head, const int& d) {
        if (nullptr != head) {
            doublyList* temp =  new doublyList(d);
            temp -> mNext =  head;
            head -> mPrev = temp;
            head =  temp;
        }
        else if (nullptr == head) {
            doublyList* temp = new doublyList(d);
            head =  temp;
            tail = temp;
        }
    }
    
    void insertAtTail(doublyList* &tail, doublyList* &head, const int& d) {
        if (nullptr != tail) {
            doublyList* temp =  new doublyList(d);
            tail -> mNext = temp;
            temp -> mPrev = tail;
            tail = temp;
        }
        else if (nullptr ==  tail) {
            doublyList* temp =  new doublyList(d);
            tail = temp;
            head = temp;
        }
    }

    void insertAtPosition(doublyList* & tail, doublyList* &head, const int& position, const int& d) {
        
    }
};


int main() {
    doublyList* obj1 =  new doublyList(42);
    doublyList* head = obj1;
    doublyList* tail = obj1;
    obj1 -> printNode(obj1);
    obj1 -> printNode(head->mNext);
    obj1 -> printNode(tail->mNext);
    obj1 -> insertAtHead(tail, head, 62);
    obj1 -> printNode(head);
    obj1 -> insertAtTail(tail, head, 32);
    obj1 -> printNode(head);
    obj1 -> insertAtPosition(tail, head,2, 22);
    obj1 -> printNode(head);
    delete head;
    delete tail;
    //delete obj1;
    return (0);
}