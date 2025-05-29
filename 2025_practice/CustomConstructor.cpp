

#include <iostream>
#include <cstring>

class String {
    int mLength;
    int *mPointer;
    public:
    String() {
        std::cout << "String()::constructor \n";
    }

    String (int d1, int d2)
    : mLength {d1}
    , mPointer {new int (d2)}
    {
        std::cout << "String(int d1)::constructor \n";
    }

    String (const String& obj)
    : mLength {obj.mLength}
    , mPointer {new int (*obj.mPointer)} {
        std::cout << "String (const String& obj)::constructor \n";
    }

    String& operator = (const String& obj) {
        std::cout << "String& operator = (const String& obj)::constructor \n";
        if (this != &obj) {
            mLength     = obj.mLength;
            *mPointer   = *obj.mPointer; 
        }
        return *this;
    }

    void display () {
        std::cout << __func__ << " " << mLength << " " << mPointer << " " << *mPointer << std::endl;
    }
};

int main() {
    String s1,s2(32,42);
    String s3(s2);  // copy constructor
    //s1.display();
    s2.display();
    s3.display();
    s3 = s1;        // assignment operator
    s3.display();
    String s4(s2);  // copy constructor
    s4.display();
    s4 = s2;        // assignment operator
    s4.display();
    return (0);
}