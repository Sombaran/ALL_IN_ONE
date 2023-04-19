// Ternary Searching
//     Like the binary search, it also separates the lists into sub-lists.
//     This procedure divides the list into three parts using two intermediate mid values.
//     As the lists are divided into more subdivisions, so it reduces the time to search a key value.

//     The complexity of Ternary Search Technique:
//         Time Complexity:    O(log3 n)
//         Space Complexity:   O(1)

#include <iostream>
using namespace std;

int ternarySearch(int array[], int beg, int end, int key)

{
    while (end >= beg)
    {

        // Find the mid1 and mid2
        int mid1 = beg + (end - beg) / 3;
        int mid2 = end - (end - beg) / 3;

        // Check if key is present at any mid
        if (array[mid1] == key)
        {
            return mid1;
        }
        if (array[mid2] == key)
        {
            return mid2;
        }

        // Since key is not present at mid,
        // check in which region it is present
        // then repeat the Search operation
        // in that region

        if (key < array[mid1])
        {
            // The key lies in between beg and mid1
            end = mid1 - 1;
        }
        else if (key > array[mid2])
        {
            // The key lies in between mid2 and r
            beg = mid2 + 1;
        }
        else
        {
            // The key lies in between mid1 and mid2
            beg = mid1 + 1;
            end = mid2 - 1;
        }
    }

    // Key not found
    return -1;
}

int main()
{
    int array[] = {10, 20, 30, 40, 50, 60, 70};
    int value = 500;
    int beg = 0, end = sizeof(array) / sizeof(int);
    int loc = 0;
    if ((loc = ternarySearch(array, beg, end, value)) >= 0)
        cout << "Item found at location: " << loc << endl;
    else
        cout << "Item is not found in the list." << endl;
}