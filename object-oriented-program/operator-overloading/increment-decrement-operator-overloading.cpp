//  Increment and Decrement Operators
//  Following are the examples of Increment and Decrement operators −

//     1. The increment (++) operators.
//     2. The decrement (--) operators.

//     The Increment and Decrement operators operate on the object for which they were 
//     called and normally.
//     This operator appears on the left side of the object like !obj, -obj, and ++obj --obj.
//     Also on right side as well obj++, obj--.
//     1. Increament and Decrement operator can be implemented in prefix as well as postfix way.
//         Prefix:  ++obj, --obj
//         Postfix: obj++, obj--

// // Example of the Prefix: ++obj, --obj and Postfix: obj++, obj--

//     As symbol for both postfix and prefix decrement operator is same.
//     The C++ language specification has a special case that provides the answer:
//         the compiler looks to see if the overloaded operator has an int parameter.
//         If the overloaded operator has an int parameter, the operator is a postfix overload.
//     If the overloaded operator has no parameter, the operator is a prefix overload.
//         So that to differentiate between these two operator functions
//         we need to pass an extra int argument in case of postfix decrement operator i.e.

#include <iostream>
using namespace std;
/*
 * A class representing integer variable
 */
class Number
{
    int variable;

public:
    Number() : variable(0)
    {
    }
    Number(int val) : variable(val)
    {
    }
    void print();
    // Prefix Increment Operator
    Number operator++();
    // Postfix Increment Operator
    Number operator++(int);
    // Prefix Decrement Operator
    Number operator--();
    // Postfix Decrement Operator
    Number operator--(int);
};

/*
 *  Prefix Increment Operator
 *  It first increments the Current Object and then returns its copy.
 */
Number Number::operator++()
{
    ++variable;
    return *this;
}
/*
 *  Postfix Increment Operator
 *  It first creates a temp copy of current object.
 *  Then increments the current object.
 *  In the end returns the temp copy created initially.
 */
Number Number::operator++(int)
{
    // Temp copy of the current object
    Number tempObj(this->variable);
    variable++;
    // Return the temp copy that stores the data before updation
    return tempObj;
}
/*
 *  Prefix Decrement Operator
 *  It first decrements the Current Object and then returns its copy.
 */
Number Number::operator--()
{
    --variable;
    return *this;
}
/*
 *  Postfix Decrement Operator
 *  It first creates a temp copy of current object.
 *  Then decrements the current object.
 *  In the end returns the temp copy created initially.
 */
Number Number::operator--(int)
{
    // Temp copy of the current object
    Number tempObj(this->variable);
    variable--;
    // Return the temp copy that stores the data before updation
    return tempObj;
}

void Number::print()
{
    cout << variable << endl;
}
int main()
{
    Number c1(2);
    std::cout << "c1 = ";
    c1.print();
    std::cout << "Testing Postfix Increment" << std::endl;
    Number c2 = c1++; // Postfix Increment
    std::cout << "c2 = ";
    c2.print();
    std::cout << "c1 = ";
    c1.print();
    std::cout << "Testing Prefix Increment" << std::endl;
    Number c3 = ++c1; // Prefix Increment
    std::cout << "c3 = ";
    c3.print();
    std::cout << "c1 = ";
    c1.print();
    std::cout << "Testing Postfix Decrement" << std::endl;
    Number c4 = c1--; // Postfix Decrement
    std::cout << "c4 = ";
    c3.print();
    std::cout << "c1 = ";
    c1.print();
    std::cout << "Testing Prefix Decrement" << std::endl;
    Number c5 = --c1; // Prefix Decrement
    std::cout << "c5 = ";
    c3.print();
    std::cout << "c1 = ";
    c1.print();
    return 0;
}

/**
 * @brief
 * There are a few interesting things going on here. First, note that we’ve distinguished the prefix from the postfix operators by providing an integer dummy parameter on the postfix version. Second, because the dummy parameter is not used in the function implementation, we have not even given it a name. This tells the compiler to treat this variable as a placeholder, which means it won’t warn us that we declared a variable but never used it.

Third, note that the prefix and postfix operators do the same job -- they both increment or decrement the object. The difference between the two is in the value they return. The overloaded prefix operators return the object after it has been incremented or decremented. Consequently, overloading these is fairly straightforward. We simply increment or decrement our member variables, and then return *this.

The postfix operators, on the other hand, need to return the state of the object before it is incremented or decremented. This leads to a bit of a conundrum -- if we increment or decrement the object, we won’t be able to return the state of the object before it was incremented or decremented. On the other hand, if we return the state of the object before we increment or decrement it, the increment or decrement will never be called.

The typical way this problem is solved is to use a temporary variable that holds the value of the object before it is incremented or decremented. Then the object itself can be incremented or decremented. And finally, the temporary variable is returned to the caller. In this way, the caller receives a copy of the object before it was incremented or decremented, but the object itself is incremented or decremented. Note that this means the return value of the overloaded operator must be a non-reference, because we can’t return a reference to a local variable that will be destroyed when the function exits. Also note that this means the postfix operators are typically less efficient than the prefix operators because of the added overhead of instantiating a temporary variable and returning by value instead of reference.

Finally, note that we’ve written the post-increment and post-decrement in such a way that it calls the pre-increment and pre-decrement to do most of the work. This cuts down on duplicate code, and makes our class easier to modify in the future.
 *
 */