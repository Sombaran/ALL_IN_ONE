//  Input/Output Operators Overloading in C++
//         C++ is able to input and output the built-in data types using the
//             stream extraction operator '>>'
//             stream insertion operator '<<'
//         The stream insertion and stream extraction operators also can be overloaded
//         to perform input and output for user-defined types like an object.

//         Here, it is important to make operator overloading function a friend of the class
//         because it would be called without creating an object.

//         Following example explains how extraction operator >> and insertion operator <<.
#include <iostream>
using namespace std;

class Distance
{
private:
    int value; // 0 to infinite

public:
    // required constructors
    Distance()
    {
        value = 0;
    }
    Distance(int val)
    {
        value = val;
    }
    friend ostream &operator<<(ostream &output, const Distance &Dist)
    {
        output << "value : " << Dist.value;
        return output;
    }

    friend istream &operator>>(istream &input, Distance &Dist)
    {
        input >> Dist.value;
        return input;
    }
};

int main()
{
    Distance D1(11), D2(5), D3;
    cout << "First Distance : " << D1 << endl;
    cout << "Second Distance :" << D2 << endl;
    cout << "Enter the value of object : " << endl;
    cin >> D3;
    cout << "Third Distance :" << D3 << endl;

    return 0;
}