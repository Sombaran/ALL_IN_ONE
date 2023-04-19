/*


//	1. Pointers to constant:	const int* score =  &high;
//			Value of the pointer can not change but the address where it is pointing can change.
//
//		a. Data pointed by the pointer is constant and cannot be changed.
//		b. The pointer itself can change and points somewhere else (as the pointer itself is a variable).
//
//	2. Constant pointers:	 int* const ptr = &a;
//			Address where it is pointing can not be change but Value of the pointer can not change.
//
//		a. The pointer points to a fixed memory location, and the value can be changed because it is a variable,
//		b. The pointer will always point to the same location because it is made constant here.
//
//	3. Constant Pointers to constants:	const int* const ptr = &a;
//		Address where it is pointing can not be change and Value of the pointer can not change.
//
//			a. The data pointed to by the pointer is constant and cannot be changed.
//			b. The pointer itself is constant and cannot change and point somewhere else.

#include <iostream>
using namespace std;

int main() {

	int val = 10;
	int val1 = 20;
	int &value = val;
	value = 100;
	cout << value << endl;


	 const int ivalue = val;
	 ivalue = val1; 	//error: assignment of read-only variable 'ivalue'
	 ivalue = 10; 	//error: assignment of read-only variable 'ivalue'

	 int const ivalue1 = val;
	 ivalue1 = val1; 	//error: assignment of read-only variable 'ivalue1'
	 ivalue1 = 10; 		//error: assignment of read-only variable 'ivalue1'

	 const int *ptr1 = &val;
	 ptr1 = &val1;
	 *ptr1 = 100; 		//error: assignment of read-only location '* ptr1'

	 int *const ptr2 = &val;
	 ptr2 = &val1;		//error: assignment of read-only variable 'ptr2'
	 *ptr2 = 100;

	 const int *const ptr3 = &val;
	 ptr3 = &val1;		//error: assignment of read-only variable 'ptr3'
	 *ptr3 = 100;   		//error: assignment of read-only location '*(const int*)ptr3'


	return 0;
}

*/
