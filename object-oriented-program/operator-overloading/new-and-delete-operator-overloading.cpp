// C++ new and delete Operator Overloading
//     The new and delete operators can also be overloaded like other operators in C++.
//     New and Delete operators can be overloaded globally.
//     They can be overloaded for specific classes.

//     If these operators are overloaded using member function for a class,
//     It means that these operators are overloaded only for that specific class.
//     If overloading is done outside a class
//         (i.e. it is not a member function of a class),
//     The overloaded ‘new’ and ‘delete’ will be called anytime, if
//     you make use of these operators (within classes or outside classes).
//     This is global overloading.

//     Syntax for overloading the new operator :
//         void* operator new(size_t size);

//     The overloaded new operator receives size of type size_t,
//     which specifies the number of bytes of memory to be allocated.
//     The return type of the overloaded new must be void*.
//     The overloaded function returns a pointer to the beginning
//     of the block of memory allocated.

//     Syntax for overloading the delete operator :
//         void operator delete(void*);

//     The function receives a parameter of type void* which has to be deleted.
//     Function should not return anything.

//     NOTE:
//          Both overloaded new and delete operator functions are static members
//          by default. Therefore, they don’t have access to this pointer.

#include <iostream>
using namespace std;

#ifndef NULL
class student
{
    string name;
    int age;

public:
    student()
    {
        cout << "Constructor is called\n";
    }
    student(string name, int age)
    {
        this->name = name;
        this->age = age;
    }
    void display()
    {
        cout << "Name:" << name << endl;
        cout << "Age:" << age << endl;
    }
    void *operator new(size_t size)
    {
        cout << "Overloading new operator with size: " << size << endl;
        void *p = ::operator new(size);
        // void *p = malloc(size); // will also work fine
        return p;
    }

    void operator delete(void *p)
    {
        cout << "Overloading delete operator " << endl;
        cout << "p: " << p << endl;
        free(p);
        cout << "p: " << p << endl;
        p = NULL;
        cout << "p: " << p << endl;
    }
};

int main()
{
    student *p = new student("Yash", 24);

    p->display();
    delete p;
}

// NOTE:
//     In the above new overloaded function, we have allocated dynamic memory through
//     new operator, but it should be global scope new operator otherwise
//     it will go in recursion call.
//     void *p = new student();
//          this will go in recursion as new will be overloaded again and again
//     void *p = ::new student(); // this is correct
//          this is now call to global scope new operator
#endif

// Global overloading of new and delete operator for primitive data types:

void *operator new(size_t size)
{
    void *p = malloc(size);
    return p;
}

void operator delete(void *p)
{
    free(p);
}

int main()
{
    int size = 5;
    int *p = new int(size);

    for (int i = 0; i < size; i++)
        p[i] = i;

    for (int i = 0; i < size; i++)
    {
        cout << p[i] << endl;
    }
}