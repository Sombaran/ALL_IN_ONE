// find Peak Element

#include <iostream>
using namespace std;

int find_peak(int array[], int size)
{
    int value = array[0];
    int i;
    for (i = 1; i < size; i++)
    {
        if (array[i] > value)
        {

            value = array[i];
        }
        cout << value << " " << array[i] << endl;
    }
    return value;
}
int main()
{
    int array[] = {1, 4, 5, 17, 9, 3, 6, 8};
    int size = sizeof(array) / sizeof(array[0]);
    int value = find_peak(array, size);
    if (value == -1)
        cout << "wrong" << endl;
    else
        cout << "value: " << value << endl;
    return 0;
}
