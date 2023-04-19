// Decorator Design Pattern

// The decorator pattern is used to add new functionality to an existing object without changing its structure.

#include <iostream>
using namespace std;

class ICar
{
public:
    virtual int getPrice() = 0;
};

class Sedan : public ICar
{
public:
    int getPrice()
    {
        return 1000;
    }
};

class SUV : public ICar
{
public:
    int getPrice()
    {
        return 2000;
    }
};

class CarDecorator : public ICar
{
protected:
    ICar *icar; // composition
    CarDecorator(ICar *icar_) : icar(icar_)
    {
    }

public:
    virtual int getPrice()
    {
        return icar->getPrice();
    }
};

class AlloyWheelsCarDecorator : public CarDecorator
{
public:
    AlloyWheelsCarDecorator(ICar *icar) : CarDecorator(icar)
    {
    }
    int getPrice()
    {
        return icar->getPrice() + 100;
    }
};

class SunRoofCarDecorator : public CarDecorator
{
public:
    SunRoofCarDecorator(ICar *icar) : CarDecorator(icar)
    {
    }
    int getPrice()
    {
        return icar->getPrice() + 200;
    }
};

class LeatherWorkCarDecorator : public CarDecorator
{
public:
    LeatherWorkCarDecorator(ICar *icar) : CarDecorator(icar)
    {
    }
    int getPrice()
    {
        return icar->getPrice() + 300;
    }
};

int main()
{
    ICar *sedanCar = new Sedan;
    sedanCar = new AlloyWheelsCarDecorator(sedanCar);
    cout << "sedanCar AlloyWheelsCarDecorator: " << sedanCar->getPrice() << endl;
    sedanCar = new LeatherWorkCarDecorator(sedanCar);
    cout << "sedanCar AlloyWheelsCarDecorator + LeatherWorkCarDecorator: " << sedanCar->getPrice() << endl;
    sedanCar = new SunRoofCarDecorator(sedanCar);
    cout << "sedanCarAlloyWheelsCarDecorator + LeatherWorkCarDecorator + SunRoofCarDecorator: " << sedanCar->getPrice() << endl;
    cout << endl;

    ICar *SUVCar = new SUV;
    SUVCar = new AlloyWheelsCarDecorator(SUVCar);
    cout << "SUVCar AlloyWheelsCarDecorator: " << SUVCar->getPrice() << endl;
    SUVCar = new LeatherWorkCarDecorator(SUVCar);
    cout << "SUVCar AlloyWheelsCarDecorator + LeatherWorkCarDecorator: " << SUVCar->getPrice() << endl;
    SUVCar = new SunRoofCarDecorator(SUVCar);
    cout << "SUVCar AlloyWheelsCarDecorator + LeatherWorkCarDecorator + SunRoofCarDecorator: " << SUVCar->getPrice() << endl;
}