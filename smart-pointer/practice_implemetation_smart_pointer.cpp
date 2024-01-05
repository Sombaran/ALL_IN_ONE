#include <iostream>


class myClass {
    int *data;
    public:
    myClass(int *p=nullptr) 
    : data(p) {
        std::cout << "I am constructor \n";
    }

    ~myClass() {
        delete data;
        std::cout << "I am desctructor \n";
    }
};

int main() {
    //int *p = new int (19);
    //myClass myClassObj = myClass(p);
    myClass(new int (19));
    return (0);
}


/* output

19:11 $ g++ practice_implemetation_smart_pointer.cpp -o raw -std=gnu++11;  ./raw 
I am constructor 
I am desctructor 
*/