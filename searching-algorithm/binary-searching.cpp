
// Binary Search Algorithm
// Binary Searching is the process of finding some particular element in the list. If the element is present in the list,
//  then the process is called successful, and the process returns the location of that element.
//  Otherwise, the search is called unsuccessful.

// Binary search is the search technique that works efficiently on sorted lists.
// Hence, to search an element into some list using the binary search technique, we must ensure that the list is sorted.
// Binary search follows the divide and conquer approach in which the list is divided into two halves,
//     and the item is compared with the middle element of the list.
// If the match is found then, the location of the middle element is returned.
// Otherwise, we search into either of the halves depending upon the result produced through the match.

// Working of Binary search
// There are two methods to implement the binary search algorithm -
//      1. Iterative method - space complexity will be O(1)
//      2. Recursive method - space complexity will be O(log n)
// The recursive method of binary search follows the divide and conquer approach.

// Binary Search complexity
//     1. Time Complexity
//         Case	Time        Complexity
//         Best Case	    O(1)
//         Average Case	    O(logn)
//         Worst Case	    O(logn)

//     Best Case Complexity -
//         In Binary search, best case occurs when the element to search is found in first comparison,
//         i.e., when the first middle element itself is the element to be searched.
//         The best-case time complexity of Binary search is O(1).

//     Average Case Complexity -
//         The average case time complexity of Binary search is O(logn).

//     Worst Case Complexity -
//         In Binary search, the worst case occurs, when we have to keep reducing the search space till it has only one element.
//         The worst-case time complexity of Binary search is O(logn).

//     2. Space Complexity
//         Space Complexity	O(1)
//     The space complexity of binary search is O(1).

#include <iostream>
using namespace std;

int binarySearchIterative(int array[], int beg, int end, int value)
{
    // end = end - 1;
    while (end >= beg)
    {
        int mid = (beg + end) / 2;

        if (array[mid] > value)
            end = mid - 1;

        if (array[mid] < value)
            beg = mid + 1;
        else // it means (array[mid] == value)
            return mid;
    }
    return -1;
}

int binarySearchRecursive(int array[], int beg, int end, int value)
{
    end = end - 1;
    int mid = 0;

    if (beg <= end)
    {
        mid = (beg + end) / 2;

        if (array[mid] > value)
        {
            end = mid - 1;
            return binarySearchRecursive(array, beg, end, value);
        }
        else if (array[mid] < value)
        {
            beg = mid + 1;
            return binarySearchRecursive(array, beg, end, value);
        }
        else // it means (array[mid] == value)
        {
            return mid + 1;
        }
    }

    return -1;
}

int main()
{

    int array[] = {10, 20, 30, 40, 50, 60, 70};
    int value = 50;
    int beg = 0, end = sizeof(array) / sizeof(int);

    cout << "Finding element by binarySearchRecursive" << endl;
    int valuePos = binarySearchRecursive(array, beg, end, value);
    if (valuePos == -1)
        cout << "Element not found in array" << endl;
    else
        cout << "Element found in array at: " << valuePos << " value is : " << array[valuePos - 1] << endl;

    cout << "Finding element by binarySearchIterative" << endl;
    valuePos = binarySearchIterative(array, beg, end, value);
    if (valuePos == -1)
        cout << "Element not found in array" << endl;
    else
        cout << "Element found in array at: " << valuePos << " value is : " << array[valuePos - 1] << endl;
    return 0;
}
