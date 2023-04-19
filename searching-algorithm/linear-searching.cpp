
// Liner Searching Algorithm
// Liner Searching is the process of finding some particular element in the list.
// If the element is present in the list, then the process is called successful, and the process returns the location of that element;
// otherwise, the search is called unsuccessful.

// Two popular search methods are
//     1. Linear Search
//     2. Binary Search.

// 1. Linear Search
//     Linear search is also called as sequential search algorithm.
//     It is the simplest searching algorithm.
//     In Linear search, we simply traverse the list completely and match each element of the list with
//     the item whose location is to be found.
//     If the match is found, then the location of the item is returned;
//     otherwise, the algorithm returns NULL.

//     It is widely used to search an element from the unordered list,
//     i.e., the list in which items are not sorted. The worst-case time complexity of linear search is O(n).

//     The steps used in the implementation of Linear Search are listed as follows -

//         First, we have to traverse the array elements using a for loop.
//         In each iteration of for loop, compare the search element with the current array element, and -
//         If the element matches, then return the index of the corresponding array element.
//         If the element does not match, then move to the next element.
//         If there is no match or the search element is not present in the given array, return -1.

// Linear Search complexity
// 1. Time Complexity
//     Case	         Time Complexity
//     Best Case	     O(1)
//     Average Case	     O(n)
//     Worst Case        O(n)

// Best Case Complexity -
//     In Linear search, best case occurs when the element we are finding is at the first position of the array.
//     The best-case time complexity of linear search is O(1).

// Average Case Complexity -
//     The average case time complexity of linear search is O(n).

// Worst Case Complexity -
//     In Linear search, the worst case occurs when the element we are looking is present at the end of the array.
//     The worst-case in linear search could be when the target element is not present in the given array,
//     and we have to traverse the entire array.
//     The worst-case time complexity of linear search is O(n).
//     The time complexity of linear search is O(n) because every element in the array is compared only once.

// 2. Space Complexity
//     Space Complexity	O(1)
//     The space complexity of linear search is O(1).

#include <iostream>
using namespace std;

int linerSearch(int array[], int size, int value)
{
    for (int i = 0; i < size; i++)
    {
        if (array[i] == value)
            return i + 1;
    }
    return -1;
}

int main()
{

    int array[] = {10, 62, 45, 17, 91, 34, 62, 67};
    int value = 62;
    int size = sizeof(array) / sizeof(int);

    cout << "Finding element: " << value << endl;
    int valuePos = linerSearch(array, size, value);
    if (valuePos == -1)
        cout << "Element not found in array" << endl;
    else
        cout << "Element found in array at: " << valuePos << " value is: " << array[valuePos - 1] << endl;
    return 0;
}
