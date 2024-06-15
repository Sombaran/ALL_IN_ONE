/*
@Brief: SOLID Principle
S --> A class should have only one reason to change 
      Single responsiblity principle
O --> A class should be open for extension but closed for modification. 
      OCP-->adding new functionality without modifying the existing code.
      Open-closed principle
L --> If B is sub-type of class A, then we should be able to replace the object 
      of A with B without breaking the behavior of the program in other words,
      a subclass should be able to be substituted for its parent class without 
      causing any errors or unexpected behavior.
      Liskov substitution principle
I --> Interface should be designed in such a way that client should not implement
      unneccesary methods which is not required.
      Interface segregation principle
D --> high-level modules should not depend on low-level modules
      Both should depend on abstractions.
      This principle promotes loose coupling and allows for easier changes and substitutions
      Dependency inversion principle
*/
#include <iostream>
#include <memory>

//Single responsiblity principle
class Marker {
    public:
    std::string m_color;
    std::string m_name;
    int m_year;
    int m_price;
    public:

    Marker (std::string color, std::string name, int year,int price) 
    : m_color (color)
    , m_name (name)
    , m_year (year)
    , m_price (price) {
        std::cout << "I am parameterized constructor for function " << __func__ << std::endl;
        std::cout << this->m_color << std::endl;
        std::cout << this->m_name << std::endl;
        std::cout << this->m_year << std::endl;
        std::cout << this->m_price << std::endl;
    }

    ~Marker() {
        std::cout << "I am destructor \n";
    }
};

class Invoice {
    Marker *_marker;
    int m_quantity;
    public:
    Invoice (Marker *marker, int quantity)
    : _marker (marker)
    , m_quantity (quantity) {
        std::cout << "I am parameterized constructor for function " << __func__ << std::endl;
    }
    /*
    ~Invoice () {
        std::cout << "I am destructor \n";
    }*/

    int calculatePrice (const Marker& obj) {
        int price = ((obj.m_price) * (m_quantity));
        return price;

    }
};

int main() {
    //std::shared_ptr <Marker>Obj = std::make_shared<Marker>("Blue","Parker", 2007, 100);
    Marker mObj("Purple","Parker", 2008, 10);
    Marker *vptrObj;
    Invoice invOb(vptrObj,1111);
    std::cout << "Price of quantity " << invOb.calculatePrice(mObj) << std::endl;
    //Marker * mObj = new Marker("Purple","Parker", 2008, 102);
    //Invoice inObj(mObj);
}
//Liskov substitution principle
class Bird {
    public:
    virtual void fly()=0;
    virtual ~Bird() = default;
};

class Eagle : public Bird {
    public:
    void fly() override {
        std::cout << __func__ << std::endl;
    }
};

void makeBirdFly(Bird& bird) {
  bird.fly();
}
int main2() {
    //std::shared_ptr<Bird>eagle = std::make_shared<Eagle>();
    Eagle eagle;
    makeBirdFly(eagle); // okay, eagle can fly
}

