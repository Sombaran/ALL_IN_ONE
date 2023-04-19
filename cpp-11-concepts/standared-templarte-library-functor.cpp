// C++ STL Functors

//     STL includes a set of template classes that overload the
//      function call operator (operator ()).
//     Instances of those classes are called functors or function objects.

//     STL has two kinds of function objects Or functor:
//         Unary Functor: Functor that can be called with one arguments
//              The predicates which take one argument are called unary predicates
//         Binary Functor: Functor that can be called with two arguments
//              The predicates which take two argument are called binary predicates

//     STL functors are declared in the header <functional>, Tts in namespace std.
//     STL functors are gtoup of function objects called predicate.
//     This predicate takes one arguments or two arguments to return booleana.

//     They are divided in following groups according their functionality:
//         Functors for Arithmetic Operations:
//             They are called for arithmetic operations like addition and subtraction
//         Functors for Comparison Operations:
//             They are called for comparing two values like equality or inequality.
//         Functors for Logical Operations:
//             They are called for logical operation like logical AND
//         Functors for Bitwise Operations:
//             They are called to perform bitwise operations like bitwise AND and bitwise OR.

// example - Functor
#include <iostream>
#include <algorithm>
#include <vector>
#include <functional>

using namespace std;

int normal_functor()
{

    // initialize vector of int
    vector<int> nums = {1, 20, 3, 89, 2};

    // sort the vector in descending order
    sort(nums.begin(), nums.end(), greater<int>());

    for_each(begin(nums), end(nums), [](int i)
             { cout << i << " "; });
    cout << endl;
    // sort the vector in acending order
    sort(nums.begin(), nums.end(), less<int>());

    for_each(begin(nums), end(nums), [](int i)
             { cout << i << " "; });
    cout << endl;
    return 0;
}

struct Student
{
    int marks;
    string name;
    bool presence;
    //----------------------------------------------------------
    int operator+(const Student &stud) // std::plus
    {
        return this->marks + stud.marks;
    }
    int operator-(const Student &stud) // std::minus
    {
        return this->marks - stud.marks;
    }
    int operator-() // std::negate
    {
        return -1 * this->marks;
    }
    int operator*(const Student &stud) // std::negate
    {
        return this->marks * stud.marks;
    }
    int operator/(const Student &stud) // std::negate
    {
        return this->marks / stud.marks;
    }

    int operator%(const Student &stud) // std::negate
    {
        return this->marks % stud.marks;
    }
    //----------------------------------------------------------
    bool operator==(const Student &stud)
    {
        if (this->marks == stud.marks)
        {
            return true;
        }
        return false;
    }

    bool operator!=(const Student &stud)
    {
        if (this->marks != stud.marks)
        {
            return true;
        }
        return false;
    }

    bool operator>(const Student &stud)
    {
        if (this->marks > stud.marks)
        {
            return true;
        }
        return false;
    }

    bool operator>=(const Student &stud)
    {
        if (this->marks >= stud.marks)
        {
            return true;
        }
        return false;
    }

    bool operator<(const Student &stud)
    {
        if (this->marks < stud.marks)
        {
            return true;
        }
        return false;
    }

    bool operator<=(const Student &stud)
    {
        if (this->marks <= stud.marks)
        {
            return true;
        }
        return false;
    }

    //----------------------------------------------------------

    bool operator&&(const Student &stud)
    {
        if (this->marks <= stud.marks)
            return true;
        return false;
    }
    bool operator||(const Student &stud)
    {
        if (this->marks || stud.marks)
            return true;
        return false;
    }
    bool operator!()
    {
        return !this->presence;
    }
    //----------------------------------------------------------

    bool operator^(const Student &stud)
    {
        return this->marks ^ stud.marks;
    }

    bool operator|(const Student &stud)
    {
        return this->marks | stud.marks;
    }

    bool operator&(const Student &stud)
    {
        return this->marks & stud.marks;
    }
    //----------------------------------------------------------
};

//     1. Arithmetic Functors
//         Functors	    Description
//         plus	        returns the sum of two parameters
//         minus	        returns the difference of two parameters
//         multiplies	    returns the product of two parameters
//         divides	        returns the result after dividing two parameters
//         modulus	        returns the remainder after dividing two parameters
//         negate	        returns the negated value of a parameter

void Arithmetic_Functors()
{

    std::plus<int> Plus1;  // Subtract two integers
    std::plus<> Plus2;     // // default type is void, so specialization used
    std::plus<void> Plus3; //  // template specialization used

    std::cout << "std::plus<> Plus1: " << Plus1(20, 10) << endl;
    std::cout << "std::plus<> Plus2: " << Plus2(200, -100) << endl;
    std::cout << "std::plus<> Plus3: " << Plus3(2000, 1000) << endl;

    std::minus<int> Minus1; // Minus two integers
    std::cout << "Minus1(20, -10): " << Minus1(20, -10) << endl;

    std::multiplies<int> Multiplies; // Multiplies two integers
    std::cout << "Multiplies(20, 10): " << Multiplies(20, 10) << endl;

    std::divides<int> Divides; // Divides two integers
    std::cout << "Divides(20, 10): " << Divides(20, 10) << endl;

    std::modulus<int> Modulus; // Modulus two integers
    std::cout << "Modulus(20, 3): " << Modulus(20, 3) << endl;

    std::negate<> Negate; // Negate two integers
    std::cout << "Negate(20): " << Negate(20) << endl;

    Student S1{90, "Joti", true}, S2{80, "Swami", false}, S3{90, "Priya", true};

    std::cout << "Plus S1 + S2 : " << S1 + S2 << endl;
    std::cout << "Minus S1 - S2 : " << S1 - S2 << endl;
    std::cout << "Multiplies S1 * S2 : " << S1 * S2 << endl;
    std::cout << "Divides S1 / S2 : " << S1 / S2 << endl;
    std::cout << "Modulus S1 % S2 : " << S1 % S2 << endl;
    std::cout << "Negate -S1 : " << -S1 << endl;
}

//     2. Relational Functors
//         Functors	        Description
//         equal_to	        returns true if the two parameters are equal
//         not_equal_to	    returns true if the two parameters are not equal
//         greater	        returns true if the first parameter is greater than the second
//         greater_equal	returns true if the first parameter is >= to the second
//         less	            returns true if the first parameter is less than the second
//         less_equal	    returns true if the first parameter is <= to the second

void Relational_Functors()
{
    std::equal_to<int> Equal_to; // Equal_to two integers
    std::cout << "Equal_to(20, 10): " << Equal_to(20, 10) << endl;

    std::not_equal_to<int> Not_equal_to; // Not_equal_to two integers
    std::cout << "Not_equal_to(20, 10): " << Not_equal_to(20, 10) << endl;

    std::greater<int> Greater; // Greater two integers
    std::cout << "Greater(20, 10): " << Greater(20, 10) << endl;

    std::greater_equal<int> Greater_equal; // Greater_equal two integers
    std::cout << "Greater_equal(20, 10): " << Greater_equal(20, 10) << endl;

    std::less<int> Less; // Less two integers
    std::cout << "Less(20, 3): " << Less(20, 3) << endl;

    std::less_equal<> Less_equal; // Less_equal two integers
    std::cout << "Less_equal(20, 30) : " << Less_equal(20, 30) << endl;

    Student S1{90, "Joti", true}, S2{80, "Swami", false}, S3{90, "Priya", true};

    cout << "Equal_to (S1 == S3): " << (S1 == S3) << endl;
    cout << "Not_equal_to (S1 == S2): " << (S1 != S2) << endl;
    cout << "Greater (S1 > S2): " << (S1 > S2) << endl;
    cout << "Greater_equal (S1 >= S2): " << (S1 >= S2) << endl;
    cout << "Less (S1 < S2): " << (S1 < S2) << endl;
    cout << "Less_equal (S1 <= S2): " << (S1 <= S2) << endl;

    std::equal_to<> Eequal_to1;
    cout << "Eequal_to1(S1, S2): " << Eequal_to1(S1, S2) << endl;
    cout << "Eequal_to1(S1, S3): " << Eequal_to1(S1, S3) << endl;
    cout << "Eequal_to1(S2, S3): " << Eequal_to1(S2, S3) << endl;

    std::not_equal_to<> Not_equal_to1; // Not_equal_to two Student marks
    std::cout << "Not_equal_to(S1, S2): " << Not_equal_to1(S1, S2) << endl;

    std::greater<> Greater1; // Greater two integers
    std::cout << "Greater(S1, S2): " << Greater1(S1, S2) << endl;

    std::greater_equal<> Greater_equal1; // Greater_equal two Student marks
    std::cout << "Greater_equal(S1, S2): " << Greater_equal1(S1, S2) << endl;

    std::less<> Less1; // Less two Student marks
    std::cout << "Less(S1, S2): " << Less1(S1, S2) << endl;

    std::less_equal<> Less_equal1; // Less_equal two Student marks
    std::cout << "Less_equal(S1, S2): " << Less_equal1(S1, S2) << endl;
}

//     3. Logical Functors
//         Functors	        Description
//         logical_and	    returns the result of Logical AND operation of two booleans
//         logical_or	    returns the result of Logical OR operation of two booleans
//         logical_not	    returns the result of Logical NOT operation of a boolean

void Logical_Functors()
{
    Student S1{90, "Joti", true}, S2{80, "Swami", false}, S3{90, "Priya", true};
    std::logical_and<> Logical_and; // logical_and two integers
    std::cout << "Logical_and(20, 10): " << Logical_and(20, 10) << endl;
    std::cout << "Logical_and(S1, S2): " << Logical_and(S1, S2) << endl;

    std::logical_or<> Logical_or; // Logical_or two integers
    std::cout << "Logical_or(20, 10): " << Logical_or(20, 10) << endl;
    std::cout << "Logical_or(S1, S2): " << Logical_or(S1, S2) << endl;

    std::logical_not<> Logical_not; // Logical_or two integers
    std::cout << "Logical_not(false): " << Logical_not(false) << endl;
    std::cout << "Logical_not(true): " << Logical_not(true) << endl;

    cout << "logical_and (S1 && S2): " << (S1 && S2) << endl;
    cout << "logical_or (S1 || S2): " << (S1 || S2) << endl;
    cout << "logical_not (!S2): " << !S2 << endl;
}

//     4. Bitwise Functors
//         Functors	        Description
//         bit_and	        returns the result of Bitwise AND operation of two parameters
//         bit_or	        returns the result of Bitwise OR operation of two parameters
//         bit_xor	        returns the result of Bitwise XOR operation of two parameters

void Bitwise_Functors()
{
    Student S1{60, "Joti", true}, S2{13, "Swami", false}, S3{90, "Priya", true};

    std::bit_and<int> ba;  // Argument type is int
    std::bit_or<> bo;      // Argument type is void
    std::bit_xor<void> bx; // Argument type is void
    std::bit_not<char> bn; // Argument type is void

    std::cout << ba(0x60, 0x13) << std::endl; // Bitwiae AND operation of two integers.
    std::cout << bo(0x11, 0x22) << std::endl; // Bitwise OR operation of two arguments of deduced types
    std::cout << bn(~('A')) << std::endl;     // Bit-wise NOT operation of the argument type char
    std::cout << bx(0x10, 0x01) << std::endl; // Bit-wise XOR operation of the argument of deduced

    std::bit_and<> Bit_and; // Bit_and two integers
    std::cout << "Bit_and(60 & 13): " << Bit_and(0x60, 0x13) << endl;
    std::cout << "Bit_and(S1, S2): " << Bit_and(S1, S2) << endl;

    std::bit_or<> Bit_or; // Bit_or two integers
    std::cout << "Bit_or(20, 10): " << Bit_or(60, 13) << endl;
    std::cout << "Bit_or(S1, S2): " << Bit_or(S1, S2) << endl;

    std::bit_xor<> Bit_xor; // Logical_or two integers
    std::cout << "Bit_xor(60, 13): " << Bit_xor(60, 13) << endl;
    std::cout << "Bit_xor(S1, S2): " << Bit_xor(S1, S2) << endl;
}

int main()
{
    normal_functor();
    Arithmetic_Functors();
    Relational_Functors();
    Logical_Functors();
    Bitwise_Functors();
    return 0;
}