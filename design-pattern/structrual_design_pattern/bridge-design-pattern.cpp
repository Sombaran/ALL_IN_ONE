// // Bridge design pattern

//     The Bridge design pattern allows you to separate the abstraction from the implementation.It is a structural design pattern.

//     There are 2 parts in Bridge design pattern :
//         1. Abstraction
//         2. Implementation

//     Following participants constitute the bridge design pattern.

//     Abstraction (abstract class)
//         It defined the abstract interface i.e. behavior part. It also maintains the Implementer reference.

//     RefinedAbstraction (normal class)
//         It extends the interface defined by Abstraction.

//     Implementer (interface)
//         It defines the interface for implementation classes.
//     This interface does not need to correspond directly to abstraction interface and
//     can be very different. Abstraction imp provides an implementation in terms of
//     operations provided by Implementer interface.

//     ConcreteImplementor (normal class)
//         It implements the Implementer interface.

//     When we need bridge design pattern
//         For example, let’s say you are creating various GUI shapes with different
//     colors. One solution could be:

/*
                        -------------------Shape------------------
                            /                                \
                        Rectangle                          Circle
                        /       \                       /          \
                BlueRectangle   RedRectangle     BlueCircle    RedCircle
*/
//     But above solution has a problem.
//     If you want to change Rectange class, then you may end up changing BlueRectangle
//     and RedRectangle as well – and even if change is color specific then you may need
//     to change Circle classes as well.

//     We can solve above problem by decoupling the Shape and Color interfaces
//     in below manner.

/*
                    ----------—-Shape---------—-            Color
                           /               \                /    \
                    Rectangle(Color)    Circle(Color)    Blue     Red
*/
//     Now when you change any Shape, color would be unchanged.Similarily, vice - versa.

#include <iostream>
using namespace std;

/*
 * Color
 * defines the interface for implementation RedColor and BlueColor classes
 */
class Color
{
protected:
    string value;

public:
    virtual ~Color()
    {
    }

    virtual string fillColor() = 0;
};

/*
 * Concrete Implementors
 * implement the Color interface and define concrete implementations
 */
class RedColor : public Color
{

public:
    RedColor()
    {
        value = "Red";
    }
    ~RedColor()
    {
    }

    string fillColor()
    {
        return value;
    }
};

/*
 * Concrete Implementors
 * implement the Color interface and define concrete implementations
 */
class BlueColor : public Color
{
public:
    BlueColor()
    {
        value = "Blue";
    }
    ~BlueColor()
    {
    }

    string fillColor()
    {
        return value;
    }
};

/*
 * Shape
 * defines the abstraction's interface
 */
class Shape
{
protected:
    Color *scale;

public:
    virtual ~Shape()
    {
    }

    virtual void colorIt() = 0;
};

/*
 * Rectangle
 * extends the interface defined by Shape
 */
class Rectangle : public Shape
{

public:
    ~Rectangle()
    {
    }

    Rectangle(Color *impl)
    {
        scale = impl;
    }

    void colorIt()
    {
        cout << "Color of Rectangle is: " << scale->fillColor() << endl;
    }
};

/*
 * Circle
 * extends the interface defined by Shape
 */
class Circle : public Shape
{

public:
    ~Circle()
    {
    }

    Circle(Color *impl)
    {
        scale = impl;
    }

    void colorIt()
    {
        cout << "Color of Circle is: " << scale->fillColor() << endl;
    }
};

int main()
{
    Color *ia = new RedColor;
    Color *ib = new BlueColor;

    Shape *Shape_rectangle = new Rectangle(ia);
    Shape_rectangle->colorIt();

    Shape *Shape_circle = new Circle(ib);
    Shape_circle->colorIt();

    delete Shape_rectangle;
    delete Shape_circle;

    delete ia;
    delete ib;

    return 0;
}
