
/**
 * Implemting factory design pattern
 * It is a type of Creational Pattern also known as virtual constructor
 * https://refactoring.guru/design-patterns/factory-method
 * */


#include <iostream>

class Toy {
protected:
	std::string mName;
	int 		mPrice;
public:
	//Toy();
	virtual void applyLable() = 0;
	virtual void printName()  = 0;
};

class car : public Toy {
public:
	//car ();
	void applyLable() { mName = "alto "; mPrice = 500000;}
	void printName() { std::cout << "Car name " << mName << " Car price " << mPrice  << std::endl;}
	~car();
};

class toyfactory {
public:
	static Toy* toyfactoryMethod(int type) { 
		Toy* toy = NULL;
		if (type==1) {
			toy = new car;
			//std::shared_ptr<Toy> toy = new car;
		}

		toy->applyLable();
		return (toy);
	}
};



int main () {
	int type;
	while(true) {
		std::cout << "Enter type \n";
		std::cin >> type;
		//std::shared_ptr <Toy> v;
		Toy* v = toyfactory::toyfactoryMethod(type);
		if (nullptr != v) {
			v->printName();
			//delete v;
		}

	}
	std::cout << std::endl;
	return (0);
};
