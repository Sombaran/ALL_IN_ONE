/*
 * queue.cpp
 *
 *  Created on: 16-Mar-2022
 *      Author: Jotirling Swami
 */

//
//	 1. Container adapter with the Functionality of a Queue.
//	 2. Need to include <queue> header f‌ile.
//	 3. By default underling container will deque. The class template acts as a wrapper to the underlying deque.
//	 4. deque container - only a specific set of functions is provided.
//	 5. Pushes elements at one end (Front) and pops From other end (back) of underlying container.
//
//	Syntax
//	template<class T, class Container = deque<T> > class queue;
//
//
//	1. This data structure works on the FIFO technique
//	2. Where FIFO stands for First In First Out.
//	3. The element which was first inserted will be extracted at the first and so on.
//	4. There is an element called as 'front' which is the element at the front most position or say the first position,
//	5. The element called as 'rear' which is the element at the last position.
//	6. In normal queues insertion of elements take at the rear end and the deletion is done from the front.
//
//
//	Template Parameters
//		T: The argument specifies the type of the element which the container adaptor will be holding.
//		Container: The argument specifies an internal object of container where the elements of the queues are held.
//
//	Functions
//		With the help of functions, an object or variable can be played with in the field of programming.
//		Queues provide a large number of functions that can be used or embedded in the programs.
//
//		A list of the same is given below:
//			(constructor)	The function is used for the construction of a queue container.
//			empty		The function is used to test for the emptiness of a queue. If the queue is empty the function returns true else false.
//			size		The function returns the size of the queue container, which is a measure of the number of elements stored in the queue.
//			front		The function is used to access the front element of the queue.
//						The element plays a very important role as all the deletion operations are performed at the front element.
//			back		The function is used to access the rear element of the queue.
//						The element plays a very important role as all the insertion operations are performed at the rear element.
//			push		The function is used for the insertion of a new element at the rear end of the queue.
//			pop			The function is used for the deletion of element; the element in the queue is deleted from the front end.
//			emplace		The function is used for insertion of new elements in the queue above the current rear element.
//			swap		The function is used for interchanging the contents of two containers in reference.

#include <iostream>
#include <queue>
#include <iterator>
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
	//-----------------------------------------------------------------------
	std::queue<int> Queue1;
	Queue1.push(5);

	std::queue<int> Queue2(Queue1);
	std::cout << "Queue2.size(): " << Queue2.size() << endl;

	std::deque<int> dQueue { 3, 1, 4, 1, 5 };
	std::queue<int> Queue3(dQueue);
	std::cout << "Queue3.size(): " << Queue3.size() << endl;
	//-----------------------------------------------------------------------
	Queue1.push(24); // queue Push operation : return the first element in the queue.
	Queue1.push(80); // queue Push operation : return the first element in the queue.

	std::cout << "Queue1.size(): " << Queue1.size() << endl;

	while (!Queue1.empty()) { // check queue empty() operation
		cout << Queue1.front() << " "; // queue front operation : return the first element in the queue
		Queue1.pop(); // queue pop operation : delete the last element from the queue
	}
	cout << endl;
	//-----------------------------------------------------------------------
	queue<char> Queueswap1; // A B C D E
	Queueswap1.push('A');
	Queueswap1.push('B');
	Queueswap1.push('C');
	Queueswap1.push('D');
	Queueswap1.push('E');

	queue<char> Queueswap2; // P Q R S
	Queueswap2.push('P');
	Queueswap2.push('Q');
	Queueswap2.push('R');
	Queueswap2.push('S');

	Queueswap1.swap(Queueswap2); // swap function interchange the content to another
	cout << "After Swap Queueswap1: ";
	while (!Queueswap1.empty()) { // check queue empty() operation
		cout << Queueswap1.front() << " "; // queue front operation : return the first element in the queue
		Queueswap1.pop(); // queue pop operation : delete the last element from the queue
	}
	cout << endl;
	cout << "After Swap Queueswap2: ";
	while (!Queueswap2.empty()) { // check queue empty() operation
		cout << Queueswap2.front() << " "; // queue front operation : return the first element in the queue
		Queueswap2.pop(); // queue pop operation : delete the last element from the queue
	}
	cout << endl;
	//-----------------------------------------------------------------------

	std::queue<Base> QueueBase;
	Base b1(50);
	Base b2(70);
	Base b3(20);

	QueueBase.push(b1);
	QueueBase.push(b2);
	QueueBase.push(b3);

	QueueBase.emplace(40);

	std::queue<Base> QueueBase1(QueueBase);
	cout << "QueueBase1: ";
	while (!QueueBase1.empty()) { // check queue empty() operation
		QueueBase1.front().display(); // queue front operation : return the first element in the queue
		QueueBase1.pop(); // queue pop operation : delete the last element from the queue
	}
	cout << endl;

	std::deque<Base> dQueueBase { b1, b2, b3 };
	std::queue<Base> QueueBase2(dQueueBase);
	cout << "QueueBase2: ";
	while (!QueueBase2.empty()) { // check queue empty() operation
		QueueBase2.front().display(); // queue front operation : return the first element in the queue
		QueueBase2.pop(); // queue pop operation : delete the last element from the queue
	}
	cout << endl;

	//-----------------------------------------------------------------------

	return 0;
}

