#include <iostream>
using namespace std;

void mySwap(int **vptr_1, int **vptr_2)
{
    int temp_var;
    temp_var = **vptr_1;
    **vptr_1 = **vptr_2;
    **vptr_2 = temp_var;
}

int main()
{
    int *var_1 = new int(100);
    int *var_2 = new int(300);

    cout << "Before calling mySwap(), value of var_1 : " << *var_1
         << " value of var_2 : " << *var_2 << endl;

    cout << "Calling mySwap() function - Call by address." << endl;
    mySwap(&var_1, &var_2);

    cout << "Before calling mySwap(), value of var_1 : " << *var_1
         << " value of var_2 : " << *var_2 << endl;

    return 0;
}
