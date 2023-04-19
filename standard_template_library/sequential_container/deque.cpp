/*
 * deque.cpp
 *
 *  Created on: 16-Mar-2022
 *      Author: Jotirling Swami
 */

//		***  Deque  ***
//		Deque stands for double ended queue. It generalizes the queue data structure
//		i.e insertion and deletion can be performed from both the ends either front or back.

		1. Double-ended queues are sequence containers with the feature of expansion and contraction on both ends.
		2. They are similar to vectors, but are more efficient in case of insertion and deletion of elements.
			Unlike vectors, contiguous storage allocation may not be guaranteed.

		3. Double Ended Queues are basically an implementation of the data structure double-ended queue.
		4. A queue data structure allows insertion only at the end and deletion from the front.
		5. This is like a queue in real life, wherein people are removed from the front and added at the back.
		6. Double-ended queues are a special case of queues where insertion and deletion operations are possible at both the ends.

		The functions for deque are same as vector, with an addition of push and pop operations for both front and back.

#include <iostream>
#include<deque>
using namespace std;

void printElement(deque<int> &Deque) {
	deque<int>::iterator it = Deque.begin();
	for (it = Deque.begin(); it != Deque.end(); ++it)
		std::cout << *it << " ";
	cout << endl;
}

void printElement(deque<string> &Deque) {
	deque<string>::iterator it = Deque.begin();
	for (it = Deque.begin(); it != Deque.end(); ++it)
		std::cout << *it << " ";
	cout << endl;
}

int main() {
//		Method	Description
//		assign()		It assigns new content and replacing the old one.

//		void assign(InputIterator first, InputIterator last);
//		void assign(int n,value_type val);
	{
		deque<int> first = { 1, 2, 3, 4 };
		deque<int> second;

		printElement(first);
		second.assign(first.begin(), first.end());
		printElement(second);
	}

//		emplace()		It adds a new element at a specified position.

	{
		deque<int> Deque { 1, 2, 3, 4, 5 };
		deque<int>::iterator it = Deque.begin();
		Deque.emplace(it + 3, 300);
		printElement(Deque);
	}

//		emplace_back()	It adds a new element at the end.

	{
		deque<int> Deque { 1, 2, 3, 4, 5 };
		Deque.emplace_back(300);
		printElement(Deque);
	}

//		emplace_front()	It adds a new element in the beginning of a deque.
	{
		deque<int> Deque { 1, 2, 3, 4, 5 };
		Deque.emplace_front(300);
		printElement(Deque);
	}
//		insert()		It adds a new element just before the specified position.

	{
		deque<string> Deque { "Joti", "Prajot", "Priya" };
		deque<string>::iterator it = Deque.begin();
		Deque.insert(it + 2, "swami");
		printElement(Deque);
	}
//		push_back()		It adds a new element at the end of the container.

	{
		deque<string> Deque { "Joti", "Prajot", "Priya" };
		deque<string>::iterator it = Deque.begin();
		Deque.push_back("swami");
		printElement(Deque);
	}

//		push_front()	It adds a new element at the beginning of the container.
	{
		deque<string> Deque { "Joti", "Prajot", "Priya" };
		deque<string>::iterator it = Deque.begin();
		Deque.push_front("swami");
		printElement(Deque);
	}
//		pop_back()		It deletes the last element from the deque.
	{
		deque<string> Deque { "Joti", "Prajot", "Priya" };
		deque<string>::iterator it = Deque.begin();
		Deque.pop_back();
		printElement(Deque);
	}
//		pop_front()		It deletes the first element from the deque.
	{
		deque<string> Deque { "Joti", "Prajot", "Priya" };
		deque<string>::iterator it = Deque.begin();
		Deque.pop_front();
		printElement(Deque);
	}
//		swap()			It exchanges the contents of two deques.
	{
		deque<string> Deque { "Joti", "Prajot", "Prajot" };
		deque<string> Deque1 { "swami", "Priya" };
		cout << "before Swap ... " << endl;
		printElement(Deque);
		printElement(Deque1);
		Deque1.swap(Deque);
		cout << "after Swap ... " << endl;
		printElement(Deque);
		printElement(Deque1);
	}
//		clear()			It removes all the contents of the deque.
	{
		deque<string> Deque { "Joti", "Prajot", "Priya" };
		deque<string>::iterator it = Deque.begin();
		Deque.clear();
		cout << "size after clear :" << Deque.size() << endl;
	}
//		empty()			It checks whether the container is empty or not.
	{
		deque<string> Deque { "Joti", "Prajot", "Priya" };
		deque<string>::iterator it = Deque.begin();
		cout << "is empty check:" << Deque.empty() << endl;
		Deque.clear();
		cout << "is empty check:" << Deque.empty() << endl;
	}
//		erase()			It removes the elements.
	{
		deque<int> Deque { 1, 2, 3, 4, 5 };
		deque<int>::iterator it = Deque.begin();
		printElement(Deque);
		Deque.erase(it + 1);
		printElement(Deque);
		Deque.erase(Deque.begin(), Deque.end());
		cout << "size after erase :" << Deque.size() << endl;
	}
//		max_size()		It determines the maximum size of the deque.
	{
		deque<int> Deque;
		cout << "max_size: " << Deque.max_size() << endl;
	}
//		resize()		It changes the size of the deque.
	{
		deque<int> Deque { 1, 2, 3, 4, 5 };
		cout << "max_size: " << Deque.max_size() << endl;
		cout << "size: " << Deque.size() << endl;
		printElement(Deque);
		Deque.resize(3);
		cout << "size: " << Deque.size() << endl;
		printElement(Deque);
	}
//		shrink_to_fit()	It reduces the memory to fit the size of the deque.
	{
		deque<int> Deque = { 1, 2, 3, 4, 5 };
		cout << "size: " << Deque.size() << endl;
		Deque.resize(10);
		cout << "resize: " << Deque.size() << endl;
		Deque.shrink_to_fit();
		cout << "shrink_to_fit: " << Deque.size() << endl;

	}
//		size()			It returns the number of elements.
	{
		deque<int> Deque = { 1, 2, 3, 4, 5 };
		cout << "Deque.size(): " << Deque.size() << endl;
	}
//		at()			It access the element at position pos.
	{
		deque<int> Deque = { 1, 2, 3, 4, 5 };
		cout << "Deque.at(1): " << Deque.at(1) << endl;
	}
//		operator[]()	It access the element at position pos.
	{
		deque<int> Deque = { 1, 2, 3, 4, 5 };
		cout << "Deque.operator[](1): " << Deque.operator[](1) << endl;
		deque<int> a = { 1, 2, 3, 4, 5, 6 };
		cout << "a.operator[](7) :" << a.operator[](7) << endl;
	}
//		operator=()		It assigns new contents to the container.
	{
		deque<int> Deque = { 1, 2, 3, 4, 5 };
		cout << "Deque :";
		printElement(Deque);
		deque<int> Deque1 = Deque;
		cout << "Deque1 :";
		printElement(Deque1);
	}
//		back()			It access the last element.
	{
		deque<int> Deque = { 1, 2, 3, 4, 5 };
		cout << "Deque.back(): " << Deque.back() << endl;
	}
//		begin()			It returns an iterator to the beginning of the deque.
	{
		deque<int> Deque = { 1, 2, 3, 4, 5 };
		cout << "Deque.begin(): " << *(Deque.begin() + 4) << endl;
	}
//		cbegin()		It returns a constant iterator to the beginning of the deque.
	{
		deque<int> Deque = { 1, 2, 3, 4, 5 };
		cout << "Deque.cbegin(): " << *(Deque.cbegin() + 4) << endl;
	}
//		end()			It returns an iterator to the end.
	{
		deque<int> Deque = { 1, 2, 3, 4, 5 };
		cout << "Deque.end(): " << *(Deque.end()) << endl;
		cout << "Deque.end()-1: " << *(Deque.end() - 1) << endl;
	}
//		cend()			It returns a constant iterator to the end.
	{
		deque<int> Deque = { 1, 2, 3, 4, 5 };
		cout << "Deque.cend(): " << *(Deque.cend()) << endl;
		cout << "Deque.cend()-1: " << *(Deque.cend() - 1) << endl;
	}
//		rbegin()		It returns a reverse iterator to the beginning.
	{
		deque<int> Deque = { 1, 2, 3, 4, 5 };
		cout << "Deque.rbegin(): " << *(Deque.rbegin()) << endl;
		cout << "Deque.rbegin()-1: " << *(Deque.rbegin() + 1) << endl;
	}
//		crbegin()		It returns a constant reverse iterator to the beginning.
	{
		deque<int> Deque = { 1, 2, 3, 4, 5 };
		cout << "Deque.crbegin(): " << *(Deque.crbegin()) << endl;
		cout << "Deque.crbegin()-1: " << *(Deque.crbegin() + 1) << endl;
	}
//		rend()			It returns a reverse iterator to the end.
	{
		deque<int> Deque = { 1, 2, 3, 4, 5 };
		cout << "Deque.rend()-1: " << *(Deque.rend()- 1) << endl;
	}
//		crend()			It returns a constant reverse iterator to the end.
	{
		deque<int> Deque = { 1, 2, 3, 4, 5 };
		cout << "Deque.crend(): " << *(Deque.crend()) << endl;
		cout << "Deque.crend()-1: " << *(Deque.crend() + 1) << endl;
	}
//		front()			It access the last element.
	{
		deque<int> Deque = { 1, 2, 3, 4, 5 };
		cout << "Deque.front(): " << Deque.front() << endl;
	}
	return 0;
}
