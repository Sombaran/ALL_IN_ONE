/*
 * array.cpp
 *
 *  Created on: 16-Mar-2022
 *      Author: Jotirling Swami
 */

// What is an Array?
//     An array is a data structure that stores an element of the same data type sequentially.
//     A C++ array has a fixed-size.

//         You can see an array as a collection of variables of a similar data type.
//         Instead of declaring each variable and assigning it a value individually,
//         you can declare one variable (the array) and add the values of the various variables to it.
//         Each value-added to the array is identified by an index.

//     Why do we need arrays?
//         Arrays are very important in any programming language.
//         They provide a more convenient way of storing variables or a collection of data of a similar data type
//         together instead of storing them separately.
//         Each value of the array will be accessed separately.

//     Declare an array in C++
//         Array declaration in C++ involves stating the type as well as the number of elements to be stored by the array.

//     Syntax:
//         type array-Name [ array-Size ];

//     Rules for declaring a single-dimension array in C++.

//         Type:
//             The type is the type of elements to be stored in the array.
//             it must be a valid C++ data type.
//         Array-Name:
//             The array-Name is the name to be assigned to the array.
//         Array-Size:
//             The array-Size is the number of elements to be stored in the array.
//             It must be an integer and greater than 0.

//     Array Initialization
//         Array initialization is the process of assigning/storing elements to an array.
//         The initialization can be done in a single statement or one by one.
//         Note that the first element in an array is stored at index 0,
//         while the last element is stored at index n-1,
//         where n is the total number of elements in the array.

//             int age[5] = {19, 18, 21, 20, 17};

//         The total number of elements within the { } cannot exceed the value stated within the [ ].
//         The element 19 is at index 0, 18 at index 1, 21 at index 2, 20 at index 3 and 17 at index 4.
//         If you don't state the number of elements to be stored in the array within [ ],
//         Then array will only be large enough to hold the elements added within { }.

//             For example:
//                     int age[] = {19, 18, 21, 20, 17};

//         Types of Arrays
//             1. One dimensional Array
//             2. Multi-dimensional Array
//             3. Pointer to an Array

//     One-Dimensional Array
//         This is an array in which the data items are arranged linearly in one dimension only.
//         It is commonly called a 1-D array. Syntax:

//             datatype array-name[size];

//         The array - name is the name of the array.
//         The size  -  is the number of items to be stored in the array.

#include <iostream>
using namespace std;

int OneD_array()
{
    cout << "OneD_array : " << endl;

    int age[5] = {19, 18, 21, 20, 17};
    for (int x = 0; x < 5; x++)
    {
        cout << "\t";
        cout << age[x] << " ";
    }
    cout << endl;
}

// output:
//     OneD_array :
//         19      18      21      20      17

//     Multi-dimensional Array
//         This is an array in which data items are arranged to form an array of arrays.
//         A multi-dimensional array can have any number of dimensions,
//         but two-dimensional and three-dimensional arrays are common.

//         Syntax:
//             datatype array-name[d1][d2][d3]...[dn];

//     Two Dimensional Array
//         A 2D array stores data in a list with 1-D array.
//         It is a matrix with rows and columns.
//         To declare a 2D array,

//         use the following syntax:
//             type array-Name [ x ][ y ];

//         The type must be a valid C++ data type.
//         where x denotes the number of rows
//         where y denotes the number of columns.

//         int a[2][3] = {
//             {0, 2, 1} ,   /*  row at index 0 */
//             {4, 3, 7} ,   /*  row at index 1 */
//         };

//         In above example, we have a 2D array which can be seen as a 2x3 matrix.
//         There are 2 rows and 3 columns.
//         The element 2 can be accessed as a[0][1]
//             because it is located at the intersection of row indexed 0 and column indexed 1.
//         The element 1 can be accessed as a[1][0]
//             because it is located at the intersection of row indexed 1 and column indexed 2.

int TwoD_array()
{
    cout << "TwoD_array : " << endl;

    int a[3][2] = {{0, 2},
                   {1, 4},
                   {3, 7}};
    for (int i = 0; i < 3; i++)
    {
        cout << "\t";
        for (int j = 0; j < 2; j++)
        {
            cout << "\t";
            cout << a[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
    return 0;
}

// output:
//     TwoD_array :
//                 0       2
//                 1       4
//                 3       7

//     Three - Dimensional Array
//         A 3D array is an array of arrays.
//         Each element in a 3D array is identified by a set of 3 indexes.
//         To access the elements of a 3D array, we use three for loops.

void ThreeD_array()
{
    cout << "ThreeD_array : " << endl;
    int a[2][3][2] = {
        {{4, 8},
         {2, 4},
         {1, 6}},
        {{3, 6},
         {5, 4},
         {9, 3}}};

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            cout << "\t";
            for (int k = 0; k < 2; k++)
            {
                cout << "\t";
                cout << a[i][j][k] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
}
// output:
//     ThreeD_array :
//                     4       8
//                     2       4
//                     1       6

//                     3       6
//                     5       4
//                     9       3

//     Pointer to an Array
//         A pointer is a variable that holds an address.Other than using a pointer to store the address of a variable,
//         we can use it to store the address of an array cell.The name of an array constantly points to its first element.

//         int *age[5];

//         The age is a pointer to $age[0],
//         the address of the first element of an array named age.

int Pointer_to_array()
{
    // an array of 5 elements.
    int age[5] = {19, 18, 21, 20, 17};
    int *p;

    p = age;

    cout << "Print one-d array using pointer: " << endl;

    for (int x = 0; x < 5; x++)
    {
        cout << "*(p + " << x << ") : ";
        cout << *(p + x) << endl;
    }

    cout << "Print one-d array using address: " << endl;

    for (int x = 0; x < 5; x++)
    {
        cout << "*(age + " << x << ") : ";
        cout << *(age + x) << endl;
    }

    cout << "Print two-d array using pointer: " << endl;

    int two_d_array[3][2] = {{0, 2},
                             {1, 4},
                             {3, 7}};

    for (int i = 0; i < 3; i++)
    {
        int *twoptr = two_d_array[i];
        cout << "\t";
        for (int j = 0; j < 2; j++)
        {
            cout << "\t";
            cout << *(twoptr + j) << " ";
        }
        cout << endl;
    }

    cout << "Print three-d array using pointer: " << endl;
    int three_d_array[2][3][2] = {
        {{4, 8},
         {2, 4},
         {1, 6}},
        {{3, 6},
         {5, 4},
         {9, 3}}};

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            cout << "\t";
            int *one_ptr = three_d_array[i][j];
            for (int k = 0; k < 2; k++)
            {
                cout << "\t";
                cout << *(one_ptr + k) << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

    return 0;
}

int main()
{
    OneD_array();
    TwoD_array();
    ThreeD_array();
    Pointer_to_array();

    return 0;
}

//     Advantages of an Array in C++
//         1. Array elements can be traversed easily.
//         2. Easy to manipulate array data.
//         3. Array elements can be accessed randomly.
//         4. Arrays facilitate code optimization; hence, we can perform much work using less code.
//         5. Easy to sort array data.

//     Disadvantages of an Array in C++
//         1. An array has a fixed size; hence, we cannot add new elements to it after initialization.
//         2. Allocating more memory than the requirement leads to wastage memory space,
//              and less allocation of memory can create a problem.
//         3. The number of elements to be stored in an array must be known in advance.

//     Summary
//         1. An array is a data structure that stores elements of the same data type.
//         2. Array elements are stored sequentially.
//         3. The array elements are denoted using their respective indexes. The first element is at index 0,
//             while the last element is at index n-1, where is the total number of array elements.
//         4. The declaration of an array involves defining the data types of the array elements
//             as well as the number of elements to be stored in the array.
//         5. A one-dimensional array stores elements sequentially.
//         6. A two-dimensional array stores elements in rows and columns.
//         7. A three-dimensional array is an array of arrays.
//         8. Elements can be added to an array using their indexes.
//         9. Array elements are accessed using their indexes.
//         10. A multi-dimensional array has more than one dimension.
//         11. The array name points to its first element.
//         12. Arrays have a fixed size, meaning that new elements cannot be added to the array after its initialization.