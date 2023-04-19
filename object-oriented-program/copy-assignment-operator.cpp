#include <iostream>
#include <cstring>
using namespace std;

class String
{
private:
    char *s;

public:
    String() // default constructor
    {
        s = NULL;
    }

    String(const char *str) // parametrized constructor
    {
        int size = strlen(str);
        s = new char[size + 1];
        strcpy(s, str);
    }

    // String(const String &old_str) // copy constructor
    // {
    // 	cout << "copy constructor" << endl;

    // 	int size = strlen(old_str.s);
    // 	s = new char[size + 1];
    // 	strcpy(s, old_str.s);
    // }

    String &operator=(const String &obj) // assignment operator
    {
        int size = strlen(obj.s);
        s = new char[size + 1];
        strcpy(s, obj.s);
        return *this;
    }

    void print() // Function to print string
    {
        cout << s << endl;
    }

    void change(const char *str) // Function to change string
    {
        delete[] s;
        int size = strlen(str);
        s = new char[size + 1];
        strcpy(s, str);
    }

    ~String() // destructor
    {
        delete[] s;
    }
};

int main()
{
    String str1("joti");
    String str2("priya");
    cout << "before assignment" << endl;
    str1.print();
    str2.print();

    str2 = str1; // assignment operator
    cout << "after assignment" << endl;
    str1.print();
    str2.print();

    str2.change("prajot");
    cout << "after change second " << endl;

    str1.print();
    str2.print();

    return 0;
}
