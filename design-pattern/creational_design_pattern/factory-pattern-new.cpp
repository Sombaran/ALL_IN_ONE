//============================================================================
// Name        : factory_pattern.cpp
// Author      : Jotirling Swami
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

/**
 * The Toy interface declares the operations that all concrete products must
 * implement.
 */
#include <iostream>
using namespace std;

// Interface Class
class Toy
{
protected:
    string name;
    float price;

public:
    virtual ~Toy()
    {
    }
    virtual void prepare() = 0;
    virtual void assemble() = 0;
    virtual void label() = 0;
    virtual void show() = 0;
};

/**
 * Concrete Class Car
 * Concrete Class Car provide various implementations of the Toy interface.
 */
class ConcreteCar : public Toy
{
public:
    void prepare()
    {
        std::cout << "\t\t\tCar prepared " << std::endl;
    }
    void assemble()
    {
        std::cout << "\t\t\tCar assembled " << std::endl;
    }
    void label()
    {
        cout << "\t\t\tCar Labeled" << endl;
        name = "Car";
        price = 200;
    }
    void show()
    {
        std::cout << "\t\t\tCar name : " << name << std::endl;
        std::cout << "\t\t\tCar price : " << price << std::endl;
    }
};

/**
 * Concrete Class Bike
 * Concrete Class Bike provide various implementations of the Toy interface.
 */
class ConcreteBike : public Toy
{
public:
    void prepare()
    {
        std::cout << "\t\t\tBike prepared " << std::endl;
    }
    void assemble()
    {
        std::cout << "\t\t\tBike assembled " << std::endl;
    }
    void label()
    {
        cout << "\t\t\tBike Labeled" << endl;
        name = "Bike";
        price = 400;
    }
    void show()
    {
        std::cout << "\t\t\tBike name : " << name << std::endl;
        std::cout << "\t\t\tBike price : " << price << std::endl;
    }
};

/**
 * Concrete Class Plane
 * Concrete Class Plane provide various implementations of the Toy interface.
 */
class ConcretePlane : public Toy
{
public:
    void prepare()
    {
        std::cout << "\t\t\tPlane prepared " << std::endl;
    }
    void assemble()
    {
        std::cout << "\t\t\tPlane assembled " << std::endl;
    }
    void label()
    {
        cout << "\t\t\tPlane Labeled" << endl;
        name = "Plane";
        price = 3000;
    }
    void show()
    {
        std::cout << "\t\t\tPlane name : " << name << std::endl;
        std::cout << "\t\t\tPlane price : " << price << std::endl;
    }
};

/**
 * The ToyFactory class declares the virtual function FactoryMethod
 * The Toy object is created using the FactoryMethod.
 */

class ToyFactory
{
public:
    virtual ~ToyFactory()
    {
    }

    virtual Toy *FactoryMethod() const = 0;

    Toy *CreateToy() const
    {
        std::cout << "\t\tToyFactory Calls FactoryMethod to Create Toy" << endl;
        Toy *toy = this->FactoryMethod();
        cout << "\t\tToyFactory Creates Toy" << endl;
        toy->prepare();
        toy->assemble();
        toy->label();
        return toy;
    }
};

/**
 * ConcreteToyFactoryCar creates the object of the ConcreteCar
 * FactoryMethod return the created object.
 */
class ConcreteToyFactoryCar : public ToyFactory
{

public:
    Toy *FactoryMethod() const override
    {
        return new ConcreteCar();
    }
};
/**
 * ConcreteToyFactoryBike creates the object of the ConcreteBike
 * FactoryMethod return the created object.
 */
class ConcreteToyFactoryBike : public ToyFactory
{
public:
    Toy *FactoryMethod() const override
    {
        return new ConcreteBike();
    }
};
/**
 * ConcreteToyFactoryPlane creates the object of the ConcretePlane
 * FactoryMethod return the created object.
 */
class ConcreteToyFactoryPlane : public ToyFactory
{
public:
    Toy *FactoryMethod() const override
    {
        return new ConcretePlane();
    }
};

/**
 * Client calls to ToyFactory to create toy.
 * It create respective concretToyFactory object
 * As per main requested
 */

void ClientCode(int toyType)
{

    ToyFactory *toyFactory;
    switch (toyType)
    {
    case 1:
        toyFactory = new ConcreteToyFactoryCar();
        std::cout << "\tClient Calls to ToyFactory to create Car Toy" << endl;
        break;
    case 2:
        toyFactory = new ConcreteToyFactoryBike();
        std::cout << "\tClient Calls to ToyFactory to create Bike Toy" << endl;
        break;
    case 3:
        toyFactory = new ConcreteToyFactoryPlane();
        std::cout << "\tClient Calls to ToyFactory to create Plane Toy" << endl;
        break;
    }

    Toy *toy = toyFactory->CreateToy();
    

    std::cout << "\tClient: Showing Toy" << endl;
    toy->show();
    
    delete toy;
    delete toyFactory;
}

/**
 * Main calls to Client to create toy.
 * By passing info like,
 * It pass 1 for creating Car
 * It pass 2 for creating Bike
 * It pass 3 for creating Plane
 */
int main()
{

    std::cout << "Main Calls Client to Create Car" << endl;
    ClientCode(1);

    std::cout << "Main Calls Client to Create Bike" << endl;
    ClientCode(2);

    std::cout << "Main Calls Client to Create Plane" << endl;
    ClientCode(3);

    return 0;
}

// Abstract Factory Pattern vs Factory Method
// What's the difference between Abstract Factory Pattern and Factory Method?

//      Abstract Factory design pattern creates Factories of toy.
//      Factory design pattern creates Toys.
