//  Comparison Operator Overloading  / Relational Operators Overloading
//     0. Comparison or Binary operator required two operands to perform operation.
//     1. comparison operators are all binary operators that do not modify their left operands.
//     2. Comparison operators are binary operators that test a condition and
//     3. return 1 if that condition is logically true and 0 if that condition is false

// comparison
//     a == b
//     a != b
//     a < b
//     a > b
//     a <= b
//     a >= b

//    Implementation :
//      Class will have one data member and we create 2 object of the class in main obj1 and obj2
//      After calling the obj1 > obj2 it should return the bool value true or false;

#include <iostream>
using namespace std;

class Number
{
private:
    int variable;

public:
    Number(int variable)
        : variable{variable}
    {
        cout << variable << endl;
    }

    // if you define any Operator overloading in class and also friend function
    // then By default class calls the class operator overloading
    // it will not call the friend class operator.

    friend bool operator==(const Number &obj1, const Number &obj2);

    bool operator==(const Number &obj)
    {
        cout << "class operator== " << endl;
        return this->variable == obj.variable;
    }

    bool operator!=(const Number &obj)
    {
        return this->variable != obj.variable;
    }

    bool operator<(const Number &obj)
    {
        return this->variable < obj.variable;
    }

    bool operator>(const Number &obj)
    {
        return this->variable > obj.variable;
    }

    bool operator<=(const Number &obj)
    {
        return this->variable <= obj.variable;
    }

    bool operator>=(const Number &obj)
    {
        return this->variable >= obj.variable;
    }
};

bool operator==(const Number &obj1, const Number &obj2)
{
    cout << "friend operator== " << endl;
    return obj1.variable == obj2.variable;
}

int main()
{
    cout << "Priya: ";
    Number Priya(100);
    cout << "Joti: ";
    Number Joti(100);

    if (Joti == Priya)
        std::cout << "a Priya is equal to a Joti - true \n";
    else
        std::cout << "a Priya is equal to a Joti - false \n";

    if (Joti != Priya)
        std::cout << "a Priya is not equal to a Joti - true \n";
    else
        std::cout << "a Priya is not equal to a Joti - false \n";

    if (Joti < Priya)
        std::cout << "a Priya is greater than a Joti - true \n";
    else
        std::cout << "a Priya is greater than a Joti - false \n";

    if (Joti > Priya)
        std::cout << "a Joti is greater than a Priya - true \n";
    else
        std::cout << "a Joti is greater than a Priya - false \n";

    if (Joti <= Priya)
        std::cout << "a Priya is greater than or equal to a Joti - true \n";
    else
        std::cout << "a Priya is greater than or equal to a Joti - false \n";

    if (Joti >= Priya)
        std::cout << "a Joti is greater than or equal to a Priya - true \n";
    else
        std::cout << "a Joti is greater than or equal to a Priya - false \n";
    return 0;
}
