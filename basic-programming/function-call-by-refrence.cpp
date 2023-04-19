

// What is Call By Reference / Pass by reference?
//      In the Call by reference / Pass by reference method, the function parameters are passed as a reference.
//      Inside the function definition, the actual parameters are accessed using the reference variable.


#include <iostream>
using namespace std;

void mySwap(int &vref_1, int &vref_2)
{
    int temp_var;
    temp_var = vref_1;
    vref_1 = vref_2;
    vref_2 = temp_var;
}

int main()
{
    int var_1 = 100;
    int var_2 = 300;

    cout << "Before calling mySwap() function, value of var_1 : " << var_1 << endl;
    cout << "Before calling mySwap() function, value of var_2 : " << var_2 << endl;

    cout << "Calling mySwap() function - Call by reference." << endl;
    mySwap(var_1, var_2);

    cout << "After calling mySwap() function, value of var_1 : " << var_1 << endl;
    cout << "After calling mySwap() function, value of var_2 : " << var_2 << endl;

    return 0;
}