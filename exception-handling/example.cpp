#include <iostream>
#include <vector>
using namespace std;

//----------------------------------------------------------------------------------------------

class newException : public exception
{
    virtual const char *what() const throw()
    {
        return "newException occurred";
    }
} newex;

int example3()
{

    try
    {
        throw newex;
    }
    catch (exception &ex)
    {
        cout << ex.what() << '\n';
    }
    return 0;
}

//----------------------------------------------------------------------------------------------

double zeroDivision(double x, int y)
{

    if (y == 0)
    {
        throw "Division by Zero!";
    }
    return (x / y);
}

int example2()
{
    double x = 11;
    int y = 0;
    double c = 0;

    try
    {
        c = zeroDivision(x, y);
        cout << c << endl;
    }
    catch (const char *message)
    {
        cerr << message << endl;
    }
    return 0;
}

//----------------------------------------------------------------------------------------------

int example1()
{
    vector<int> vec;
    vec.push_back(0);
    vec.push_back(1);

    try
    {
        vec.at(2); // access the third element, which doesn't exist
    }
    catch (exception &ex)
    {
        cout << "Exception occurred!" << endl;
    }
    return 0;
}
//----------------------------------------------------------------------------------------------
int main()
{
    example1();
    example2();
    example3();
    return 0;
}
//----------------------------------------------------------------------------------------------