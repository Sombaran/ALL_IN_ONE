/**
 * @Brief: Function Overloading provides multiple definitions of the function by changing signature i.e. changing number of parameters, change datatype of parameters, return type doesn’t play any role. 
 */

#include <iostream>
#include <memory>


class Overloading {
    public:
    Overloading() = default;
    Overloading(const Overloading&) = delete;
    Overloading(const Overloading&&) = delete;
    Overloading& operator = (const Overloading&) = delete;
    void calculateArea(int&  p1) {
        std::cout << "Overloading with one integer parameter \n";
    }
    void calculateArea(int& p1, int& p2) {
        std::cout << "Overloading with two integer parameter \n";
    }
    void calculateArea(float& p1) {
        std::cout << "Overloading with one float parameter \n";
    }
    virtual void display()  {
        std::cout << "I am display in Overloading function\n";
    }
    virtual ~Overloading() = default;
};

class Overiding : public Overloading {
    public:
    void display() override {
        std::cout << "I am display in Overiding function\n";
    }
};


int main() {
    int a = 5;
    float b = 5.5;
    std::shared_ptr<Overloading> overloadingObj{ std::make_shared<Overiding> ()};
    overloadingObj->display();
    overloadingObj->calculateArea(b);
    overloadingObj->calculateArea(a,a);
    overloadingObj->calculateArea(a);
    return(0);
}
