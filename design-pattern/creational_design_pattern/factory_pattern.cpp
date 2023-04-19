//============================================================================
// Name        : factory_pattern.cpp
// Author      : Jotirling Swami
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

// 	 Factory Design Pattern :

//	 1. Factory Design is also known as the Virtual constructor
//
//	 Notes: We dont have the virtual constructor in C++ because :
// 		1. we need object to call the virtual function and if we want the virtual constructor then
// 			here object should call the constructor. but that facility is not there in C++.
// 		2. Suppose we want to create multiple instances of a similar kind and want to achieve loose
// 			coupling then you can go for Factory pattern.
// 		3. A class implementing factory design pattern works as a bridge between multiple classes.

//	 Notes :
//		 0. A factory pattern is one of the core design principles to create an object
//		 1. It is a creational design pattern which talks about the creation of an object.
//		 2. Factory method is a creational design pattern, i.e., related to object creation.
//		 3. In Factory pattern, we create objects without exposing the creation logic to the client and
//		 	 the client uses the same common interface to create a new type of object.

// 	 What is meant when we talk about factory method and clients?
// 	 	 1. A factory is something that produces toys that exposes some public APIs and
//	 	 2. clients make calls to those public APIs to produce toy.
//

// 	When to use
// 		1. a class can't anticipate the class of objects it must create.
// 		2. a class wants its subclasses to specify the objects it creates.
// 		3. classes delegate responsibility to one of several helper subclasses, and you want to localize the knowledge of which 		helper subclass is the delegate

// 	Benefits of Factory Pattern
// 		1. Factory design pattern provides an approach to code for interface rather than implementation.
// 		2. Factory pattern removes the instantiation of actual implementation classes from client code.
// 		3. Factory pattern makes our code more robust, less coupled, and easy to extend.
// 		4. Factory pattern provides abstraction between implementation and client classes through inheritance.

//	 Factory Pattern: follows below steps:
//		 1. Creates object through inheritance
//		 2. Produces only one product
//		 3. Implements code in the abstract creator that make use of the concrete type that sub class produces.

//	 Example Toy Factory :
//		 Toy factory is working on producing multiple toy like Car and Bike.
//		 But in future Factory want to produce the toy Plane in that case it affect code changes in client side.
//		 Using factory design patterns maintain loose coupling and easy implementation between the client and factory class.
// 		 we should go for the factory design pattern in order to achieve loose coupling
//		 The creation of a similar kind of object.

#include <iostream>
using namespace std;

class Toy
{
protected:
	string name;
	float price;

public:
	virtual void prepare() = 0;
	virtual void assemble() = 0;
	virtual void label() = 0;
	virtual void show() = 0;
	virtual ~Toy()
	{
	}
};

class Car : public Toy
{
public:
	void prepare()
	{
		std::cout << "	Car prepare " << std::endl;
	}
	void assemble()
	{
		std::cout << "	Car assemble " << std::endl;
	}
	void label()
	{
		cout << "	Car Label" << endl;
		name = "Car";
		price = 200;
	}
	void show()
	{
		std::cout << "	Car show : " << std::endl;
		std::cout << "		Car name : " << name << std::endl;
		std::cout << "		Car price : " << price << std::endl;
	}
};

class Bike : public Toy
{
public:
	void prepare()
	{
		std::cout << "	Bike prepare " << std::endl;
	}
	void assemble()
	{
		std::cout << "	Bike assemble " << std::endl;
	}
	void label()
	{
		cout << "	Bike Label" << endl;
		name = "Bike";
		price = 400;
	}
	void show()
	{
		std::cout << "Bike show : " << std::endl;
		std::cout << "		Bike name : " << name << std::endl;
		std::cout << "		Bike price : " << price << std::endl;
	}
};

class Plane : public Toy
{
public:
	void prepare()
	{
		std::cout << "	Plane prepare " << std::endl;
	}
	void assemble()
	{
		std::cout << "	Plane assemble " << std::endl;
	}
	void label()
	{
		cout << "	Plane Label" << endl;
		name = "Plane";
		price = 3000;
	}
	void show()
	{
		std::cout << "Plane show : " << std::endl;
		std::cout << "		Plane name : " << name << std::endl;
		std::cout << "		Plane price : " << price << std::endl;
	}
};

class ToyFactory
{
public:
	static Toy *createToy(int type)
	{

		Toy *toy = NULL;

		switch (type)
		{
		case 1:
		{
			toy = new Car;
			break;
		}
		case 2:
		{
			toy = new Bike;
			break;
		}
		case 3:
		{
			toy = new Plane;
			break;
		}
		default:
		{
			cout << "invalid toy type please re-enter type" << endl;
			return NULL;
		}
		}

		toy->prepare();
		toy->assemble();
		toy->label();
		return toy;
	}
};

int main()
{
	int type;
	while (true)
	{
		cout << "1. Car\t2. Bike\t3. Plane" << endl;
		cout << "Enter type :";
		cin >> type;

		Toy *toy = ToyFactory::createToy(type);
		if (toy)
		{
			toy->show();
			delete toy;
		}
		cout << endl;
	}
	cout << "Exit.." << endl;
	return 0;
}

// Abstract Factory Pattern vs Factory Method
// What's the difference between Abstract Factory Pattern and Factory Method?

//      Abstract Factory design pattern creates Factory.
//      Factory design pattern creates Products.
