#include <iostream>
using namespace std;
class student
{
private:
    int id;
    int marks;

public:
    void adddata()
    {
        int i, mks;
        cout << "enter student marks and id:";
        cin >> i;
        cin >> mks;
        id = i;
        marks = mks;
        print();
    }
    void print()
    {
        cout << "student id num:" << id << endl;
        cout << "student marks:" << marks << endl;
    }
};

int main()
{
    student s;

    s.adddata();
    s.adddata();

    return 0;
}