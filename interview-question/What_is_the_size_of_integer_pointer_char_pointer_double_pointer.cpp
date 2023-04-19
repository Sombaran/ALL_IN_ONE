// What is the size of pointer of type integer, char, float, double
// Answer = 4 Bytes

#include <iostream>
using namespace std;

int main()
{
    char *c_ptr = new char('A');
    cout << "Sizeof char: " << sizeof(*c_ptr) << endl;
    cout << "Sizeof char ptr: " << sizeof(c_ptr) << endl;

    int *i_ptr = new int(1202);
    cout << "Sizeof int: " << sizeof(*i_ptr) << endl;
    cout << "Sizeof int ptr: " << sizeof(i_ptr) << endl;

    float *f_ptr = new float(100.11);
    cout << "Sizeof float: " << sizeof(*f_ptr) << endl;
    cout << "Sizeof float ptr: " << sizeof(f_ptr) << endl;

    double *d_ptr = new double(102.15430);
    cout << "Sizeof double: " << sizeof(*d_ptr) << endl;
    cout << "Sizeof double ptr: " << sizeof(d_ptr) << endl;

    return 0;
}

/*
Explaination:
In 32 bit machine system  -  All pointers will be 4 Bytes
In 64 bit machine system  -  All pointers will be 8 Bytes

Because in 32 bit operating system the RAM memory address mapping is done by 4 Bytes and 
in 64 Bit Operating system RAM memory address mapping is done by the 8 Byte

So when you create one char_variable and assign some value to it. 
The total value stored will be 1 Bytes in the char_variable but when you assign the address of char_variable to char_pointer,that time OS will assign 4 Bytes to char_pointer to store the memory address of the char_variable in char_pointer.

This happens with all the type of variable.
so due that for all types (char, float, double, int) of pointers the size of memory address will be same.

*/