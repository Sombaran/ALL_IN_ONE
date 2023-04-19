

// Call by value in C++
//     In call by value, original value is not modified.
//     In call by value, value being passed to the function is locally stored by the function parameter in stack memory location.
//     If you change the value of function parameter, it is changed for the current function only.
//     It will not change the value of variable inside the caller method such as main().
//     This is because both actual and formal parameters point to different locations in memory (i.e. they both have different memory addresses).


// Call by value	
//     A copy of value is passed to the function	
//     Changes made inside the function is not reflected on other functions	
//     Actual and formal arguments will be created in different memory location	

// Call by reference
//     An address of value is passed to the function
//     Changes made inside the function is reflected outside the function also
//     Actual and formal arguments will be created in same memory location

#include <iostream>
using namespace std;
void change(int data)
{
    data = 5;
}

int main()
{
    int data = 3;
    change(data);
    cout << "Value of the data is: " << data << endl; // Value of the data is: 3
    return 0;
}
