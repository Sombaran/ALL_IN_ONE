
//  Arithmetic operators

//     Operator Name           Syntex
//     unary plus              +a
//     unary minus             -a

//     bitwise NOT             ~a
//     bitwise AND             a & b
//     bitwise OR              a | b
//     bitwise XOR             a ^ b
//     bitwise left shift      a << b
//     bitwise right shift     a >> b

//     addition                a + b
//     subtraction             a - b
//     multiplication          a * b
//     division                a / b
//     modulo                  a % b

#include <iostream>
using namespace std;

class Numbers
{
    int value;

public:
    Numbers() : value(0) {}
    Numbers(int num) : value(num) {}
    void setValue(int val);
    void print();
    Numbers operator+(const Numbers &obj); // addition          a + b
    Numbers operator-(const Numbers &obj); // substraction      a + b
    Numbers operator*(const Numbers &obj); // multiplication    a * b
    Numbers operator/(const Numbers &obj); // division          a / b
    Numbers operator%(const Numbers &obj); // modulus           a % b
};

Numbers Numbers::operator+(const Numbers &obj)
{
    cout << "object1: " << this->value << " + object2: " << obj.value << endl;
    return Numbers(this->value + obj.value);
}
Numbers Numbers::operator-(const Numbers &obj)
{
    cout << "object1: " << this->value << " - object2: " << obj.value << endl;
    return Numbers(this->value - obj.value);
}
Numbers Numbers::operator*(const Numbers &obj)
{
    cout << "object1: " << this->value << " * object2: " << obj.value << endl;
    return Numbers(this->value * obj.value);
}

Numbers Numbers::operator/(const Numbers &obj)
{
    cout << "object1: " << this->value << " / object2: " << obj.value << endl;
    return Numbers(this->value / obj.value);
}

Numbers Numbers::operator%(const Numbers &obj)
{
    cout << "object1: " << this->value << " % object2: " << obj.value << endl;
    return Numbers(this->value % obj.value);
}

void Numbers::setValue(int val)
{
    value = val;
}
void Numbers::print()
{
    cout << value << endl;
}

int main()
{
    Numbers object1(45);
    Numbers object2(30);
    //---------------------------------------------------------------------------------------
    cout << "addition of the object3 = object1 + object1" << endl;
    Numbers object3 = object1 + object2;
    cout << "object3: "; // object3: 75
    object3.print();

    cout << "addition of the object4 = object1 + object2 + object3" << endl;
    Numbers object4 = object1 + object2 + object3;
    cout << "object4: "; // object4: 150
    object4.print();
    //---------------------------------------------------------------------------------------
    object1.setValue(8);
    object2.setValue(3);
    cout << "addition of the object3 = object1 - object1" << endl;
    object3 = object1 - object2;
    cout << "object3: "; // object3: 15
    object3.print();
    //---------------------------------------------------------------------------------------
    object1.setValue(2);
    object2.setValue(6);
    cout << "addition of the object3 = object1 * object1" << endl;
    object3 = object1 * object2;
    cout << "object3: "; // object3: 15
    object3.print();
    //---------------------------------------------------------------------------------------
    object1.setValue(8);
    object2.setValue(2);
    cout << "addition of the object3 = object1 / object1" << endl;
    object3 = object1 / object2;
    cout << "object3: "; // object3: 15
    object3.print();
    //---------------------------------------------------------------------------------------
    object1.setValue(10);
    object2.setValue(3);
    cout << "addition of the object3 = object1 % object1" << endl;
    object3 = object1 % object2;
    cout << "object3: "; // object3: 15
    object3.print();
    return 0;
}