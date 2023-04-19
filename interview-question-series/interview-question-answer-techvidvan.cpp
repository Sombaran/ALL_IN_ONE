//  Link - https://techvidvan.com/tutorials/c-interview-questions-and-answers/

//-----------------------------------------------------------------------------------
// Q.1 Write a program to check for equality without using arithmetic and comparison operators?
// Ans.

#include <iostream>
using namespace std;

int example1()
{
    int a = 16;
    int b = 13;
    if ((a ^ b))
        cout << "a is not equal to b" << endl;
    else
        cout << "a is equal to b" << endl;
    return 0;
}
// Ans - a is not equal to b
//-----------------------------------------------------------------------------------

// Q.2 What is the output of the following program?

int example2()
{
    extern int i;
    cout << i << endl;
    return 0;
}
int i = 20;

// Ans. 20
//-----------------------------------------------------------------------------------

// Q.3 What is the output of the following program?

int foo(unsigned int n, unsigned int r)
{
    if (n > 0)
        return (n % r + foo(n / r, r));
    // n % r This will be 22
    // (n / r) This part will be zero 0 so next time it enter into else case
    else
        return 0;
}
int example3()
{
    int res;
    res = foo(22, 30);
    cout << res << endl;
    return 0;
}
// Ans. 22
//-----------------------------------------------------------------------------------

// Q.4 How to allocate a 2d array dynamically in C++?

int example4()
{
    int row = 2, col = 2;
    int *a = new int[row * col];

    int i, j, count = 0;
    for (i = 0; i < row; i++)
        for (j = 0; j < col; j++)
            *(a + i * col + j) = count++;

    for (i = 0; i < row; i++)
        for (j = 0; j < col; j++)
            printf("%d ", *(a + i * col + j));
    cout << endl;
    delete[] a;
    return 0;
}
//-----------------------------------------------------------------------------------
// Q.5 Can you write a program to generate random numbers in C++?
// Ans.

#include <iostream>
#include <random>
using namespace std;
int example5()
{
    int max = 15, min = 5, i;
    int range = max - min + 1;
    for (i = min; i < max; i++)
    {
        int num = rand() % range + min;
        cout << num << " ";
    }
    cout << endl;
    return 0;
}
// Output:-
// It will generate 8 digit random numbers.
//-----------------------------------------------------------------------------------

// Q.6 Why do you use the namespace std in C++?
// Ans. A computer needs to know the code for the cout, cin functionalities
//     it also needs to know which namespace they are defined.
//     But it is not necessary to write namespace, you can simply use scope resolution(::).
//     It works the same as the namespace.
//     Usage of namespace is a bad practice.
//     The problem with putting ‘using namespace’ in the header files of your classes is that it forces
//         anyone who wants to use your classes to also be using those other namespaces.
//-----------------------------------------------------------------------------------
// Q.7 How to remove segmentation faults in C++?
// Ans. It basically means that whenever a piece of code tries to perform a
//         read and write operation in a read only location in memory.
//         It indicates an error in memory corruption.
//     solutions behind segmentation fault:-
//     1. Reason:- Accessing an address that is freed.
//         Solution:- Before freeing the pointer, check the assignment or any operation which is required to perform.
//     2. Reason:- Accessing out of array index bounds.
//         Solution:- Correct the array bound.
//     3. Reason:- Dereferencing uninitialized pointer
//         Solution:- A pointer must point to valid memory before accessing it.
//     4. Reason:- Stack Overflow
//         Solution:- Having a base condition to return from the recursive function.
//-----------------------------------------------------------------------------------

// Q.8 How to sort a string using C++?
// Ans. You can use the sort() function to sort a string in C++.
#include <algorithm>
int example8()
{
    string str = "swami";
    // cout << "Enter String" << endl;
    // cin >> str;
    sort(str.begin(), str.end());
    cout << "Sorted String " << endl;
    cout << str << endl;
    return 0;
}
// Output:-
//     Enter String
//     swami
//     Sorted String
//     aimsw

//-----------------------------------------------------------------------------------

// Q.9 Why is C++ an object oriented programming language?
// Ans. C++ always views a problem in terms of objects rather than the procedure of doing it.
//     In object oriented programming, objects represent an entity that can store data and has its interface through functions.
//     The advantages of oop programming is that it makes the program less complex and enhances readability.
//     Components are reusable and extendible.
//     It is also very much desired for maintenance and long term support.

//     All the member functions of an object may not be used thus it introduces code overhead.
//     If you want to write large business logics and large applications or games then you can make use of the oop programming.
//-----------------------------------------------------------------------------------

// Q.10 How would you implement the Is-a and Has-a class relationships?
// Ans. A specialized class has Is-a relationship with another class.
//     It is best implemented with inheritance.
//     A class may have an instance of another class.
//     For example, an employee ‘has’ a salary, therefore the employee has the ‘HAS-A’
//         relationship with the salary class.
//     It is best implemented by setting an object of the Salary class in the Employee class.
//     There are other relationships also.
//         When one class uses the service of another then it is known as the USES-A relationship.
//-----------------------------------------------------------------------------------

// Q.11 Why a template is a better solution than a base class?
// Ans. When you design a generic class to contain or manage objects of other types
//     when the format and the behaviour of the other types are not important to their management
//     then you can use a template rather than using a base class.
//     You can also use templates when those other types are not known to the designer of the container or managed class.

// Example:-

template <class T>
T add(T &i, T &j)
{
    T res = i + j;
    return res;
}
int example11()
{
    int x = 2;
    int y = 3;
    cout << "The value of the addition of is: " << add(x, y);
    return 0;
}
// Output:-
//     The value of the addition of is: 5
//-----------------------------------------------------------------------------------

// Q.12 In C++, how to write to a file?
// Ans.

#include <fstream>
int example12()
{
    fstream exp;
    exp.open("Tech.txt", ios::out);
    if (!exp)
    {
        cout << "File creation failed!" << endl;
    }
    else
    {
        cout << "Created a new file!" << endl;
        exp << "TechVidvan Tutorial: C++ File Handling!" << endl;
        exp.close();
    }
    return 0;
}
//-----------------------------------------------------------------------------------

// Q.13 Instead of reference of the object, can a copy constructor accept an object of the same class as a parameter?
// Ans. No, it cannot be done.
//     It will generate an error if you specify a copy constructor with a first argument which is an object and not a reference.
//     This is specified in the definition of the copy constructor itself.

//     Copy constructor by value:
//     Syntax :
//         Tech(const Tech ob)
//     It will throw you a compile error.

//     Copy constructor by reference:
//     Syntax :
//         Tech(const Tech& ob)

class Tech
{
public:
    Tech() {}
    Tech(const Tech &ob)
    {
        printf("Testing\n");
    }
};
int example13()
{
    Tech ob1;
    Tech ob2 = ob1;
    return 0;
}
//-----------------------------------------------------------------------------------

// Q.14 If a constructor fails, how can you handle it?
// Ans. You can throw an exception to handle the failure of a constructor.
//     Constructor don’t have a return type.
//     That’s why it is not possible to use return types.
//     All you can do to handle the failure of a constructor is to throw an exception.

//     You should use exceptions to signal failure in constructors.
//     The memory related with the object is cleaned up if the constructor finishes by throwing an exception.
class A
{
};
class B
{
};
void fun()
{
    A a;
    B *pp = new B();
}
// If A:A() throws an error then the memory for x will not leak.
// And if B:B() throws an error then the memory for *p will not leak.
//-----------------------------------------------------------------------------------

// Q.15 State the difference between a copy constructor and an overloaded assignment operator?
// Ans. An overloaded assignment operator helps you to assign the contents of an existing object to
//         another existing object of the same class.

//     A copy constructor helps you to construct a new object by using the content of the argument object.
//     It is called when an object is passed by value.
//     Copy constructor is itself a function.
//     You can also perform a user-defined copy constructor.
//-----------------------------------------------------------------------------------

// Q.16 Can you call a destructor explicitly on a local variable?
// Ans. No, you cannot call a destructor explicitly on a local variable.
//     It will be called again at the close } of the block in which the local is created.
//     It happens automatically. And there is no way to stop it from occurring.
//     You will get some bad results if you call a destructor on the same object.
//-----------------------------------------------------------------------------------

// Q.17 Why should you use the "placement_new"?
// Ans. There are many types of placement_new.
//     Though a simple way to use this is to place an object at a particular location in memory.
//     It can be done by supplying the place as a pointer parameter to the new part of a new expression.
//-----------------------------------------------------------------------------------

// Q.18 State the difference between List x; and List x();?
// Ans. There is a big difference between these two.
//     Let’s say that List is the name of the same class.
//     The function func() declares a local list object called x:

void func11()
{
    // List x; // Local object named x
}
void fun1()
{
    // List x(); // Function named x
}
//     In the above example, the func() function declares a local List object called x.
//     And, the function fun() declares a function named x() that returns a List.
//     So, a function within another function.
//-----------------------------------------------------------------------------------

// Q.19 What will happen when you create and destroy a derived-class object?
// Ans. The constructor of the base class is called to initialize the data members inherited from the base class.
//     The constructor of the derived class is called to initialize the data members which are added in the derived class.
//     It is then reusable.
// When the object is destroyed then the destructor of the derived class is called on the object first.
// Then the destructor of the base class is called on the object.
//-----------------------------------------------------------------------------------

// Q.20 Why should you use new instead of malloc()?
// Ans. Constructor and Destructors:-
//     Unlike malloc(sizeof(Anything)),
//     new Anything() calls Anything’s constructor.
//     Also delete p calls the destructor of *p.

//     Type Safety:- malloc() returns a void* which is not type safe.
//     new returns a pointer of the right type.

//     Overridability:- The class can override the new operator.
//         And malloc() is not overridable.
//-----------------------------------------------------------------------------------

// Q.21 How to allocate or deallocate an array of things?
// Ans.
// {
//     p = new T[n]
//     delete[] p:
//     Anything* p = new Anything[100];

//     delete[] p;
// }
//     So, when you allocate an array of objects using new, you must use [] in the delete statement.
//     It is useful to differentiate between a pointer to a thing and a pointer to an array of things.
//-----------------------------------------------------------------------------------

// Q.22 Can you tell what a dangling pointer is?
// Ans. It takes place when you use the address of an object after its lifetime is over.
//     It can happen in situations such as returning addresses of the automatic variables
//     from a function or using the address of the memory block after it is freed.

//     This is the most common bug that is related to pointers and memory management.
//     It is also known as wild pointers.
//     You can avoid the errors caused by the dangling pointer by initializing the pointer to the NULL value.
int *f()
{
    static int x = 10; // if are not using the satic then the dangling pointer will create
    return &x;
}
int example22()
{
    int *ptr = f();
    printf("%d\n", *ptr);
    return 0;
}
//-----------------------------------------------------------------------------------

// Q.23 Write a program to check if the word is palindrome or not!
// Ans. A string is said to be palindrome if the reverse of the string is the same as the string.

#include <iostream>
#include <string.h>
int example23()
{
    char s1[] = "nun";
    int i, j, len = 0, flag = 0;
    len = sizeof(s1) / sizeof(s1[0]);
    cout << "len: " << (len) << endl;

    // Solution one - swap element check s1
    len = len - 1;
    for (int i = 0; i < len / 2; i++)
    {
        if (s1[i] == s1[len - i - 1])
        {
            cout << s1[i] << " = " << s1[len - i - 1] << endl;
            flag = 1;
        }
        else
            flag = 0;
    }
    if (flag == 1)
        cout << "It is a palindrome!" << endl;
    else
        cout << "It is not a palindrome!" << endl;

    return 0;
}
//-----------------------------------------------------------------------------------

// Q.24 How to create a thread using C++ programming language?
// Ans.

#include <thread>

using namespace std;
char *st = "Child thread";
void threadfunction(char *st)
{
    cout << "Created child thread: " << (char *)st << endl;
}
int example24()
{
    thread t1(threadfunction, st);
    cout << "Created Main thread!" << endl;
    t1.join();
    return 0;
}
// Output:
//     Created Main thread!
//     Created child thread: Child thread
//-----------------------------------------------------------------------------------

// Q.25 State the difference between object oriented programming and procedural programming.
// Ans.

//     - Procedural Programming
//         = Object Oriented Programming
//     - In Procedural programming, there is no data security.
//         = Object oriented programming provides data security by using access specifiers and it is more secure and efficient.
//     - In procedural programming, the program is divided into functions.
//         = The program is divided into small blocks called objects in object oriented programming.
//     - It works on the top-down approach.
//         = It works on the bottom-up approach.
//     - C supports procedural programming.
//         = C++ supports object oriented programming.
//     - Function is more important than data.
//         = Here, data is more important than functions.
//     - It is based on an unreal world.
//         = It is based on the real world.
//-----------------------------------------------------------------------------------

// Q.26 Can you predict the output of the following program?

int example26()
{
    char let = 'T', new_let;
    int val = 20, res;
    cout << "ASCII of 'T': " << (int)let << endl;
    if (islower(let))
        new_let = toupper(let);
    else
        new_let = val + let;
    res = new_let + 21;
    cout << res << endl;
    return 0;
}
//-----------------------------------------------------------------------------------

// Q.27 Write an algorithm to reverse a linked list.
// Ans.

// template void linkt::reversing()
// {
//     node ptr = head;
//     node nextp = ptr->_next;
//     while (nextp)
//     {
//         node tmp = nextp->_next;
//         nextp->_next = ptr;
//         ptr = nextp;
//         nextp = tmp;
//     }
//     head->_next = 0;
//     head = ptr;
// }
//-----------------------------------------------------------------------------------

// Q.28 Can you tell me your way of debugging when you came across a problem?
// Ans. You can debug with amazing tools like:-

//     GDB, DBG, Visual Studio.
//     You can make use of tusc to trace the last system call before crashing.
//     Analyzing the core dump
//     It is very helpful to put debug statements in the program code.
//     Proofreading your code carefully.
//-----------------------------------------------------------------------------------

// Q.29 Can you find the error in the following code?

int example29()
{
    const int data = 230;
    const int *const ptr = &data;
    // (*ptr)++; // error: increment of read-only location ‘*(const int*)ptr’
    int value = 20;
    // ptr = &value; // error: assignment of read-only variable ‘ptr’
}
// Ans. It is not possible to change the value of a constant variable through the entire code.
//  The statements ‘(*ptr)++’ and ‘ptr=&value’ cannot modify a constant object as well.
//-----------------------------------------------------------------------------------

// Q.30 In C++, which operators can or cannot be overloaded?
// Ans. In C++, we can overload most of the operators except :: and .*.
//-----------------------------------------------------------------------------------

// Q.31 Can you guess the output of the following program code?

int example31()
{
    int positive = -2;
    try
    {
        if (positive < 0)
        {
            throw "It must be a positive number!";
        }
        cout << positive;
    }
    catch (const char *msg)
    {
        cout << "error: " << msg << endl;
    }
    return 0;
}
// Output:
//     Ans. error: It must be a positive number!
//-----------------------------------------------------------------------------------

// Q.32 What will be the output of the following program?
class Tech1
{
    int t;

public:
    virtual void print() = 0;
};
class Random : public Tech1
{
public:
    void print()
    {
        cout << "Random Class" << endl;
    }
};
int example32()
{
    // Tech1 t;   // error: cannot declare variable ‘t’ to be of abstract type ‘Tech1’
    // t.print(); // error: ‘a’ was not declared in this scope
    return 0;
}
// Ans. The above program will give you an error because
//      C++ does not allow you to declare a normal object for an abstract class.
//      Can not create object of the abstract class.
//-----------------------------------------------------------------------------------

// Q.33 Can you guess the output of the following program code?
int example33()
{
    int num, val = 3;
    num = (100 % val ? val + 1 : val - 1);
    // 100 % val => 33 means its non zero means
    // its true so it will return val + 1 => 4
    cout << num << endl;
    return 0;
}
// Ans. 4
//-----------------------------------------------------------------------------------
int main()
{
    example1();
    example2();
    example3();
    example4();
    example5();
    example8();
    example11();
    example12();
    example13();
    example22();
    example23();
    example24();
    example26();
    example29();
    example31();
    example32();
    example33();
    return 0;
}