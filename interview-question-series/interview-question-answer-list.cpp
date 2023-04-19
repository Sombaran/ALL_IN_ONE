// 1 What are storage classes?

#include <iostream>
// using namespace std;

// Storage class is used to define the lifetime and visibility of a variable and/or function within a C++ program.
// Lifetime refers to the period during which the variable remains active and visibility
//     refers to the module of a program in which the variable is accessible.

// There are five types of storage classes, which can be used in a C++ program
//     Automatic
//     Register
//     Static
//     External
//     Mutable

//     Storage Class	    Keyword	    Lifetime	        Visibility	    Initial Value
//     Automatic	        auto	    Function Block	    Local	        Garbage
//     Register	        register	Function Block	    Local	        Garbage
//     Mutable	            mutable	    Class	            Local	        Garbage
//     External	        extern	    Whole Program	    Global	        Zero
//     Static	            static	    Whole Program	    Local	        Zero

//     Automatic Storage Class
//         It is the default storage class for all local variables.
//         The auto keyword is applied to all local variables automatically.

//         {
//         auto int y;
//         float y = 3.45;
//         }

//         Thses defines two variables with a same storage class, auto can only be used within functions.

//     Register Storage Class
//         The register variable allocates memory in register than RAM.
//         Its size is same of register size.
//         It has a faster access than other variables.

//         It is recommended to use register variable only for quick access such as in counter.

//         Note: We can't get the address of register variable.

//         register int counter=0;

//     Static Storage Class
//         The static variable is initialized only once and exists till the end of a program.
//         It retains its value between multiple functions call.
//         The static variable has the default value 0 which is provided by compiler.
//         The static modifier may also be applied to global variables.
//         When this is done, it causes that variable's scope to be restricted to the file in which it is declared.

void func()
{
    static int i = 0; // static variable
    int j = 0;        // local variable
    i++;
    j++;
    cout << "i=" << i << " and j=" << j << endl;
}
int main()
{
    func();
    func();
    func();
}
//     Extern Storage Class
//     The extern storage class is used to give a reference of a global variable
//         that is visible to ALL the program files.
//     When you use 'extern' the variable cannot be initialized as all it does is point the variable name
//         at a storage location that has been previously defined.
//     When you have multiple files and you define a global variable or function,
//     which will be used in other files also, then extern will be used in another file to give
//         reference of defined variable or function.
//     Just for understanding extern is used to declare a global variable or function in another file.
//     The extern modifier is most commonly used when there are two or more files sharing
//         the same global variables or functions as explained below.

//----------------------------------------------------------
// First File: main.cpp
#include <iostream>

int count;
extern void write_extern();

main()
{
    count = 5;
    write_extern();
}
//----------------------------------------------------------
// Second File: support.cpp
#include <iostream>
extern int count;

void write_extern(void)
{
    std::cout << "Count is " << count << std::endl;
}
//----------------------------------------------------------

//     Mutable Storage Class
//         The mutable specifier applies only to class objects,
//         It allows a member of an object to override const member function.
//         That is, a mutable member can be modified by a const member function.
//         You cannot use the mutable specifier with names declared as static or const, or reference.

// 2 What is the difference between static and extern?

// 3 Can static variable be created oustide class?
// 4 What is the difference between static and global variable?
// 5 What is the memory allocation of static and global variable?
// 6 What are inline function?
// 7 What is recursion?
// 8 What is abstraction in C++?
// 9 Memory allocation of class object in stack and heap?
// 10 Is it legal to assign base class object into derived class pointer?
// 11 What is the size of empty class in C++?
// 12 What is size of abstract class with only pure virtual function?
// 13 On what basis compiler called the overloaded function in class?
// 14 Difference between normal function overloading and class member function overloading?
// 15 What is operator overloading?
// 16 What is virtual and pure virtual function?
// 17 What is static function in class?
// 18 Difference between normal static function and class static function in C++?
// 19 What is friend function and why it is used?
// 20 Difference between structure and class?
// 21 What are access specifier and what is the role in inheritance ?
// 22 What is public protected and private inheritance?
// 23 What is Singleton class and how to create it?
// 24 Why static function used in singleton class?
// 25 What are some advance C++ feature?
// 26 What are smart pointer and its types?
// 27 How to overload preincreement and post increement operator?
// 28 What is the sequence of construtor and destructor call in inheritance?
// 29 Can alias of pointer variable is possible?
// 30 What is casting?
// 31 What is the difference bewteen reinterpret cast and static cast?
// 32 What is const cast?
// 33 What is constant pointer and pointer to constant?
// 34 What is virtual pointer and vtable when it is created?
// 35 What is downcasting and when it is uesd?
// 36 What is the significance of dynamic casting?
// 37 what is auto keyword why it is used?
// 38 What is nullptr?
// 39 What is the use of container class in STL?
// 49 What is the use of weak pointer?
// 41 What is the difference bewteem map() and map[] in STL?
// 42 Difference between vector and list?
// 43 Differenec between inline and macros function?
// 44 What is stack overflow?
// 45 Difference between class and structure?
// 46 What is polymorphism in C++?
// 47 What is function overridding?
// 48 Use of unique pointer inside singleton class?
// 49 What is mutable class?
// 59 What is memory layout ?
// 51 What are different datatype specifier?
// 52 Is Recusrion possible with inline function?
// 53 Can static function be called by class object?
// 54 What is name mangling in C++?
// 55 What is alias?