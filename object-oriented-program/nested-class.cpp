// // Nested Classes
//     A Nested class is a class which is declared in another OuterClass class.
//     A Nested class is a member and as it has same access rights as any other member of the class.
//     The members of an OuterClass class have no special access to members of a Nested class;
//     The usual access rules shall be obeyed.

// program - 1 // compiles success

#include <iostream>
using namespace std;

/* start of OuterClass class declaration */
class OuterClass1
{
private:
    int x;
    /* start of Nested class declaration */
    class Nested1
    {
        int y;
        void NestedFun(OuterClass1 *e)
        {
            cout << e->x; // works fine: Nested class can access
                          // private members of OuterClass class
        }
    }; // declaration Nested class ends here

}; // declaration OuterClass class ends here

int program1()
{
    return 0;
}

// Program - 2 // fails in compilation.

/* start of OuterClass class declaration */
class OuterClass
{
    int x;
    /* start of Nested class declaration */
    class Nested
    {
        int y;
    }; // declaration Nested class ends here

    void OuterClassFun(Nested *n)
    {
        cout << n->y; // Compiler Error: y is private in Nested
    }
}; // declaration OuterClass class ends here

int main()
{
    program1();
    return 0;
}
