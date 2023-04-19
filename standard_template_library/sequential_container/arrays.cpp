/*
 * arrays.cpp
 *
 *  Created on: 16-Mar-2022
 *      Author: Jotirling Swami
 */

// TOPIC: std::array

// Syntax: std::array<T, N> array;

// NOTICE:

//         1. std::array is a container that encapsulates fixed size arrays.
//         2. arraysize is needed at compile time.
//         3. Assign by value is actually by value.
//         4. Access Elements:
//             a. at() - out of bound exception
//             b. []  - no out of bound exception
//             c. front()
//             d. back()
//             e. data() // gives access to the underlying array

// Element access
//     at
//         access specified element with bounds checking
//     operator[]
//         access specified element
//     front
//         access the first element
//     back
//         access the last element
//     data
//         direct access to the underlying array

//     Iterators
//         begin
//         cbegin
//             returns an iterator to the beginning
//         end
//         cend
//             returns an iterator to the end
//         rbegin
//         crbegin
//             returns a reverse iterator to the beginning
//         rend
//         crend
//             returns a reverse iterator to the end

//     Capacity
//         empty
//             checks whether the container is empty
//         size
//             returns the number of elements
//         max_size
//             returns the maximum possible number of elements

//     Operations
//         fill
//             fill the container with specified value
//         swap
//             swaps the contents

#include <iostream>
#include <array>
#include <algorithm>

using namespace std;

void print(array<int, 5> intArray)
{
    for (int elem : intArray)
        std::cout << " " << elem;
    std::cout << '\n';
}
void pointer_func(const int *p, std::size_t size)
{
    std::cout << "data = ";
    for (std::size_t i = 0; i < size; ++i)
        std::cout << p[i] << ' ';
    std::cout << '\n';
}
int main()
{
    std::array<int, 5> intArray = {1, 3, 5, 7, 9}; // initializer list
    std::array<int, 5> intArray1{1, 3, 5, 7, 9};   // uniform initialization
    print(intArray);
    //-----------------------------------------------------------------------------------------------
    // array.at()
    intArray.at(1) = 88;
    std::cout << "Element at index 2 has value " << intArray.at(1) << '\n';

    try
    {
        intArray.at(6) = 666;
    }
    catch (std::out_of_range const &e)
    {
        std::cout << e.what() << '\n';
    }
    std::cout << "intArray:";
    print(intArray);
    //-----------------------------------------------------------------------------------------------
    // array[]
    intArray[0] = 5;
    std::cout << "intArray:";
    print(intArray);
    //-----------------------------------------------------------------------------------------------
    // array.front()
    cout << "front(): " << intArray.front() << endl;
    //-----------------------------------------------------------------------------------------------
    // array.back()
    cout << "back(): " << intArray.back() << endl;
    //-----------------------------------------------------------------------------------------------
    // array.data()
    pointer_func(intArray.data(), intArray.size());
    //-----------------------------------------------------------------------------------------------
    // array.empty()
    cout << "intArray.empty(): " << intArray.empty() << endl;
    //-----------------------------------------------------------------------------------------------
    // array.size()
    cout << "intArray.size(): " << intArray.size() << endl;
    //-----------------------------------------------------------------------------------------------
    // array.max_size()
    cout << "intArray.max_size(): " << intArray.max_size() << endl;
    //-----------------------------------------------------------------------------------------------
    // array.begin()
    cout << "intArray.begin(): " << *intArray.begin() << endl;
    for (std::array<int, 5>::iterator it = intArray.begin(); it != intArray.end(); it++)
        cout << *it << " ";
    cout << endl;
    //-----------------------------------------------------------------------------------------------
    // array.fill()
    std::array<int, 5> newArray;
    newArray.fill({0});
    std::cout << "newArray:";
    print(newArray);
    //-----------------------------------------------------------------------------------------------
    // array.swap()
    newArray.fill({1});
    intArray.swap(newArray);
    std::cout << "intArray:";
    print(intArray);
    std::cout << "newArray:";
    print(newArray);
    //-----------------------------------------------------------------------------------------------
    // array.get<>()
    std::array<int, 3> arr;
    std::get<0>(arr) = 1;
    std::get<1>(arr) = 2;
    std::get<2>(arr) = 3;
    std::cout << "(" << std::get<0>(arr) << ", " << std::get<1>(arr)
              << ", " << std::get<2>(arr) << ")\n";
    //-----------------------------------------------------------------------------------------------
    // std::swap()
    std::array<int, 5> arrayInt1{1, 2, 3};
    std::array<int, 5> arrayInt2{7, 8, 9};

    std::cout << "arrayInt1:";
    print(arrayInt1);
    std::cout << "arrayInt2:";
    print(arrayInt2);

    std::swap(arrayInt1, arrayInt2);
    std::cout << "arrayInt1:";
    print(arrayInt1);
    std::cout << "arrayInt2:";
    print(arrayInt2);
    //-----------------------------------------------------------------------------------------------
    return 0;
}
