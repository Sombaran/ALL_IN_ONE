/*
 * stack.cpp
 *
 *  Created on: 16-Mar-2022
 *      Author: Jotirling Swami
 */
//   *** stack ***
//		1. Stacks are a type of container adaptors with LIFO(Last In First Out) type of working,
//		2. where a new element is added at one end (top) and an element is removed from that end only.
//		3. Stack uses an encapsulated object of either vector or deque (by default) or list (sequential container class) as its underlying container,
//		4. providing a specific set of member functions to access its elements.
//		5. For creating a stack, we must include the <stack> header file in our code. We then use this syntax to define the stack:
//		Stack Syntax:-
//
//
//		Syntax :
//			template <class Type, class Container = deque<Type> > class stack;
//		1. This data structure works on the LIFO technique, where LIFO stands for Last In First Out.
//		2. The element which was first inserted will be extracted at the end and so on.
//		3  There is an element called as 'top' which is the element at the upper most position.
//		4. All the insertion and deletion operations are made at the top element itself in the stack.
//
//		Type ' is the Type of element contained in the stack. It can be any valid C++ type or even a user-defined type.
//		Container ' is the Type of underlying container object.
//
//		Member Types:-
//			value_type- The first template parameter, T. It denotes the element types.
//			container_type- The second template parameter, Container. It denotes the underlying container type.
//			size_type- Unsigned integral type.
//
//		The functions associated with stack are:
//			empty() ' Returns whether the stack is empty ' Time Complexity : O(1)
//			size() ' Returns the size of the stack ' Time Complexity : O(1)
//			top() ' Returns a reference to the top most element of the stack ' Time Complexity : O(1)
//			push(g) ' Adds the element 'g' at the top of the stack ' Time Complexity : O(1)
//			pop() ' Deletes the top most element of the stack ' Time Complexity : O(1)
//
//		Functions :
//			1. (constructor) The function is used for the construction of a stack container.
//			2. empty 		 The function is used to test for the emptiness of a stack. If the stack is empty the function returns true else false.
//			3. size			 The function returns the size of the stack container, which is a measure of the number of elements stored in the stack.
//			4. top 			 The function is used to access the top element of the stack.
//						 		The element plays a very important role as all the insertion and deletion operations are performed at the top element.
//			5. push 		 The function is used for the insertion of a new element at the top of the stack.
//			6. pop 			 The function is used for the deletion of element, the element in the stack is deleted from the top.
//			7. emplace 		 The function is used for insertion of new elements in the stack above the current top element.
//			8. swap 		 The function is used for interchanging the contents of two containers in reference.
//
//
#include <iostream>
#include <stack>
using namespace std;

class Base {
	int value;
public:
	Base(int val) :
			value { val } {
	}

	void display() {
		cout << value << " ";
	}
};

int main() {

	stack<int> stack1;
	stack1.push(5);
	cout << "stack1.size() : " << stack1.size() << '\n';

	stack<int> stack2(stack1);
	cout << "stack2.size() : " << stack2.size() << '\n';

	deque<int> deq { 3, 1, 4, 1, 5 };
	stack<int> stack3(deq); // overload (2)
	cout << "stack3.size() : " << stack3.size() << '\n';

	while (!stack3.empty()) {
		cout << stack3.top() << " ";
		stack3.pop();
	}

	cout << endl;
	//-----------------------------------------------------------------------
	stack<char> Stackswap1; // A B C D E
	Stackswap1.push('A');
	Stackswap1.push('B');
	Stackswap1.push('C');
	Stackswap1.push('D');
	Stackswap1.push('E');

	stack<char> Stackswap2; // P Q R S
	Stackswap2.push('P');
	Stackswap2.push('Q');
	Stackswap2.push('R');
	Stackswap2.push('S');

	Stackswap1.swap(Stackswap2); // swap function interchange the content to another
	cout << "After Swap Stackswap1: ";
	while (!Stackswap1.empty()) { // check stack empty() operation
		cout << Stackswap1.top() << " "; // stack front operation : return the first element in the stack
		Stackswap1.pop(); // stack pop operation : delete the last element from the stack
	}
	cout << endl;
	cout << "After Swap Stackswap2: ";
	while (!Stackswap2.empty()) { // check stack empty() operation
		cout << Stackswap2.top() << " "; // stack front operation : return the first element in the stack
		Stackswap2.pop(); // stack pop operation : delete the last element from the stack
	}
	cout << endl;
	//-----------------------------------------------------------------------

	stack<int> Stack;
	Stack.push(21); // push function it will push the element in stack and on top of previous element it will add another element
	Stack.push(22);
	Stack.push(24);
	Stack.push(25);

	Stack.pop(); // pop function removes the top element
	Stack.pop();

	while (!Stack.empty()) {
		cout << Stack.top() << " ";
		Stack.pop();
	}

	//-----------------------------------------------------------------------

	Base b1(50);
	Base b2(70);
	Base b3(20);
	deque<Base> dQueueBase { b1, b2, b3 };
	std::stack<Base> stack_Base(dQueueBase);

	stack_Base.emplace(40); // emplace operation will construct the object using the parameter.
	stack_Base.emplace(100); // emplace operation will construct the object using the parameter.
	cout << "stack_Base :";
	while (!stack_Base.empty()) {
		stack_Base.top().display();
		stack_Base.pop();
	}
}

