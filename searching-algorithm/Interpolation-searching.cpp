
// Interpolation Searching Algorithm
//     Given a sorted array of n uniformly distributed values arr[],
//     write a function to search for a particular element x in the array.
//     Linear Search finds the element in O(n) time
//     Jump Search takes O(√n) time and Binary Search take O(Log n) time.

//     The Interpolation Search is an improvement over Binary Search for instances,
//     where the values in a sorted array are uniformly distributed.
//     Binary Search always goes to the middle element to check.
//     On the other hand, interpolation search may go to different locations
//     according to the value of the value being searched.
//     For example, if the value of the value is closer to the last element,
//       interpolation search is likely to beg search toward the end side.

//     For the binary search technique, the lists are divided into equal parts.
//     For the interpolation searching technique, the procedure will try to locate the exact position using interpolation formula.
//     After finding the estimated location, it can separate the list using that location.
//     As it tries to find exact location every time, so the searching time reduces.
//     This technique can find items easily if the items are uniformly distributed.

//     The complexity of Interpolation Search Technique
//     Time Complexity:
//         average case         O(log2(log2 n))
//         Worst case           O(n)

//     Space Complexity:
//         space Complexity     O(1)
//
// Condition to use Interpolation searching 
//          1. Array must be sorted
//          2. Array should be uniform (means difference between the two Consecutive element should be same)

#include <iostream>
using namespace std;

int interpolationSearch(int array[], int beg, int end, int value)
{
    end = end - 1;
    while (beg <= end)
    {
        int mid = beg + (((value - array[beg]) * (end - beg)) / (array[end] - array[beg]));

        if (array[mid] > value)
            end = mid - 1;

        if (array[mid] < value)
            beg = mid + 1;
        else // it means (array[mid] == value)
            return mid + 1;
    }
    return -1;
}

int main()
{
    int array[] = {10, 20, 30, 40, 50, 60, 70};
    int value = 40;
    int beg = 0, end = sizeof(array) / sizeof(array[0]);

    cout << "Finding element by interpolation Search" << endl;
    int valuePos = interpolationSearch(array, beg, end, value);
    if (valuePos == -1)
        cout << "Element not found in array" << endl;
    else
        cout << "Element found in array at: " << valuePos << " value is : " << array[valuePos - 1] << endl;
}

// Output
