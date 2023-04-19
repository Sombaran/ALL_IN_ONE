// // Overloading the Comma Operator in C++
//     In C++, we can overload the comma operator using Operator Overloading.
//     For Example: For "Send the query X to the server Y and put the result in variable Z".
//     The "and" plays the role of the comma.
//     The comma operator (, ) is used to isolate two or more expressions
//     That are included where only one expression is expected.
//     When the set of expressions has to be solved for operands, only the rightmost expression is considered.

//     Examples:
//         Input: x = (y = 5, y + 2)
//         Output: x = 7, y = 5
//     Explanation:
//         In the above expression:
//         It would first assign the value 5 to y, and then assign y + 2 to variable x.
//         So, at the end ‘x’ would contain the value 7 while variable ‘y’ would contain value 7.

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
    Numbers operator,(const Numbers &obj)
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

    int a = 20, b = 50, c = 10, d = 0;
    d = a, b, c;
    // the above line is evaluated as: (((a = 2), 3), 4);
    cout << "d = a, b, c    => " << d << endl;
    d = (a, b, c);
    cout << "d = (a, b, c)  => " << d << endl;

    Numbers obj1(20), obj2(50), obj3(10), obj4;

    obj4 = obj1, obj2, obj3;

    cout << "obj4 = obj1 , obj2, obj3" << endl;
    cout << "obj4 : ";
    obj4.display();
    // output is 50
    // but the expected output is 20
    // (TODO): I Think its because of the compiler given default assignment operator

    obj4 = (obj1, obj2, obj3);
    cout << "obj4 = (obj1 , obj2, obj3)" << endl;
    cout << "obj4 : ";
    obj4.display();
    return 0;
}

//	Output:
// obj4 = obj1 , obj2, obj3
// obj4 : 50
// obj4 = (obj1 , obj2, obj3)
// obj4 : 10
