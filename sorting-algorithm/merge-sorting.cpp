
//   Merge Sorting Algorithm
//         Merge sort is similar to the quick sort algorithm as it uses the divide and
//         conquer approach to sort the elements.
//         It is one of the most popular and efficient sorting algorithm.
//         It divides the given list into two equal halves, calls itself for the two halves
//         and then merges the two sorted halves.
//         We have to define the merge() function to perform the merging.
//         The sub-lists are divided again and again into halves until the list
//         cannot be divided further.
//         Then we combine the pair of one element lists into two-element lists,
//         sorting them in the process.
//         The sorted two-element pairs is merged into the four-element lists, and
//         so on until we get the sorted list.

//     Merge sort complexity
//         1. Time Complexity
//             Case	        Time Complexity
//             Best Case	    O(n*logn)
//             Average Case	O(n*logn)
//             Worst Case	    O(n*logn)

//         Best Case Complexity -
//             It occurs when there is no sorting required, i.e. the array is already sorted.
//             The best-case time complexity of merge sort is O(n*logn).
//         Average Case Complexity -
//             It occurs when the array elements are in jumbled order that is not properly
//             ascending and not properly descending.
//             The average case time complexity of merge sort is O(n*logn).
//         Worst Case Complexity -
//             It occurs when the array elements are required to be sorted in reverse order.
//             That means suppose you have to sort the array elements in ascending order,
//             but its elements are in descending order.
//             The worst-case time complexity of merge sort is O(n*logn).

//         2. Space Complexity
//                 Space Complexity	        O(n)
//                 Stable	                YES
//         The space complexity of merge sort is O(n).
//         It is because, in merge sort, an extra variable is required for swapping.

#include <iostream>
using namespace std;

/* Function to merge the subarrays of a[] */
void merge(int a[], int beg, int mid, int end)
{
    int i, j, k;
    int n1 = mid - beg + 1;
    int n2 = end - mid;

    int LeftArray[n1], RightArray[n2]; // temporary arrays

    /* copy data to temp arrays */
    for (int i = 0; i < n1; i++)
        LeftArray[i] = a[beg + i];
    for (int j = 0; j < n2; j++)
        RightArray[j] = a[mid + 1 + j];

    i = 0;   /* initial index of first sub-array */
    j = 0;   /* initial index of second sub-array */
    k = beg; /* initial index of merged sub-array */

    while (i < n1 && j < n2)
    {
        if (LeftArray[i] <= RightArray[j])
        {
            a[k] = LeftArray[i];
            i++;
        }
        else
        {
            a[k] = RightArray[j];
            j++;
        }
        k++;
    }
    while (i < n1)
    {
        a[k] = LeftArray[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        a[k] = RightArray[j];
        j++;
        k++;
    }
}

void merge_sort(int a[], int beg, int end)
{
    if (beg < end)
    {
        int mid = (beg + end) / 2;
        merge_sort(a, beg, mid);
        merge_sort(a, mid + 1, end);
        merge(a, beg, mid, end);
    }
}

/* Function to print the array */
void printArray(int a[], int n)
{
    int i;
    for (i = 0; i < n; i++)
        cout << a[i] << " ";
    cout << endl;
}

int main()
{
    int a[] = {11, 30, 24, 7, 31, 16, 39, 41};
    int n = sizeof(a) / sizeof(a[0]);
    cout << "Before sorting array elements are - \n";
    printArray(a, n);
    merge_sort(a, 0, n - 1);
    cout << "After sorting array elements are - \n";
    printArray(a, n);
    return 0;
}