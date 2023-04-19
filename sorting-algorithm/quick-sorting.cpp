// Quick-Sorting-Algorithm

//     Sorting is a way of arranging items in a systematic manner.
//     Quicksort is the widely used sorting algorithm that makes (n log n)
//     comparisons in average case for sorting an array of n elements.
//     It is a faster and highly efficient sorting algorithm.

//     This algorithm follows the divide and conquer approach.
//     Divide and conquer is a technique of breaking down the algorithms into subproblems,
//     then solving the subproblems, and combining the results back together to solve the original problem.

//     Divide:
//         In Divide, first pick a pivot element. After that,
//         partition or rearrange the array into two sub-arrays such that each element in the left sub-array is less than
//         or equal to the pivot element and each element in the right sub-array is larger than the pivot element.

//     Conquer:
//         Recursively, sort two subarrays with Quicksort.

//     Combine:
//         Combine the already sorted array.

//     Quicksort picks an element as pivot, and then it partitions the given array around the picked pivot element.
//     In quick sort, a large array is divided into two arrays in which one holds values that are smaller
//     than the specified value (Pivot), and another array holds the values that are greater than the pivot.

// Quick - sort complexity
//     1. Time Complexity
//         Case	        Time Complexity
//         Best Case	    O(n*logn)
//         Average Case	O(n*logn)
//         Worst Case	    O(n2)

//     Best Case Complexity -
//         In Quicksort, the best-case occurs when the pivot element is the middle element or near to the middle element.
//         The best-case time complexity of quicksort is O(n*logn).
//     Average Case Complexity -
//         It occurs when the array elements are in jumbled order that is not properly ascending and not properly descending.
//         The average case time complexity of quicksort is O(n*logn).
//     Worst Case Complexity -
//         In quick sort, worst case occurs when the pivot element is either greatest or smallest element.
//         Suppose, if the pivot element is always the last element of the array,
//         the worst case would occur when the given array is sorted already in ascending or descending order.
//         The worst-case time complexity of quicksort is O(n2).

//     Though the worst-case complexity of quicksort is more than other sorting algorithms
//     such as Merge sort and Heap sort, still it is faster in practice.
//     Worst case in quick sort rarely occurs because by changing the choice of pivot,
//     it can be implemented in different ways.
//     Worst case in quicksort can be avoided by choosing the right pivot element.

//     2. Space Complexity
//             Space Complexity	O(n*logn)
//             Stable	            NO
//     The space complexity of quicksort is O(n*logn).

// Quick sort in C++

#include <iostream>
using namespace std;

void swap(int *a, int *b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

/* function to print an array */
void printArr(int a[], int n)
{
    int i;
    for (i = 0; i < n; i++)
        cout << a[i] << " ";
    cout << endl;
}

/* function that consider last element as pivot,
place the pivot at its exact position, and place
smaller elements to left of pivot and greater
elements to right of pivot.  */

int partition(int A[], int start, int end)
{
    int i = start;
    int piv = A[end];
    int c;
    for (c = start; c < end; c++)
    {
        if (A[c] < piv)
        {
            swap(A[c], A[i]);
            i++;
        }
    }
    swap(A[i], A[c]); // put the pivot element in its proper place
    return i;         // return the position of the pivot
}

void quick_sort(int A[], int s, int e)
{
    if (s < e)
    {
        int pi = partition(A, s, e);
        quick_sort(A, s, pi - 1); // sorts the left side OF pivot.
        quick_sort(A, pi + 1, e); // sorts the right side of pivot.
    }
}

int main()
{
    int a[] = {23, 8, 28, 13, 18, 26};
    int n = sizeof(a) / sizeof(a[0]);
    cout << "Before sorting array elements are - \n";
    printArr(a, n);
    quick_sort(a, 0, n - 1);
    cout << "\nAfter sorting array elements are - \n";
    printArr(a, n);

    return 0;
}