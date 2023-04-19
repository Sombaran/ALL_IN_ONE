// Unary Operator Overloading:
//     Positive (+) -  unary plus
//     Negative (-) -  unary minus
//     Logical not (!)

//     These operators all are unary operators.
//     Which means they only operate on one operand.
//     Because they only operate on the object they are applied to.
//     Typically unary operator overloads are implemented as member functions.
//     All three operands are implemented in an identical manner.

//     Overloading Unary Operator:
//         Let us consider to overload (-) unary operator.
//         In unary operator function, no arguments should be passed.
//         It works only with one class objects.
//         It is a overloading of an operator operating on a single operand.

#include <iostream>
using namespace std;

class Point
{
private:
    double m_x{};
    double m_y{};
    double m_z{};

public:
    Point() {}
    Point(double x = 0.0, double y = 0.0, double z = 0.0) : m_x{x}, m_y{y}, m_z{z}
    {
    }
    // Convert a Point into its negative equivalent
    Point operator-() const;

    // Return true if the point is set at the origin
    bool operator!() const;

    // print all the elements
    void print();
};

// Convert a Point into its negative equivalent
Point Point::operator-() const
{
    return Point(-m_x, -m_y, -m_z);
}

// Return true if the point is set at the origin, false otherwise
bool Point::operator!() const
{
    return (m_x == 0.0 && m_y == 0.0 && m_z == 0.0);
}

void Point::print()
{
    cout << "X: " << m_x << " Y: " << m_y << " Z: " << m_z << endl;
}

int main()
{
    Point point(10, 20, 30);
    point.print();

    cout << "Make all the co-ordinate of the point class negative: " << endl;
    Point point3 = (-point);
    point3.print();

    Point point1(0, 0, 0);
    point1.print();

    if (!point1)
        std::cout << "point is set at the origin.\n";
    else
        std::cout << "point is not set at the origin.\n";

    return 0;
}