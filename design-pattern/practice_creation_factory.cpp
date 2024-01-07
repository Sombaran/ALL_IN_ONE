/*
@Brief:
abstract class interface
concrete class implementing abstract interface methods

client code is not aware of the complexity in creating the object 
because the abstract interface does not implement how an object is created


 main -->> client code -->> factoryMethod(object created here) --> concrete class implementing methods in abstract interface.
@Sombaran: 
*/

#include <iostream>


class Toy
{
protected:
    std::string name;
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
        std::cout << "\t\t\tCar Labeled" << std::endl;
        name = "Car";
        price = 200;
    }
    void show()
    {
        std::cout << "\t\t\tCar name : " << name << std::endl;
        std::cout << "\t\t\tCar price : " << price << std::endl;
    }
};

/*
-----------------------------------
factory methods which are used to create object
-----------------------------------
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
        std::cout << "\t\tToyFactory Calls FactoryMethod to Create Toy" << std::endl;
        Toy *toy = this->FactoryMethod();
        std::cout << "\t\tToyFactory Creates Toy" << std::endl;
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
        std::cout << "\tClient Calls to ToyFactory to create Car Toy" << std::endl;
        break;
    default: break;
    }

    Toy *toy = toyFactory->CreateToy();
    

    std::cout << "\tClient: Showing Toy" << std::endl;
    toy->show();
    
    delete toy;
    delete toyFactory;
}

/**
 * Main calls to Client to create toy.
 * By passing info like,
 * It pass 1 for creating Car
 */
int main()
{

    std::cout << "Main Calls Client to Create Car" << std::endl;
    ClientCode(1);
    return 0;
}