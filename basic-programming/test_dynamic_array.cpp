#include <iostream>
using namespace std;

int *Array;
int current = 0;
int capacity = 1;

void arrayPrint()
{
    for (int i = 0; i < current; i++)
    {
        cout << "Value at index[" << i << "] value : ";
        cout << Array[i] << " " << endl;
    }
    cout << endl;
}

void ArrayPop()
{
    current--;
}

int ArraySize()
{
    return current;
}

int ArrayCapacity()
{
    return capacity;
}

void add(int value)
{
    if (current == capacity)
    {
        capacity = 2 * capacity;
        int *temp = new int[capacity];

        // copy old item into temp;
        for (int i = 0; i < current; i++)
        {
            temp[i] = Array[i];
        }

        // delete old and assign new
        delete[] Array;
        Array = temp;
    }
    else
    {
        if (current == 0)
        {
            Array = new int[1];
        }
    }

    Array[current] = value;
    current++;
}

void add(int value, int index)
{
    if (index == capacity)
        add(value);
    else if (index > capacity)
    {
        cout << "invalid index" << endl;
        return;
    }
    else
    {
        Array[index] = value;
    }
}
void print()
{
    cout << "array_vec elements: ";
    for (int i = 0; i < current; i++)
    {
        cout << Array[i] << " ";
    }
    cout << endl;
}
int main()
{
    add(10);
    add(20);
    add(30);
    add(40);
    add(50);

    print();
    add(60, 2);
    print();
    add(60, 10);
    add(60, 2);
    print();
    cout << "ArraySize: " << ArraySize() << endl;
    cout << "ArrayCapacity: " << ArrayCapacity() << endl;
    ArrayPop();
    print();

    return 0;
}