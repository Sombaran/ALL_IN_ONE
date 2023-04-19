

// What is Call By Address / Pass by address?
//     In the case of the Call by address / Pass by address method, the function arguments are passed as address.
//     The caller function passes the address of the parameters.
//     Pointer variables are used in the function definition.
//     With the help of the Call by address method, the function can access the actual parameters and modify them.

#include <iostream>
using namespace std;

void mySwap(int *vptr_1, int *vptr_2)
{
    int temp_var;
    temp_var = *vptr_1;
    *vptr_1 = *vptr_2;
    *vptr_2 = temp_var;
}

int main()
{
    int var_1 = 100;
    int var_2 = 300;

    cout << "Before calling mySwap(), value of var_1 : " << var_1 << " value of var_2 : " << var_2 << endl;

    cout << "Calling mySwap() function - Call by address." << endl;
    mySwap(&var_1, &var_2);

    cout << "Before calling mySwap(), value of var_1 : " << var_1 << " value of var_2 : " << var_2 << endl;

    return 0;
}