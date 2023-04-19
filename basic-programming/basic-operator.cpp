// Operators in C++

//     What are Operators
//     An operator is a symbol used for performing operations on operands.
//     An operator operates operands.
//     The operations can be mathematical or logical.
//     There are different types of operators in C++ for performing different operations.

#include <iostream>
using namespace std;

// Types Of Operators in C++
//     There are mainly 6 different types of operators in C++
//         1. Arithmetic Operators
//         2. Relational Operators
//         3. Logical Operators
//         4. Bitwise Operators
//         5. Assignment Operators
//         6. Other Operators

// Arithmetic Operators
//     They are the types of operators used for performing mathematical/arithmetic operations.
//     Operator Description
//         +  addition operator         Adds to operands.
//         —  subtraction operator      Subtracts 2nd operand from 1St operand.
//         *  multiplication operator   Multiplies 2 operands.
//         /  division operator.        Divides numerator by denominator.
//         %  modulus operator          Returns remainder after division.
//         ++ increment operator       Increases an integer value by 1.
//         —  decrement operator.       Decreases an integer value by 1.

int Arithmetic_Operators()
{
    int a = 11;
    int b = 5;
    int c;

    cout << "a= " << a << ", b= " << b << endl;
    cout << "a + b is :" << a + b << endl; //    11 + 5
    cout << "a - b is :" << a - b << endl; //    11 - 5
    cout << "a * b is :" << a * b << endl; //    11 * 5
    cout << "a / b is :" << a / b << endl; //    11 / 5
    cout << "a % b is :" << a % b << endl; //    11 % 5
    cout << "a++ is :" << a++ << endl;     //    11++
    cout << "a-- is :" << a-- << endl;     //    12--
    cout << "++a is :" << ++a << endl;     //    ++11
    cout << "--a is :" << --a << endl;     //    --12
    return 0;
}

// Relational Operators
//     These types of operators perform comparisons on operands.
//     For example, you may need to know which operand is greater than the other, or less than the other.

// Operator	Description
// == equal to operator.
//     Checks equality of two operand values.
// != not equal to operator
//     Checks equality of two operand values.
// > great than operator
//     Checks whether value of left operand is greater than value of right operand.
// < less than operator.
//     Checks whether value of left operand is less than value of right operand.
// >= greater than or equal to operator
//     Checks whether value of left operand is greater than or
//     equal to value of right operand.
// <= less than or equal to operator.
//     Checks whether value of left operand is less than or
//     equal to value of right operand.

int Relational_Operators()
{
    int a = 11;
    int b = 5;

    cout << "a=11, b=5" << endl;
    if (a == b)
    {
        cout << "a == b is true" << endl;
    }
    else
    {
        cout << " a == b is false" << endl;
    }

    if (a != b)
    {
        cout << "a != b is true" << endl;
    }
    else
    {
        cout << " a != b is false" << endl;
    }

    if (a < b)
    {
        cout << "a < b is true" << endl;
    }
    else
    {
        cout << "a < b is false" << endl;
    }

    if (a > b)
    {
        cout << "a > b is true" << endl;
    }
    else
    {
        cout << "a > b is false" << endl;
    }

    return 0;
}

// Logical Operators
//     The logical operators combine two/more constraints/conditions.
//     Logical operators also complement evaluation of original condition under consideration.

// Operator	Description
// && logical AND operator.
//     The condition is true if both operands are not zero.
// || logical OR operator.
//     The condition is true if one of the operands is non-zero.
// !  logical NOT operator.
//     It reverses operand’s logical state. If the operand is true,
//     the ! operator makes it false.

int Logical_Operators()
{
    int a = 5, b = 2, c = 6, d = 4;
    if (a == b && c > d)
        cout << "a equals to b AND c is greater than d" << endl;
    else
        cout << "AND operation returned false" << endl;

    if (a == b || c > d)
        cout << "a equals to b OR c is greater than d" << endl;
    else
        cout << "Neither a is equal to b nor c is greater than d" << endl;

    if (!b)
        cout << "b is zero" << endl;
    else
        cout << "b is not zero" << endl;

    return 0;
}

// Bitwise Operators
//     Bitwise operators perform bit-level operations on operands.
//     First, operators are converted to bit level then operations are performed on the operands.
//     When arithmetic operations like addition and subtraction are done at bit level,
//     results can be achieved faster.

// Operator	Description
// & (bitwise AND).
//     It takes 2 numbers (operands) then performs AND on each bit of two numbers.
//     If both are 1, AND returns 1, otherwise 0.

// | (bitwise OR)
//     Takes 2 numbers (operands) then performs OR on every bit of two numbers.
//     It returns 1 if one of the bits is 1.

// ^ (the bitwise XOR)
//     Takes 2 numbers (operands) then performs XOR on every bit of 2 numbers.
//     It returns 1 if both bits are different.

// << (left shift)
//     Takes two numbers then left shifts the bits of the first operand.
//     The second operand determines total places to shift.

// >> (right shift)
//     Takes two numbers then right shifts the bits of the first operand.
//     The second operand determines number of places to shift.

// ~ (bitwise NOT).
//     Takes number then inverts all its bits.

int Bitwise_Operators()
{
    unsigned int p = 60; //    60 = 0011 1100
    unsigned int q = 13; //    13 = 0000 1101
    int z = 0;

    z = p & q;
    cout << "p&q is : " << z << endl; //    12 = 0000 1100

    z = p | q;
    cout << "p|q is : " << z << endl; //    61 = 0011 1101

    z = p ^ q;
    cout << "p^q is : " << z << endl; //    49 = 0011 0001

    z = ~p;
    cout << "~p is : " << z << endl; //    -61 = 1100 0011

    z = p << 2;
    cout << "p<<2 is: " << z << endl; //    240 = 1111 0000

    z = p >> 2;
    cout << "p>>2 is : " << z << endl; //    15 = 0000 1111

    unsigned int val = 60; //    60 = 0011 1100

    // if you right shift by one time value will get double
    // if value = 60 and you performed once right shit the final value = 120
    cout << (val << 1) << endl; //=> 120

    // if you right shift by two times valye will get double double
    // if value = 60 and you performed double Left shit the final value = 240
    cout << (val << 2) << endl; //=> 240

    cout << val << endl; //=> 60

    // if you Left shift by one time value will get divided by 2
    // if value = 60 and you performed once Left shit the final value = 5
    cout << (val >> 1) << endl; //=> 5

    // if you Left shift by two times valye will get double double
    // if value = 60 and you performed double Left shit the final value = 1
    cout << (val >> 2) << endl; //=> 1

    return 0;
}

// Assignment Operators
//     Assignment operators assign values to variables.
//     The operand/variable is added to left side of the operator while the value is added to the right side of the operator.
//     The variable and the value must belong to the same data type, otherwise, the C++ compiler will raise error.
//     For example:

// Operator	Description
// =  (simple assignment operator)
//     It assigns value on the right to variable on the left.
// += (Add AND assignment operator)
//         It first adds value of left operand to value of right operand then
//         assigns result to variable on the left.
// -= (Subtract AND assignment operator)
//     It first subtracts value of right operand from value of left operand then
//     assigns result to variable on the left.
// *= (Multiply AND assignment operator)
//     It first multiplies value of left operand with value of right operand then
//     assigns result to variable on the left.
// /= (Division AND assignment operator)
//     It first divides value of left operand by value of right operand then
//     assigns result to variable on the left.

int Assignment_Operators()
{
    int x = 5;
    cout << "Initial value of x is " << x << endl;

    x += 5;
    cout << "x += 5 gives :" << x << endl;

    x -= 5;
    cout << "x -= 5 gives : " << x << endl;

    x *= 5;
    cout << "x *= 5 gives :" << x << endl;

    x /= 5;
    cout << "x /= 5 gives : " << x << endl;

    return 0;
}

// Other Operators
//     Other Operators include
//         1. sizeof operator,
//         2. Comma Operator
//         3. Conditional Operator
//         4. Operators Precedence.

// Let us discuss other operators supported by C++ :

int sizeof_Operator()
{
    cout << "Size of int : " << sizeof(int) << " bytes" << endl;
    cout << "Size of char : " << sizeof(char) << " bytes" << endl;
    cout << "Size of float : " << sizeof(float) << " bytes" << endl;
    cout << "Size of double : " << sizeof(double) << " bytes" << endl;
    return 0;
}

// Comma Operator
//     The comma
//     operator(, ) triggers the performance of a sequence of operations.It expresses first operand and discards the result.Next,
//     it evaluates the second operand and returns the value and type.

int Comma_Operator()
{
    int x, y;
    y = 100;
    x = (y++, y + 10, 99 + y); //    101, ->101, ->101 + 99, = > 200
    cout << x << endl;
    return 0;
}

// Conditional Operator
//     This operator evaluates a condition and acts based on the outcome of the evaluation.

//     Syntax:
//         Condition ? Expression2 : Expression3;
//     Parameters:
//         The Condition is the condition that is to be evaluated.
//         Expression1 is the expression to be executed if condition is true.
//         Expression3 is the expression to be executed if condition is false.

int Conditional_Operator()
{
    int a = 5, b = 10, ret;
    ret = (a > b) ? a : b;
    cout << "value of max(5, 10): " << ret << endl;
    ret = (a < b) ? a : b;
    cout << "value of min(5, 10): " << ret << endl;
    return 0;
}

// Operators Precedence
//     A single operation may have more than one operator.
//     In that case, operator precedence determines the one evaluated first.
//     The following list shows the precedence of operators in C++, with decreasing precedence from left to right:
//     (), [],*, /, %, +/-, << , >>, == , !=, ^, |, &&, ||, ?:, =, +=, -=, *=, /=

// Right to Left

//     ++a         Prefix Increment
//     --a         Prefix decrement
//     +a          Unary plus
//     -a          Unary minus
//     !           Logical NOT
//     ~           Bitwise NOT
//     (type)      * C style cast
//     *a          Indirection (dereference)
//     &a          Address-of
//     Sizeof      Size-of
//     new new[]   Dynamic memory allocation
//     delete      Dynamic memory deallocation
//     delete[]    Dynamic memory deallocation

//     a ? b: c            Ternary Conditional
//     throw               throw operator
//     =                   Assignment
//     +=                  Addition Assignment
//     -=                  Subtraction Assignment
//     *=                  Multiplication Assignment
//     /=                  Division Assignment
//     %=                  Modulus Assignment
//     <<=                 Bitwise Shift Left Assignment
//     >>=                 Bitwise Shift Right Assignment
//     &=                  Bitwise AND Assignment
//     ^=                  Bitwise XOR Assignment
//     |=                  Bitwise OR Assignment

// Left to right
//     ::          Scope Resolution
//     a++         Suffix/postfix increment
//     a--         Suffix/postfix decrement
//     type( )     Function cast
//     type{}      Function cast
//     a()         Function call
//     a[]         Subscript
//     .           Member access from an object
//     ->          Member access from object ptr
//     ,	       Comma operator
//     .*          Member object selector
//     ->*	       Member pointer selector
//     a*b       Multiplication
//     a/b         division
//     a % b       Modulus
//     a + b       Addition
//     a - b       Subtraction
//     <<          Bitwise left shift
//     >>          Bitwise right shift
//     <=<         Three-way comparison operator
//     <           Less than
//     <=          Less than or equal to
//     >           Greater than
//     >=          Greater than or equal to
//     ==          Equal to
//     !=          Not equal to
//     &           Bitwise AND
//     ^           Bitwise XOR
//     |           Bitwise OR
//     &&          Logical AND
//     ||          Logical OR

int Operators_Associativity()
{
    int a = 1;
    int b = 4;

    // a -= 6 is evaluated first
    b += a -= 6;

    cout << "a = " << a << endl;
    cout << "b = " << b;
    return 0;
}

int main()
{
    Arithmetic_Operators();
    Relational_Operators();
    Logical_Operators();
    Bitwise_Operators();
    Assignment_Operators();
    sizeof_Operator();
    Comma_Operator();
    Conditional_Operator();
    Operators_Associativity();

    return 0;
}

// Summary:
//     1. Operators are symbols for performing logical and arithmetic operations.
//     2. Arithmetic operators help us perform various arithmetic operations on operands.
//     3. Relational operators help us perform various comparison operations on operands.
//     4. Logical operators help us perform various logical operations on operands.
//     5. Bitwise operators help us perform bitwise operations on operands.
//     6. Assignment operators help us perform various arithmetic operations on operands.
//     7. The sizeof operator returns the size of a variable or data type.
//     8. The comma operator executes a sequence of operations.
//     9. The conditional operator evaluates a condition and acts based on the outcome.

/*
Note - Example with details
    1. Pre - Increment/Decrement
    2. Post - Increment/Decrement

*/

int main()
{

    unsigned int value1 = 10;
    unsigned int value2;

    cout << "value1 is now: " << value1 << endl;
    cout << "value2 = ++value1" << endl;

    // Pre - Increment
    value2 = ++value1;

    // the above line will break into only one below line in complilation
    // statement - 1 =>  value2 = value1 = value1 + 1

    cout << "Pre - Increment value1: " << value1 << endl;
    cout << "Pre - Increment value2: " << value2 << endl;

    // Pre - Decrement
    cout << "value2 = --value1" << endl;

    value2 = --value1;
    // the above line will break into only one below line in complilation
    // statement - 1 =>  value2 = value1 = value1 - 1

    cout << "Pre - Decrement value1: " << value1 << endl;
    cout << "Pre - Decrement value2: " << value2 << endl;

    cout << "value1 is now: " << value1 << endl;

    // Post - Increment
    cout << "value2 = value1++" << endl;

    value2 = value1++;
    // the above line will break into the below two statement line in complilation
    // statement - 1 =>  value2 = value1
    // statement - 2 =>   value1 = value1 + 1

    cout << "Post - Increment value1: " << value1 << endl;
    cout << "Post - Increment value2: " << value2 << endl;

    // Post - Decrement
    cout << "value2 = value1--" << endl;

    value2 = value1--;
    // the above line will break into the below line in complilation
    // statement - 1 =>  value2 = value1
    // statement - 2 =>  value1 = value1 - 1

    cout << "Post - Decrement value1: " << value1 << endl;
    cout << "Post - Decrement value2: " << value2 << endl;

    return 0;
}

/*
Output of Above program:

value1 is now: 10
value2 = ++value1
Pre - Increment value1: 11
Pre - Increment value2: 11
value2 = --value1
Pre - Decrement value1: 10
Pre - Decrement value2: 10
value1 is now: 10
value2 = value1++
Post - Increment value1: 11
Post - Increment value2: 10
value2 = value1--
Post - Decrement value1: 10
Post - Decrement value2: 11

*/