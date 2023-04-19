// Find the minimum and maximum element in an array
#include <iostream>
using namespace std;
pair<int, int> find_min_max(int array[], int size)
{
    int min = array[0], max = array[0];
    for (int i = 1; i < size; i++)
    {
        if (min < array[i])
            min = array[i];

        if (max > array[i])
            max = array[i];
    }
    return pair<int, int>(min, max);
}

int main()
{
    int array[] = {1, 4, 5, 17, 9, 3, 6, 8};
    int size = sizeof(array) / sizeof(array[0]);
    pair<int, int> value = find_min_max(array, size);
    cout << "min: " << value.first << endl;
    cout << "max: " << value.second << endl;
    return 0;
}
