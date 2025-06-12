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

    constexpr size_t lengthOfList(doublyList* &head) {
        size_t counter {};
        if (head == nullptr) {
            return (counter);
        }
        doublyList* temp = head;
        while(temp != nullptr) {
            //std::cout << temp -> mData << "->";
            ++counter;
            temp = temp -> mNext;
        }
        return (counter);
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
        if (tail == nullptr or head ==  nullptr) {
            return;
        }
        if (position == 1) {
            insertAtHead(tail, head, d);
        }
        doublyList * temp = head;
        int counter {1};

        while(counter < (position-1)) {
            temp =  temp -> mNext;
            ++counter;
        }

        doublyList* nodeToInsert =  new doublyList(d);
        nodeToInsert -> mNext =  temp -> mNext;
        temp -> mNext = nodeToInsert;
        temp -> mNext ->mPrev =  nodeToInsert;
        nodeToInsert -> mPrev = temp;
    }

    void deleteNode(doublyList* & head, const int& position) {
        if (position > lengthOfList(head)) {
            std::cerr << "Invalid position \n";
        }

        if (position == 1) {
            doublyList* temp = head;
            temp -> mNext -> mPrev = nullptr;
            head = temp -> mNext;
            temp -> mNext = nullptr;
            delete temp;
        }
        else {
            doublyList* current =  head;
            doublyList* previous = nullptr;
            int counter {1};

            while(counter < position) {
                previous =  current;
                current =  current -> mNext;
                ++counter;
            }
            current -> mPrev = nullptr;
            previous -> mNext =  current -> mNext;
            current -> mNext = nullptr;
            delete current;
        }
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
    obj1 -> deleteNode(head,2);
    obj1 -> printNode(head);
    delete head;
    delete tail;
    //delete obj1;
    return (0);
}