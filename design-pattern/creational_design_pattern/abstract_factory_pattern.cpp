//============================================================================
// Name        : abstract_factory_pattern.cpp
// Author      : Jotirling Swami
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

// *** Abstract Factory design ***
//
//	Abstract Factory Pattern uses a super factory and creates other factories.
//		1. It is also known as a factory of factories.
//		2. It Defines an abstract class for creating families of related objects but 
//			without specifying their concrete sub-class.
//		3. This pattern is one of the creational pattern.
//		4. Abstract Factory pattern is almost similar to Factory Pattern is considered 
//			as another layer of abstraction over factory pattern.
//		5. Abstract Factory patterns work around a super-factory which creates other factories.
//		6. Abstract factory pattern implementation provides us with a framework that 
//			allows us to create objects that follow a general pattern.
//		7. So at runtime, the abstract factory is coupled with any desired concrete factory 
//			which can create objects of the desired type.

// 	Abstract Factory Pattern:
// 		1. Createobject through composition
// 		2. Produce families ofproducts
// 		3. Concretefactories implements factory method to create product

// 	Abstract Factory Design Pattern Benefits
// 		1. Abstract Factory design pattern provides an approach to code for interface rather than implementation.
// 		2. Abstract Factory pattern is "factory of factories" and can be easily extended to accommodate more products,
// 		3. Abstract Factory pattern is robust and avoids the conditional logic of the Factory pattern.

// 	When to use?
//      0. Show interface not implementation
// 		1. a system should be independent of how its products are created, composed, and represented
// 		2. a system should be configured with one of multiple families of products
// 		3. a family of related product objects is designed to be used together
// 		4. you want to provide a class library of products, and you want to reveal just their interfaces,
// 			not their implementations

// *** Following Concept Used in Abstract Factory Design ***
// 	AbstractFactory:
// 		Declares an interface for operations that create abstract product objects.

// 	ConcreteFactory:
// 		Implements the operations declared in the AbstractFactory to create concrete product objects.

// 	Product:
// 		Defines a product object to be created by the corresponding concrete factory and implements
// 		the AbstractProduct interface.

// 	Client:
// 		Uses only interfaces declared by AbstractFactory and AbstractProduct classes.

#include <iostream>
using namespace std;

class Tire
{
protected:
	string name;
	int presure;

public:
	Tire(string n, int presure) : name(n), presure(presure){};
	string getName()
	{
		return name;
	}
	int getPresure()
	{
		return presure;
	}
};

class SimpleTire : public Tire
{
public:
	SimpleTire() : Tire("SimpleTire", 75)
	{
	}
};

class LuxuryTire : public Tire
{
public:
	LuxuryTire() : Tire("LuxuryTire", 100)
	{
	}
};

class Body
{
protected:
	string name;
	int strength;

public:
	Body(string n, int strength) : name(n), strength(strength)
	{
	}
	string getName()
	{
		return name;
	}
	int getStrength()
	{
		return strength;
	}
};

class SimpleBody : public Body
{
public:
	SimpleBody() : Body("SimpleBody", 75)
	{
	}
};

class LuxuryBody : public Body
{
public:
	LuxuryBody() : Body("LuxuryBody", 100)
	{
	}
};

class Car
{
protected:
	string name;
	Tire *tire;
	Body *body;

public:
	Car(string n) : name(n)
	{
		tire = NULL;
		body = NULL;
	}
	void setTire(Tire *t)
	{
		tire = t;
	}
	void setBody(Body *b)
	{
		body = b;
	}
	void printDetails()
	{
		cout << endl
			 << "Car: " << name << endl;
		cout << "Tire: " << tire->getName() << " Preasure: "
			 << tire->getPresure() << endl;
		cout << "Body: " << body->getName() << " Strength: "
			 << body->getStrength() << endl
			 << endl;
	}
};

class CarFactory
{
private:
	Car *car;

protected:
	virtual Tire *buildTire() = 0;
	virtual Body *buildBody() = 0;

public:
	virtual Car *buildWholeCar() = 0;
	virtual ~CarFactory()
	{
	}
};

class SimpleCarFactory : public CarFactory
{
	Tire *buildTire()
	{
		return new SimpleTire();
	}
	Body *buildBody()
	{
		return new SimpleBody();
	}

	Car *buildWholeCar()
	{
		Car *car = new Car("SimpleCar");
		car->setTire(buildTire());
		car->setBody(buildBody());
		return car;
	}
};

class LuxuryCarFactory : public CarFactory
{
	Tire *buildTire()
	{
		return new LuxuryTire();
	}
	Body *buildBody()
	{
		return new LuxuryBody();
	}
	Car *buildWholeCar()
	{
		Car *car = new Car("LuxuryCar");
		car->setTire(buildTire());
		car->setBody(buildBody());
		return car;
	}
};

#define SIMPLE_CAR 1
//#define LUXURY_CAR 1

int main()
{

#ifdef SIMPLE_CAR
	CarFactory *factory = new SimpleCarFactory;

#elif LUXURY_CAR
	CarFactory *factory = new LuxuryCarFactory;

#endif

	Car *car = factory->buildWholeCar();
	car->printDetails();

	return 0;
}
