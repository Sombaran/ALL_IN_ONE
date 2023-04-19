//   delete keyword in C++

//     Delete is an operator that is used to destroy array and non-array(pointer)
//     objects which are created by new expression.

//     1. Delete can be used by either using Delete operator or Delete [ ] operator.
//     2. New is used for dynamic memory allocation memory created on heap memory.
//     3. Which means Delete operator deallocates memory from heap.
//     4. Pointer to object is not destroyed, value or memory block of pointer destroyed.
//     5. The delete operator has void return type does not return a value.

// Here, Below are examples where we can apply delete operator:
//     1. Deleting Array Objects: We delete an array using [] brackets.

#include <iostream>
using namespace std;

#ifndef NULL

int main()
{
    // Allocate Heap memory
    int *array = new int[10];

    // Deallocate Heap memory
    delete[] array;

    return 0;
}
#endif

//     2. Deleting NULL pointer : Deleting a NULL does not cause any change and no error.

#ifndef NULL
int main()
{
    // ptr is NULL pointer
    int *ptr = NULL;

    // deleting ptr
    delete ptr;

    return 0;
}
#endif

//     3. Deleting pointer with or without value

#ifndef NULL
int main()
{
    // Creating int pointer
    int *ptr1 = new int;

    // Initializing pointer with value 20
    int *ptr2 = new int(20);

    cout << "Value of ptr1 = " << *ptr1 << "\n";
    cout << "Value of ptr2 = " << *ptr2 << "\n";

    delete ptr1; // Destroying ptr1
    delete ptr2; // Destroying ptr2

    return 0;
}
#endif

//    4. Deleting a void pointer

#ifndef NULL
int main()
{
    void *ptr; // Creating void pointer

    delete ptr; // Destroying void pointer

    cout << "ptr deleted successfully";
    return 0;
}
#endif

//    5. deleting memory dynamically allocated by malloc

#ifndef NULL
int main()
{
    // Dynamic memory allocated by using malloc
    int *ptr2 = (int *)malloc(sizeof(int));

    delete ptr2; // It is not recommended to use delete with malloc().

    cout << "ptr2 deleted successfully";

    return 0;
}
#endif

//   6. Deleting variables of User Defined data types:

#ifndef NULL
struct P
{
    static void operator delete(void *ptr, std::size_t sz)
    {
        cout << "custom delete for size " << sz << endl;
        delete (ptr); // ::operator delete(ptr) can also be used
    }
    static void operator delete[](void *ptr, std::size_t sz)
    {
        cout << "custom delete for size " << sz << endl;
        delete (ptr); // ::operator delete(ptr) can also be used
    }
};

int main()
{
    P *var1 = new P;
    delete var1;

    P *var2 = new P[10];
    delete[] var2;
}
#endif


