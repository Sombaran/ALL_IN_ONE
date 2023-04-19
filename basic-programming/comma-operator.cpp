
// Comma in C and C++
//     In C and C++, comma (, ) can be used in two contexts:

#include <iostream>
using namespace std;
#ifndef NULL
// 1) Comma as an operator:
//     The comma operator (represented by the token , ) is a binary operator
//     That evaluates its first operand and discards the result,
//     It then evaluates the second operand and returns this value (and type).
//     The comma operator has the lowest precedence of any C operator,
//     and acts as a sequence point.

//---------------------------------------------------------------------------
/* comma as an operator */
int i = (5, 10);      /* 10 is assigned to i*/
int j = (f1(), f2()); /* f1() is called (evaluated) first followed by f2().
                        The returned value of f2() is assigned to j */
//---------------------------------------------------------------------------

// 2) Comma as a separator:
//     Comma acts as a separator when used with function calls and definitions,
//     function like macros, variable declarations, enum declarations, and similar constructs.

/* comma as a separator */
int a = 1, b = 2;
void fun(x, y);
//---------------------------------------------------------------------------

// 3. The use of comma as a separator should not be confused with the use as an operator.
//     For example, in below statement, f1() and f2() can be called in any order.
/* Comma acts as a separator here and doesn't enforce any sequence.
    Therefore, either f1() or f2() can be called first */
void fun(f1(), f2());
//---------------------------------------------------------------------------
#endif
// exmple1
int exmple1()
{
    int x = 10;
    int y = 15;

    printf("%d\n", (x, y)); // 15
    return 0;
}
//---------------------------------------------------------------------------
// example2
int example2()
{
    int x = 10;
    int y = (x++, ++x);
    printf("%d\n", y); // 12
    return 0;
}
//---------------------------------------------------------------------------
// example3
int example3()
{
    int x = 10, y;

    // The following is equivalent
    // to y = x + 2 and x += 3,
    // with two printings
    y = (x++,
         printf("x = %d\n", x),
         ++x,
         printf("x = %d\n", x),
         x++); // at the end the value of x is post-increment so that y = 12 but x = 13

    // Note that last expression is evaluated
    // but side effect is not updated to y
    printf("y = %d\n", y);
    printf("x = %d\n", x);

    return 0;
}
//---------------------------------------------------------------------------

//  This is because the reason that assignment operator has high precedence over the comma operator.

int example4()
{
    int a = 5;
    a = 2, 3, 4;
    // the above line is evaluated as: (((a = 2), 3), 4);
    cout << "a = 2, 3, 4    => " << a << endl;
    a = (2, 3, 4);
    cout << "a = (2, 3, 4)  => " << a << endl;

    return 0;
}

// 3) Comma operator in place of a semicolon:
//     We know that in C and C++, every statement is terminated with a semicolon
//     but comma operator also used to terminate the statement after satisfying
//     the following rules :
//         1. The variable declaration statements must be terminated with semicolon.
//         2. The statements after declaration statement can be terminated by comma operator.
//         3. The last statement of the program must be terminated by semicolon.

int main()
{
    exmple1();
    example2();
    example3();
    example4();
}
//---------------------------------------------------------------------------