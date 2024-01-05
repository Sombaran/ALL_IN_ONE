/*
@Brief: explicit keyword to the constructor prevents the compiler 
from using that constructor for implicit conversions
*/
#include <iostream>
#include <memory>

class myClass {
    int mVar;
    public:
    myClass() {
        std::cout << "I am default constructor \n";
    }

    explicit myClass(int someVar) 
    : mVar (someVar) {
        std::cout << "I am parameterized constructor \n";
    }

    void print() {
        std::cout << this->mVar << std::endl;
    }
    ~myClass () {
        std::cout << "I am destructor \n";
    }
};

void func(myClass obj) {
    obj.print();
}

int main() {
    std::shared_ptr<myClass> obj = std::make_shared<myClass> ();
    std::shared_ptr<myClass> obj1 = std::make_shared<myClass> (14);
    obj->print();
    func(16); // implicit call
    return (0);
}