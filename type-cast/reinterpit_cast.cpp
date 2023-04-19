//============================================================================
// Name        : reinterpret_cast.cpp
// Author      : Jotirling Swami
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

//		*** reinterpret_cast ***
// 	Notes:
//		1. It can perform dangerous conversion because it tan typecast any pointer to any other pointer.
//		2. reinterpret_cast is a type of casting operator used in C++.
//		3. It is used to convert a pointer of some data type into a pointer of another data type,
//			even if the the data types before and after conversion are different.
//		4. It does not check if the pointer type and data pointed by the pointer is same or not.
//		5. Syntax :
//		 	 	 data_type *var_name = reinterpret_cast <data_type *>(pointer_variable);
//		6. Return Type
//		 	 It doesnt have any return type. It simply converts the pointer type.
//		7. Parameters
//			It takes only one parameter i.e., the source pointer variable (p in above example).
// 	BOTTOM LINE:
// 		1. 	The reinterpret_cast cannot safely be used for anything other than being cast back to its original type.
// 		2. 	we should be very careful when using this cast.
// 		3. 	if we use this type of cast then it becomes non-portable product.



#include <iostream>
using namespace std;

class Mango {
public:
	void eatMango() {
		cout << "eating Mango" << endl;
	}
};
class Banana {
public:
	void eatBanana() {
		cout << "eating Banana" << endl;
	}
};

struct info {
	int ivalue1;
	int ivalue2;
	char cvalue;
	bool bvalue;
};

int main() {

	Banana *bfruit = new Banana();
	Mango *mfruit = new Mango();

	Banana *newbanana = reinterpret_cast<Banana*>(mfruit);
	newbanana->eatBanana();		//eating Banana

	Mango *newmango = reinterpret_cast<Mango*>(bfruit);
	newmango->eatMango();		//eating Mango

	struct info Infostrcut = { 10, 300, 'A', true };

	int *i = reinterpret_cast<int*>(&Infostrcut);
	cout << "ivalue1: " << *i << endl; // ivalue1: 10

	i++;

	int *d = reinterpret_cast<int*>(i);
	cout << "ivalue2: " << *d << endl; // ivalue2: 300

	d++;

	char *c = reinterpret_cast<char*>(d);
	cout << "cvalue: " << *c << endl; // cvalue: A

	c++;

	bool *b = reinterpret_cast<bool*>(c);
	cout << "bvalue: " << boolalpha << *b << endl; // bvalue: true

	return 0;
}

