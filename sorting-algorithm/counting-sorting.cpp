// Counting Sort Algorithm
//         This sorting technique doesn't perform sorting by comparing elements.
//         It performs sorting by counting objects having distinct key values like hashing.
//         After that, it performs some arithmetic operations to calculate each object's index position in the output sequence. Counting sort is not used as a general-purpose sorting algorithm.

//         Counting sort is effective when range is not greater than number of objects to be sorted.
//         It can be used to sort the negative input values.

//     Counting sort complexity

//         1. Time Complexity
//             Case	        Time Complexity
//             Best Case	    O(n + k)
//             Average Case	O(n + k)
//             Worst Case	    O(n + k)

//         Best Case Complexity -
//             It occurs when there is no sorting required, i.e. the array is already sorted.
//             The best-case time complexity of counting sort is O(n + k).

//         Average Case Complexity -
//             It occurs when the array elements are in jumbled order that is not properly ascending and not properly descending.
//             The average case time complexity of counting sort is O(n + k).

//         Worst Case Complexity -
//             It occurs when the array elements are required to be sorted in reverse order.
//             That means suppose you have to sort the array elements in ascending order, but its elements are in descending order.
//             The worst-case time complexity of counting sort is O(n + k).

//     In all above cases, the time complexity of counting sort is same.
//     This is because the algorithm goes through n+k times, regardless of how the elements are placed in the array.

//     Counting sort is better than the comparison-based sorting techniques because
//         there is no comparison between elements in counting sort.
//         But, when the integers are very large the counting sort is bad because arrays of that size have to be created.

//         2. Space Complexity
//             Space Complexity	O(max)
//             Stable	            YES
//         The space complexity of counting sort is O(max). The larger the range of elements, the larger the space complexity.

//  NOTES:
//         This is efficient sorting algorithm if range is almost similar to the number of objects to be sorted.
//         It is best when the range is equal to number of objects to be sorted.
//         It is used in radix sort as a part of algorithms.

#include <iostream>

using namespace std;

int getMax(int array[], int n)
{
    int max = array[0];
    for (int i = 0; i < n; i++)
    {
        if (array[i] > max)
            max = array[i];
    }
    cout << "Max: " << max << endl;
    return max; // maximum element from the array
}

void counting_sort(int arr[], int size, int max)
{
    int count[max + 1] = {0}, i, j;
    for (i = 0; i < size; i++)
    {
        count[arr[i]] = count[arr[i]] + 1;
    }

    int pos = 0;
    for (i = 0; i <= max; i++)
    {
        for (j = 1; j <= count[i]; j++)
        {
            cout << i << " ";
            if (j != 0)
            {
                arr[pos] = i;
                pos += 1;
            }
        }
    }
    cout << endl;
}
void printArr(int array[], int n) /* function to print the array */
{
    int i;
    for (i = 0; i < n; i++)
        cout << array[i] << " ";
    cout << endl;
}

int main()
{
    int array[] = {31, 11, 42, 7, 30, 11};
    int size = sizeof(array) / sizeof(array[0]);
    cout << "Before sorting array elements are - \n";
    printArr(array, size);
    int value = getMax(array, size);
    counting_sort(array, size, value);
    cout << "After sorting array elements are - \n";
    printArr(array, size);
    return 0;
}