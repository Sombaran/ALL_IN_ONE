

// POINTS:

// 1. Some time we have to specialize template for a particular data type.
// 2. We can specialize both "function" and "class" template.

// REQUIREMENT:
// Add all the elements in vector

#include <iostream>
#include <vector>
#include <string>

using namespace std;

template <typename T>
T addItems(vector<T> &vec)
{
    cout << "addItems start" << endl;
    T temp = 0;
    for (auto &it : vec)
    {
        temp += it;
    }
    cout << "addItems end" << endl;
    return temp;
}

template <>
string addItems(vector<string> &vec)
{

    cout << "addItems start" << endl;
    int count = 0;
    for (auto &it : vec)
    {
        for (auto &it1 : it)
        {
            count += it1;
        }
    }

    std::string strCount = std::to_string(count);

    cout << "addItems end" << endl;

    return strCount;
}

int main()
{
    vector<int> IntVec{1, 3, 5, 7, 9, 2, 4, 6, 8};
    vector<double> DoubleVec{1.1, 3.3, 5.5, 7.7, 9.9, 2.2, 4.4, 6.6, 8.8};
    vector<string> VecString = {"abc", "bcd", "cde"};
    cout << "main start" << endl;
    // cout << addItems(IntVec) << endl;
    // cout << addItems(DoubleVec) << endl;
    cout << addItems(VecString) << endl;
    cout << "main end" << endl;
    return 0;
}