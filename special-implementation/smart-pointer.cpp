//  Smart pointers in C++
//         A pointer is used to store the address of another variable.
//         In other words, a pointer extracts the information of a resource that is outside the program (heap memory).

//         We use a copy of the resource, and to make a change, it is done in the copied version.
//         To change the content of the original resource, smart pointers are used.

//     Problems faced with normal pointers
//         Created a class Rectangle with two data members (length and breadth).
//         A function fun() dynamically creates an object of Rectangle.

//         When the function fun() ends, the object p gets destroyed.
//         Since we did not delete p, the memory remains allocated, and this allocated resource will not be available to other variables.

//         In the main(), we execute an infinite loop that will keep creating p objects and allocate resources.
//         This problem turns out into a memory leak to which smart pointers is a solution.

#include <iostream>
using namespace std;

class Rectangle
{ // created a class Rectangle
private:
    int length;  // data member as length of rectangle
    int breadth; // data member as breadth of rectangle
};

void fun() // the function to indicate the problem with normal pointer
{

    Rectangle *p = new Rectangle(); // Create a dynamic object p
}

int normalPointer()
{
    // Infinite Loop
    int a = 10;
    while (a)
    { // Run an infinite loop that will allocate p
        fun();
        a--;
    }
}

//     Smart pointers
//         We will implement smart pointers such that they will release the memory of unused resources.
//         Create a class with a pointer, overloaded operators(->, *) and destructors.
//         The destructor will be automatically called when its object goes out of the scope.
//         Automatically the dynamically allocated memory will be deleted.

class Smart_pointer
{
    int *ptr;

public:
    Smart_pointer()
    {
        ptr = NULL;
        cout << "default constructor called ..." << endl;
    }
    Smart_pointer(int *p)
    {
        ptr = p;
        cout << "parametrised constructor called ..." << endl;
    }

    int &operator*()
    {
        return *ptr;
    }

    void print()
    {
        cout << *ptr << endl;
    }

    ~Smart_pointer()
    {
        delete ptr;
        cout << "destructor called ..." << endl;
    }
};
int int_smart_pointer()
{
    cout << "main start" << endl;
    {
        Smart_pointer sptr(new int(10));
        cout << "*sptr: " << *sptr << endl; // overloaded the * operator and it returns the reference
        sptr.print();
        *sptr = 100;
        sptr.print();
        cout << "*sptr: " << *sptr << endl; // overloaded the * operator and it returns the reference
    }

    // We don't need to call delete ptr: when the object
    // ptr goes out of scope, the destructor for it is automatically
    // called, and destructor does delete ptr.

    cout << "main end" << endl;
    return 0;
}

// Implement Generic smart pointer using the Template concept.

template <class T> // Create a template class
class SmartPtr
{
    T *ptr; // Actual pointer
public:
    // Constructor
    explicit SmartPtr(T *p = NULL) { ptr = p; }

    // Destructor
    ~SmartPtr() { delete (ptr); }

    // Overloading dereferncing operator
    T &operator*() { return *ptr; }

    // Overloading arrow operator so that
    // members of T can be accessed
    // like a pointer (useful if T represents
    // a class or struct or union type)
    T *operator->() { return ptr; }
};

int generic_smart_pointer()
{
    SmartPtr<int> ptr(new int());
    *ptr = 100;
    cout << *ptr;
    return 0;
}

int main()
{
    normalPointer();
    int_smart_pointer();
    generic_smart_pointer();
    return 0;
}