// /    What is a Dynamic Array?
//         A dynamic array is quite similar to a regular array, \
//         but its size is modifiable during program runtime.
//         DynamArray elements occupy a contiguous block of memory.

//         Once an array has been created, its size cannot be changed. 
//         However, a dynamic array is different.
//         A dynamic array can expand its size even after it has been filled.

//         During the creation of an array, it is allocated a predetermined amount of memory.
//         This is not the case with a dynamic array as it grows its memory size 
//         by a certain factor when there is a need.

//     Factors impacting performance of Dynamic Arrays
//         The array's initial size and its growth factor determine its performance.

//         Note the following points:
//             If an array has a small size and a small growth factor.
//             It will keep on reallocating memory more often.
//             This will reduce the performance of the array.
//             If an array has a large size and a large growth factor.
//             It will have a huge chunk of unused memory.
//             Due to this, resize operations may take longer. 
//             This will reduce the performance of the array.

//     The new Keyword
//         In C++, we can create a dynamic array using the new keyword.
//         The number of items to be allocated is specified within a pair of square brackets.
//         The type name should precede this. The requested number of items will be allocated.

//     Syntax:
//         pointer_variable = new data_type;

//         The pointer_variable is the name of the pointer variable.
//         The data_type must be a valid C++ data type.

#include <iostream>
using namespace std;
int dynamic_array()
{
    int x, n;
    cout << "Enter the number of items:"
         << "\n";
    cin >> n;
    int *arr = new int(n);
    cout << "Enter " << n << " items" << endl;
    for (x = 0; x < n; x++)
    {
        cin >> arr[x];
    }
    cout << "You entered: ";
    for (x = 0; x < n; x++)
    {
        cout << arr[x] << " ";
    }
    return 0;
}

//     Initializing dynamically allocated arrays
//         It's easy to initialize a dynamic array to 0.

//         Syntax:
//             int *array{ new int[length]{} };

//         In the above syntax, the length denotes the number of elements to be added to the array.
//         Since we need to initialize the array to 0, this should be left empty.

// dynamic array using an initializer list

int dyamic_array_size_0()
{
    int *array;
    array = new int[5]{10, 7, 15, 3, 11};

    cout << "Array elements: " << endl;

    for (int x = 0; x < 5; x++)
    {
        cout << array[x] << endl;
    }

    return 0;
}

//     Resizing Arrays :
//         The length of a dynamic array is set during the allocation time.
//         C++ doesn't have a built-in mechanism of resizing an array once it has been allocated.
//         To overcome this challenge by allocating a new array dynamically,
//         copying over the elements,
//         then erasing the old array.
//         Note: that this technique is prone to errors, hence, try to avoid it.

//     Dynamically Deleting Arrays:
//         A dynamic array should be deleted from the computer memory once its purpose is fulfilled.
//         The delete statement can help you accomplish this.
//         The released memory space can then be used to hold another set of data.
//         However, even if you do not delete the dynamic array from the computer memory,
//         it will be deleted automatically once the program terminates.

//         Note: 
//         To delete a dynamic array from the computer memory, you should use delete[],
//         instead of delete. 
//         The [] instructs the CPU to delete multiple variables rather than one variable.
//         The use of delete instead of delete[] when dealing with a dynamic array 
//         may result in problems.

//         Examples of such problems include memory leaks, data corruption, crashes, etc

int dyamic_array_resize()
{
    int n;
    cout << "How many numbers will you type?"
         << "\n";
    cin >> n;
    int *arr = new int(n);
    cout << "Enter " << n << " numbers" << endl;
    for (int x = 0; x < n; x++)
    {
        cin >> arr[x];
    }
    cout << "arr Print: " << endl;
    for (int x = 0; x < n; x++)
    {
        cout << arr[x] << " ";
    }
    cout << endl;
    // allocate new memory
    int newsize = 0;
    cout << "How many new numbers will you type?"
         << "\n";
    cin >> newsize;
    int *newarr = new int(newsize);
    // copy old data
    int pos = 0;
    for (int x = 0; x < n; x++)
    {
        newarr[x] = arr[x];
        pos = x;
    }

    cout << "newarr Print: " << endl;
    for (int x = 0; x < newsize; x++)
    {
        cout << newarr[x] << " ";
    }
    cout << endl;

    int x = 1;
    cout << "newsize: " << newsize << endl;
    cout << "pos: " << pos << endl;
    cout << "Enter " << newsize - pos << " numbers" << endl;
    for (x = 1; x < newsize - pos; x++)
    {
        cin >> newarr[pos + x];
    }
    cout << endl;

    cout << "newarr Print: " << endl;
    for (int x = 0; x < newsize; x++)
    {
        cout << newarr[x] << " ";
    }
    // delete old array
    cout << "End" << endl;

    delete[] arr;

    return 0;
}

int main()
{
    // dynamic_array();
    dyamic_array_size_0();
    dyamic_array_resize();
    return 0;
}

//     Summary:
//         1. Regular arrays have a fixed size. You cannot modify their size once declared.
//         2. With these types of arrays, the memory size is determined during compile time.
//         3. Dynamic arrays are different. Their sizes can be changed during runtime.
//         4. In dynamic arrays, the size is determined during runtime.
//         5. Dynamic arrays in C++ are declared using the new keyword.
//         6. We use square brackets to specify the number of items to be stored in the dynamic array.
//         7. Once done with the array, we can free up the memory using the delete operator.
//         8. Use the delete operator with [] to free the memory of all array elements.
//         9. A delete without [] frees the memory of only a single element.
//         10. There is no built-in mechanism to resize C++ arrays.
//         11. To initialize an array using a list initializer, we don't use the "=" operator.