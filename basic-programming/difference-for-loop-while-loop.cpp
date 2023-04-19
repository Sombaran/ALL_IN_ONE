/*
 * difference-for-loop-while-loop.cpp
 *
 *  Created on: 19-Mar-2022
 *      Author: Jotirling Swami
 */

//   ***  For loop  ***
//		1. The initialization, condition checking, and the iteration statements are written at the beginning of the loop.
//		2. It is used only when the number of iterations is known beforehand.
//		3. If the condition is not mentioned in the 'for' loop, then the loop iterates infinite number of times.
//		4. The initialization is done only once, and it is never repeated.
//		5. The iteration statement is written at the beginning.
//			Hence, it executes once all statements in loop have been executed.
//
//			Syntex:
//			for(initialization; condition; iteration) {
//				//body of the 'for' loop
//			}
//   ***  While condition  ***
//		1. The initialization and the condition checking is done at the beginning of the loop.
//		2. It is used only when the number of iterations isn't known.
//		3. If the condition is not mentioned in the 'while' loop, it results in a compilation error.
//		4. If the initialization is done when the condition is being checked, then
//				initialization occurs every time the loop is iterated through.
//		5. The iteration statement can be written within any point inside the loop.
//
//		6. The controlling condition here appears at the beginning of the loop.
//		7. The iterations do not occur if the condition at the first iteration results in False.
//		8. It is also known as an entry-controlled loop
//		9. here is no condition at the end of the loop.
//		10. It doesn't need to execute at least one.
//
//		Syntex:
//		while ( condition) {
//		   statements;
//		   //body of the loop
//		}
//   ***  do-while condition  ***
//		1. he controlling condition is present at the end of the loop.
//		2. The condition is executed at least once even if the condition computes to false during the first iteration.
//		3. It is also known as an exit-controlled loop
//		4. There is a condition at the end of the loop.
//
//		Syntex:
//		do {
//		   statements;
//		   // body of loop.
//		}
//		while( Condition );
//
//
//  1. for-loop
//
#include <iostream>
#ifndef NULL
int main()
{

	/* for loop execution */
	for (int a = 10; a < 20; a++)
	{
		printf("value of a: %d\n", a);
	}
	return 0;
}
#endif
//  2. while-loop
//
#include <iostream>
#ifndef NULL
int main()
{
	/* local variable definition */
	int a = 10;
	/* while loop execution */
	while (a < 20)
	{
		printf("value of a: %d\n", a);
		a++;
	}
	return 0;
}
#endif
//	3. Example do-whil-loop:
//
#include <iostream>
#ifndef NULL
int main()
{
	int a = 10; // Local variable declaration:
	do
	{ // do loop execution
		printf("value of a: %d\n", a);
		a = a + 1;
	} while (a < 20);
	return 0;
}
#endif

/*
	Main Differences Between For loop and While loop

	For loop, the number of iterations to be conducted is already known whereas 
	While loop the number of iterations are not known.

	For loop contains only a single condition whereas 
	while loop may contain a set of commands to be executed together.

	In for loop, initialization of command is done only once but in while loop initialization of command is needed each time the iteration of command is done.

	If the condition is absent in for loop, the loop iterates for an infinite number of times whereas 
	while loop shows an error in case of the absence of the condition.

	For loop can be used only in case of a known number of iterations whereas 
	while loop is used only when the number of iterations is not known.

*/