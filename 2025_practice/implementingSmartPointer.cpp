/**
* @Brief: Implementing smart pointer
*/

#include <iostream>
#include <memory>


class implementSmartPointer {
    int *mData;
    public:
    implementSmartPointer(int* copyData = nullptr): mData {copyData} {
        std::cout << "I am constructor \n";
    }

    ~ implementSmartPointer() {
        delete mData;
        std::cout << "I am destrcutor \n";
        mData = nullptr;
    }
};


int main() {
    implementSmartPointer obj(new int (185525));
    return (0);
}
