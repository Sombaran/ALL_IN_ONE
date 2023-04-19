
// Exponential Searching Algorithm

// Exponential search involves two steps:

// 1. Find range where element is present
// 2. Do Binary Search in above found range.

// The complexity of Exponential Search Technique
//     Case	            Runtime
//     Best	            O(1)
//     Average	        O(log i)
//     Worst	        O(log i)
//     Auxiliary Space	O(1)

//     Space Complexity:
//     best case       O(1)

// Exponential search is also known as doubling or galloping search.
// This mechanism is used to find the range where the search key may present.
// If L and U are the upper and lower bound of the list, then L and U both are the power of 2.
// For the last section, the U is the last position of the list. For that reason, it is known as exponential.
// After finding the specific range, it uses the binary search technique to find the exact location of the search key.

#include <iostream>
using namespace std;

int binarySearchIterative(int array[], int beg, int end, int value)
{
    end = end - 1;
    while (beg <= end)
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

int exponentialSearch(int array[], int beg, int end, int value)
{
    // Ifvalue is present at first location itself
    if (array[0] == value)
        return 0;

    // Find range for binary search by
    // repeated doubling
    int i = 1;
    while (i < end && array[i] <= value)
        i = i * 2;

    //  Call binary search for the found range.
    beg = i / 2;
    end = min(i, end);
    return binarySearchIterative(array, beg, end, value);
}

// Driver code
int main(void)
{

    int item, value;
    int array[] = {10, 12, 24, 29, 39, 40, 51, 56, 70};
    value = 70;
    int beg = 0, end = sizeof(array) / sizeof(array[0]);
    cout << "Finding element by exponential Search" << endl;
    int posValue = exponentialSearch(array, beg, end, value);

    if (posValue == -1)
        cout << "Element not found in array" << endl;
    else
        cout << "Element found in array at: " << posValue << " value is : " << array[posValue] << endl;
    return 0;
}

// Application:
//     1. ubounded array means array of infinite size.
//     2. It works better than the binary search