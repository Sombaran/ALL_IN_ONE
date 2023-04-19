/*
 * interview-questing-1.cpp
 *
 *  Created on: 18-Mar-2022
 *      Author: Jotirling Swami
 */

//  1. Find out correct bracket closing loop from given example.
#include <iostream>
#include <stack>

using namespace std;

#ifndef NULL
int main()
{
	string str = "[ [ [ {} {} ]]] {}";
	stack<char> Stack;
	for (size_t i = 0; i < str.length(); i++)
	{
		if ('{' == str[i])
			Stack.push(str[i]);
		else if ('}' == str[i])
		{
			Stack.pop();
		}
		else if ('[' == str[i])
		{
			Stack.push(str[i]);
		}
		else if (']' == str[i])
		{
			Stack.pop();
		}
	}

	if (Stack.empty())
	{
		cout << "Valid" << endl;
	}
	else
	{
		cout << "Not Valid" << endl;
	}
}
#endif

//  2. reverse the int value.
//		Example Input Value = 123 Then Output Value = 321.

#include <iostream>
using namespace std;

// #ifndef NULL
int main()
{
	int number, reverse_number = 0;
	cout << "Enter a number to reverse value:"; // allow user to enter a number
	cin >> number;								// takes value from user
	while (number != 0)
	{
		reverse_number = reverse_number * 10 + number % 10;
		number = number / 10;
	}
	cout << "Reverse of entered number is:";
	cout << reverse_number; // print reverse value
	return 0;
}
// #endif

//  3. reverse string from array
#include <iostream>
#include <cstring>

using namespace std;

#ifndef NULL
int main()
{
	char charVar = '&';
	cout << (int)charVar << endl;

	char strArray[] = "Jotirling Swami Priya";
	int len = strlen(S);
	for (int i = 0; i < len / 2; ++i)
	{
		char temp = strArray[len - i - 1];
		strArray[len - i - 1] = strArray[i];
		strArray[i] = temp;
	}
	cout << S << endl;
}
#endif

//  3. seperate word from char array

#include <iostream>
#include <cstring>

using namespace std;

#ifndef NULL
int main()
{
	char strArray[] = "Jotirling Swami Priya";
	int i = 0;
	string chunk;
	char *array[4];
	while (strArray[i] != '\0')
	{
		if (strArray[i] == ' ')
		{
			cout << chunk << " ";
			chunk = "";
		}

		chunk.push_back(strArray[i]);
		i++;
	}
	cout << chunk << " ";
}
#endif
