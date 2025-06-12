#include <iostream>



struct singlyList
{
    /* data */
    int mData;
    singlyList* mNext;
    singlyList () {
        std::cout << __func__ << std::endl;
    }
    singlyList (int data)
    : mData {data}
    , mNext {nullptr} {
        std::cout << __func__ << std::endl;
    }

    virtual ~singlyList() {
        std::cout << __func__ << std::endl;
        if (mNext != nullptr) {
            delete mNext;
            mNext = nullptr;
        }
    }

    void printNode(singlyList* &head) {
        std::cout << __func__ << std::endl;
        if (nullptr == head) {
            std::cerr << "List is empty \n";
            return;
        }
        singlyList* temp =  head;
        
        while(temp != nullptr) {
            std::cout << temp -> mData << "->";
            temp = temp -> mNext;
        }
        std::cout << std::endl;
    }


    void insetAtHead(singlyList* &head, const int& d) {
        singlyList* temp = new singlyList(d);
        temp -> mNext =  head;
        head = temp;
    }

    void insetAtTail(singlyList* &tail, const int& d) {
        singlyList* temp = new singlyList(d);
        tail -> mNext = temp;
        tail = temp;
    }

    constexpr size_t lengthOfList(singlyList* &head) {
        size_t counter {};
        if (head == nullptr) {
            return (counter);
        }
        singlyList* temp = head;
        while(temp != nullptr) {
            //std::cout << temp -> mData << "->";
            ++counter;
            temp = temp -> mNext;
        }
        return (counter);
    }

    void insertAtPostion(singlyList* &head, const int& position, const int& d) {

        if (head == nullptr) {
            return;
        }
        if (position == 1) {
            insetAtHead(head, d);
            return;
        }
        singlyList * temp = head;
        int counter {1};

        while(counter < (position-1)) {
            temp =  temp -> mNext;
            ++counter;
        }

        singlyList* nodeToInsert =  new singlyList(d);
        nodeToInsert ->mNext = temp ->mNext;
        temp -> mNext = nodeToInsert;
        return;
    }

    void deleteNode(singlyList* &head, const int& position) {
        if (position > lengthOfList(head)) {
            std::cerr << "Invalid position \n";
        }

        if (position == 1) {
            singlyList* temp = head;
            head = temp -> mNext;
            temp -> mNext = nullptr;
            delete temp;
        }
        else {
            singlyList* current = head;
            singlyList* previous = nullptr;
            int counter {1};

            while(counter < position) {
                previous =  current;
                current =  current -> mNext;
                ++counter;
            }
            previous -> mNext = current -> mNext;
            current -> mNext = nullptr; 
            delete current;
        }
    }
};


int main() {
    //singlyList obj(32);
    singlyList* obj1 =  new singlyList(32);
    singlyList* head = obj1;
    singlyList* tail = obj1;
    obj1 -> printNode(obj1);
    obj1 -> printNode(head);
    obj1 -> printNode(tail);
    obj1 -> insetAtHead(head, 22);
    obj1 -> printNode(head); // 22->32->
    obj1 -> insetAtTail(tail, 42);
    obj1 -> printNode(head); // 22->32->42->
    size_t lengthLL = obj1 -> lengthOfList(head);
    std::cout << "length of linkedList " << lengthLL << std::endl;
    obj1 -> insertAtPostion(head, 2, 52);
    obj1 -> printNode(head); //22->52->32->->42->
    //obj1 -> deleteNode(head,9); // Invalid postion
    obj1 -> deleteNode(head,3); // delete head 
    obj1 -> printNode(head);
    delete head;
    return (0);
}

