// Function Call Operator () Overloading in C++ :
// Parenthesis Operator () Overloading in C++ :

//         The function call operator () can be overloaded for objects of class type.
//         When you overload ( ), you are not creating a new way to call a function.
//         Rather, you are creating an operator function that can be passed an
//         arbitrary number of parameters.

#include <iostream>
using namespace std;

class Distance
{
private:
    int feet;   // 0 to infinite
    int inches; // 0 to 12

public:
    // required constructors
    Distance()
    {
        feet = 0;
        inches = 0;
    }
    Distance(int f, int i)
    {
        feet = f;
        inches = i;
    }

    // overload function call
    Distance operator()(int a, int b, int c)
    {
        cout << "operator()" << endl;
        Distance D;
        // just put random calculation
        D.feet = a + c + 10;
        D.inches = b + c + 100;
        return D;
    }

    // method to display distance
    void display()
    {
        cout << "Feet: " << feet << " Inch:" << inches << endl;
    }
};

int main()
{
    Distance D1(11, 10);

    cout << "First Distance : ";
    D1.display();

    Distance D2 = D1(10, 10, 10); // invoke operator()
    cout << "Second Distance :";
    D2.display();

    Distance D3;
    D3 = D1(10, 10, 10); // invoke operator()
    cout << "Third Distance :";
    D2.display();

    return 0;
}