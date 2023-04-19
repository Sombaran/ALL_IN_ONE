
//   Quick-Sorting-Algorithm

//     In selection sort, the smallest value among the unsorted elements of the array is selected in every pass and
//     inserted to its appropriate position into the array.
//     It is also the simplest algorithm.
//     It is an in-place comparison sorting algorithm.
//     In this algorithm, the array is divided into two parts, first is sorted part, and another one is the unsorted part.
//     Initially, the sorted part of the array is empty, and unsorted part is the given array.
//     Sorted part is placed at the left, while the unsorted part is placed at the right.

//     In selection sort, the first smallest element is selected from the unsorted array and placed at the first position.
//     After that second smallest element is selected and placed in the second position.
//     The process continues until the array is entirely sorted.

//     Selection sort is generally used when -
//         1. A small array is to be sorted
//         2. Swapping cost doesn't matter
//         3. It is compulsory to check all elements

//     Selection sort complexity

//     1. Time Complexity
//         Case	        Time Complexity
//         Best Case       O(n2)
//         Average Case    O(n2)
//         Worst Case      O(n2)

//     Best Case Complexity -
//         It occurs when there is no sorting required, i.e. the array is already sorted. The best-case time complexity of selection sort is O(n2).
//     Average Case Complexity -
//         It occurs when the array elements are in jumbled order that is not properly ascending and not properly descending.
//         The average case time complexity of selection sort is O(n2).
//     Worst Case Complexity -
//         It occurs when the array elements are required to be sorted in reverse order.
//         That means suppose you have to sort the array elements in ascending order, but its elements are in descending order.
//         The worst-case time complexity of selection sort is O(n2).

//     2. Space Complexity
//     Space Complexity    O(1)
//     Stable              YES

//     The space complexity of selection sort is O(1).
//     It is because, in selection sort, an extra variable is required for swapping.

#include <iostream>
using namespace std;

void swap(int *x, int *y)
{
    int tmp = *x;
    *x = *y;
    *y = tmp;
}
void selection_sort(int Arr[], int n)
{
    int i, j, min_id;
    for (i = 0; i < n - 1; i++)
    {
        min_id = i;
        for (j = i + 1; j < n; j++)
        {
            if (Arr[j] < Arr[min_id])
            {
                min_id = j;
            }
        }
        swap(&Arr[min_id], &Arr[i]);
    }
}
int main()
{
    int Arr[] = {5, 1, 3, 2, 4};
    int n = sizeof(Arr) / sizeof(Arr[0]);
    selection_sort(Arr, n);
    for (int i = 0; i < n; i++)
        cout << Arr[i] << " ";
    cout << endl;
    return 0;
}