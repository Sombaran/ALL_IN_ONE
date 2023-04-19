//  Encapsulation:

//     Encapsulation is defined as wrapping up of data and information under a single unit.
//     Encapsulation is defined as binding together the data and the functions that manipulate them.

//     In a company, there are different sections like the accounts section, finance section, sales section etc.
//     The finance section handles all the financial transactions and keeps records.
//     Similarly, the sales section handles all the sales-related activities and keeps records of all the sales.
//     For some reason from the finance section needs all the data about sales in a particular month.
//     In this case, he is not allowed to directly access the data of the sales section.
//     He will first have to contact sales section and then request him to give the particular data.
//     This is what encapsulation is.

// c++ program to explain
// Encapsulation

#include <iostream>
using namespace std;

class Encapsulation
{
private:
    // data hidden from outside world
    int x;

public:
    // function to set value of
    // variable x
    void set(int a)
    {
        x = a;
    }

    // function to return value of
    // variable x
    int get()
    {
        return x;
    }
};

// main function
int main()
{
    Encapsulation obj;

    obj.set(5);

    cout << obj.get();
    return 0;
}

// Encapsulation also lead to data abstraction or hiding.
// As using encapsulation also hides the data.
// In the above example the data of any of the section like sales, finance or accounts is hidden from any other section.