
// Jump Search Algorithm
// Jump search also known as block search algorithm
// This a relatively new algorithm for searching an element in a sorted array.
// https://www.studytonight.com/data-structures/jump-search-algorithm

//     The basic idea of this searching technique is that to search fewer number of elements compared to linear search algorithm
//     This can be done by skipping some fixed number of array elements or jumping ahead by fixed number of steps in every iteration.

//     Lets consider a sorted array A[] of size n, with indexing ranging between 0 and n-1.
//     Element x (i.e value) that needs to be searched in the array A[].
//     For implementing this algorithm, a block of size m (i.e step) is also required,
//     that can be skipped or jumped in every iteration. Thus, the algorithm works as follows:

//             Iteration 1: if (x==A[0]), then success, else, if (x > A[0]), then jump to the next block.
//             Iteration 2: if (x==A[m]), then success, else, if (x > A[m]), then jump to the next block. m = sqrtof(size)
//             Iteration 3: if (x==A[2m]), then success, else, if (x > A[2m]), then jump to the next block.
//             At any point in time, if (x < A[km]), then a linear search is performed from index A[(k-1)m] to A[km]

//  Jump Search complexity

//     Time Complexity:
//     time complexity of O(√n).

//     Space Complexity:
//     The space complexity of this algorithm is O(1) since it does not requireany other data structure for its implementation.

#include <iostream>
#include <cmath>

using namespace std;
int jumpSearch(int array[], int size, int value)
{
    int beg = 0;
    int step = sqrt(size); // initializing block size= √(n)

    while (array[step] <= value && step < size)
    {
        // the control will continue to jump the blocks
        beg = step; // shift the block
        step += sqrt(size);

        if (step > size - 1) // if step exceeds the array size
            return -1;
    }

    for (int x = beg; x < step; x++)
    { // linear search in current block
        if (array[x] == value)
            return x + 1; // position of element being searched
    }
    return -1;
}

int main()
{
    int item, value;
    int array[] = {10, 12, 24, 29, 39, 40, 51, 56, 70};
    value = 12;
    int size = sizeof(array) / sizeof(array[0]);

    int posValue = jumpSearch(array, size, value);
    cout << "Elements of Array: ";
    for (int i = 0; i < size; i++)
        cout << array[i] << " ";
    cout << endl;
    cout << "Finding element in array: " << value << endl;
    if (posValue >= 0)
        cout << "Item found at location: " << posValue;
    else
        cout << "Item is not found in the list.";
}

// Output:
//     Elements of Array: 10 12 24 29 39 40 51 56 70
//     Finding element in array: 40
//     Item found at location: 6

//     Key Points to remember about Jump Search Algorithm
//         1. This algorithm works only for sorted input arrays
//         2. Optimal size of the block to be skipped is √n, thus resulting in the time complexity O(√n2)
//         3. The time complexity of this algorithm lies in between linear search (O(n)) and binary search (O(log n))
//         4. It is also called block search algorithm

//     Advantages of Jump Search Algorithm
//         1. It is faster than the linear search technique which has a time complexity of O(n) for searching an element
//
//     Disadvantages of Jump Search Algorithm
//         1. It is slower than binary search algorithm which searches an element in O(log n)
//         2. It requires the input array to be sorted
