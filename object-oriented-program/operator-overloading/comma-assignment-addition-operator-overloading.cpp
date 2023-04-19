// In C++ assignment operator has high precedence over the comma operator

#include <iostream>
using namespace std;

class Numbers
{
    int value;

public:
    Numbers() : value(0)
    {
    }
    Numbers(int val) : value(val)
    {
    }
    Numbers operator+(const Numbers &obj)
    {
        return Numbers(this->value + obj.value);
        ;
    }

    Numbers operator,(const Numbers &obj)
    {
        this->value = obj.value;
        return Numbers(this->value);
    }

    Numbers operator=(const Numbers &obj)
    {
        this->value = obj.value;
        return Numbers(this->value);
    }

    void display()
    {
        cout << value << endl;
    }
};

int main()
{

    Numbers obj1(20), obj2(50), obj3(10), obj4;

    obj4 = obj1 + obj2, obj3;

    cout << "obj4 = obj1 + obj2, obj3" << endl;
    cout << "obj4 : ";
    obj4.display();

    obj4 = obj1, obj2, obj3;

    cout << "obj4 = obj1 , obj2, obj3" << endl;
    cout << "obj4 : ";
    obj4.display();

    obj4 = (obj1 + obj2, obj3);

    cout << "obj4 = (obj1 + obj2, obj3)" << endl;
    cout << "obj4 : ";
    obj4.display();

    obj4 = (obj1, obj2, obj3);
    cout << "obj4 = (obj1 , obj2, obj3)" << endl;
    cout << "obj4 : ";
    obj4.display();

    return 0;
}

//	Output:
//		obj4 = obj1 + obj2, obj3
//		obj4 : 70
//		obj4 = obj1 , obj2, obj3
//		obj4 : 20
//		obj4 = (obj1 + obj2, obj3)
//		obj4 : 10
//		obj4 = (obj1 , obj2, obj3)
//		obj4 : 10
