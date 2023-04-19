// Subscripting [] Operator Overloading in C++
//     The subscript operator[] is normally used to access array elements.
//     This operator can be overloaded to enhance the existing functionality of C++ arrays.

#include <iostream>
using namespace std;
const int SIZE = 10;

class safearay
{
private:
    int Array[SIZE];

public:
    safearay()
    {
        for (int i = 0; i < SIZE; i++)
        {
            Array[i] = i;
        }
    }

    int &operator[](int index)
    {
        if (index > SIZE)
        {
            cout << "ERROR: OUT OF BOUND" << endl;
            return Array[0];
        }
        return Array[index];
    }

    void operator()()
    {
        for (int i = 0; i < SIZE; i++)
        {
            cout << Array[i] << " ";
        }
        cout << endl;
    }
};

int main()
{
    safearay A;
    A();
    cout << "Value of A[2] : " << A[2] << endl;
    cout << "Value of A[5] : " << A[5] << endl;
    cout << "Value of A[12] : " << A[12] << endl;

    return 0;
}