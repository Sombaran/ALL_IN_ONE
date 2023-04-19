//  Vector implementation in C++
//         The given task is to implement a class in C++ which behaves just like the Vector class.
//         Vectors are the same as dynamic arrays with the ability to resize themselves automatically
//             when an element is inserted or deleted,
//         The storage being handled automatically by the container.
//         Vector elements are placed in contiguous storage so that they can be accessed and traversed using iterators.
//             - In vectors, data is inserted at the end.
//             - Inserting at the end takes differential time, as sometimes there may be a need of extending the array.
//             - Removing the last element takes only constant time because no resizing happens.
//             - Inserting and erasing at the beginning or in the middle is linear in time.

//         We can also make the vector class generic using templates.
//         Certain functions associated with the Vector that we will implement are:

//             void push(int data):
//                 This function takes one element and inserts it at the last. Amortized time complexity is O(1).
//             void push(int data, int index):
//                 It inserts data at the specified index. Time complexity is O(1).
//             int get(int index):
//                 It is used to get the element at the specified index. Time complexity is O(1).
//             void pop():
//                 It deletes the last element. Time complexity is O(1).
//             int size():
//                 It returns the size of the vector i.e, the number of elements in the vector. Time complexity is O(1).
//             int getcapacity():
//                 It returns the capacity of the vector. Time complexity is O(1).
//             void print():
//                 It is used to print array elements. Time complexity is O(N), where N is the size of the vector.

#include <iostream>
using namespace std;

class Vector
{
    int *array_vec; // pointer which holds the elements
    int current_element;
    int capacity;

public:
    Vector()
    {
        array_vec = new int[1];
        current_element = 0;
        capacity = 1;
    }

    void push(int value)
    {
        if (current_element == capacity)
        {
            capacity = 2 * capacity;
            
            // allocate new capacity array to temp
            int *temp = new int[capacity];

            // copy all element from array_vec into the new temp.
            for (int i = 0; i < capacity / 2; i++)
            {
                cout << "copy: " << array_vec[i] << endl;
                temp[i] = array_vec[i];
            }

            // delete the old array_vec and assign new temp to the array_vec
            delete[] array_vec;
            array_vec = temp;
        }

        // insert the new value at the end of the array_vec
        array_vec[current_element] = value;
        cout << "added: " << array_vec[current_element] << endl;
        current_element++;
    }

    void push(int value, int index)
    {
        if (index == capacity)
            push(value);
        else if (index > capacity)
        {
            cout << "invalid index" << endl;
            return;
        }
        else
        {
            array_vec[index] = value;
        }
    }

    int get(int index)
    {
        return array_vec[index];
    }

    void pop()
    {
        current_element--;
    }

    int size()
    {
        return capacity;
    }

    int getcapacity()
    {
        return capacity;
    }

    void print()
    {
        cout << "array_vec elements: ";
        for (int i = 0; i < current_element; i++)
        {
            cout << array_vec[i] << " ";
        }
        cout << endl;
    }
};

int main()
{
    Vector vec;

    vec.push(10);
    vec.push(20);
    vec.push(30);
    vec.push(40);
    vec.push(50);
    vec.print();
    vec.push(60, 10);
    vec.push(60, 2);

    vec.push(10);
    vec.push(20);
    vec.push(30);
    vec.push(40);

    vec.push(100, 1);
    vec.push(100, 3);

    vec.push(300, 10);

    vec.print();

    vec.pop();
    vec.print();
}