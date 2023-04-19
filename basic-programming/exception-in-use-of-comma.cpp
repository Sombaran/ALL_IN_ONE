// A comma operator question
// Consider the following C programs.

#include <iostream>
//  program fails in compilation,
// int example1( )
// {
//     int a = 1, 2, 3;
//     printf("%d\n", a);
//     return 0;
// }

//  program compiles fine and prints 3.
int example2()
{
    int b = (1, 2, 3);
    printf("%d\n", b);
    return 0;
}

// program compiles fine and prints 1.
int example3()
{
    int a;
    a = 1, 2, 3;
    printf("%d\n", a);
    return 0;
}
// And the following program prints 3,

int example4()
{
    int a;
    a = (1, 2, 3);
    printf("%d\n", a);
    return 0;
}

int main()
{
    // example1();
    example2();
    example3();
    example4();
}

// Note:
//     1. Comma works just as a separator in example1 and we get compilation error
//     2. Comma works as an operator in example3. Precedence of comma operator is least in operator precedence table. So the assignment operator takes precedence over comma and 3. The expression "a = 1, 2, 3" becomes equivalent to "(a = 1), 2, 3". That is why we get output as 1 in the second program.
//     4. In example4, brackets are used so comma operator is executed first and we get the output as 3

/**
 *  Increases value of a by 2, then assigns value of resulting operation a + b into i.
 *  Commas act as separators in the first line and as an operator in the second line.
 *  Results: a=3, b=2, c=3, i=5

int a=1, b=2, c=3;
int i = (a += 2, a + b);

 */

/**
 *  Assigns value of b into i.
 *  Commas act as separators in the first line and as an operator in the second line.
 *  Results: a=1, b=2, c=3, i=2

int a=1, b=2, c=3;
int i = (a, b);
 */

/**
 *  Assigns value of a into i.
 *  Equivalent to: int i = a; int b;
 *  Commas act as separators in both lines.
 *  The braces on the second line avoid variable redeclaration in the same block,
 *  which would cause a compilation error.
 *  The second b declared is given no initial value.
 *  Results: a=1, b=2, c=3, i=1

int a=1, b=2, c=3;
{ int i = a, b; }

*/