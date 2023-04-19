/*
 * priority_queue.cpp
 *
 *  Created on: 10-Mar-2022
 *      Author: Jotirling Swami
 */

#include <iostream>
#include <queue>
using namespace std;

//	Sequence containers
//		Sequence containers implement data structures that can be accessed sequentially.
//
//			array: Static contiguous array (class template)
//			vector: Dynamic contiguous array (class template)
//			deque: Double-ended queue (class template)
//			forward_list: Singly-linked list (class template)
//			list: Doubly-linked list (class template)
//
//	Associative containers
//		Associative containers implement sorted data structures that can be quickly searched (O(log n) complexity).
//
//			Set: Collection of unique keys, sorted by keys (class template)
//			Map: Collection of key-value pairs, sorted by keys, keys are unique (class template).
//			multiset: Collection of keys, sorted by keys (class template)
//			multimap: Collection of key-value pairs, sorted by keys (class template)
//
//	Unordered associative containers
//		Unordered associative containers implement unsorted (hashed) data structures that can be quickly searched (O(1) amortized, O(n) worst-case complexity).
//
//			unordered_set: Collection of unique keys, hashed by keys. (class template)
//			unordered_map: Collection of key-value pairs, hashed by keys, keys are unique. (class template)
//			unordered_multiset: Collection of keys, hashed by keys (class template)
//			unordered_multimap: Collection of key-value pairs, hashed by keys (class template)
//
//	Container adaptors
//		Container adaptors provide a different interface for sequential containers.
//
//			stack: Adapts a container to provide stack (LIFO data structure) (class template).
//			queue: Adapts a container to provide queue (FIFO data structure) (class template).
//			priority_queue: Adapts a container to provide priority queue (class template).

//	Sequential container details:
//									vector
//									deque
//									list
//	Associative container details:
//									map
//									multimap
//									set
//									multiset
//	Container adaptor details:
//									stack
//									queue
//									priority_queue

//			*** Priority Queue ***
//
//
//
//	Container adaptor that provides constant time lookup of the largest (by default) element,
// 	at the expense of logarithmic insertion and extraction.
//  Need to include (queue) header file.
//	A user-provided Compare can be supplied to change the ordering
//
//
//	Requirement :
//	Container	-	The type of the underlying container to use to store the elements.
//  				The container must satisfy the requirements of SequenceContainer
//					Its iterators must satisfy the requirements of LegacyRandomAccessIterator.
// 					Additionally, it must provide the following functions with the usual semantics:
//							front()
//							push_back()
//							pop_back()
//	The standard containers std::vector and std::deque satisfy these requirements.
//
//	Compare	-	A Compare type providing a strict weak ordering.
//			Note that the Compare parameter is defined such that it returns true.
//			if its first argument comes before its second argument in a weak ordering

/*
 Queue Function :
 size()
 top()
 empty()
 push()
 pop()
 */

//-----------------------------------------------------------------------------------------------------------
//	(constructor): constructs the priority_queue
//
//		std::priority_queue<int> pq1;
//		pq1.push(5);
//
//		std::priority_queue<int> pq2 { pq1 };
//
//		std::vector<int> vec { 3, 1, 4, 1, 5 };
//		std::priority_queue<int> pq3 { std::less<int>(), vec };
//
//	(destructor) : destructs the priority_queue
//			Destructs the priority_queue. The destructors of the elements are called and the used storage is deallocated.
//			Note, that if the elements are pointers, the pointed-to objects are not destroyed.
//			Complexity : Linear in the size of the priority_queue.
//
//	operator= : assigns values to the container adaptor
//
//			priority_queue& operator=( const priority_queue& other );
//			priority_queue& operator=( priority_queue&& other );
//
//			Replaces the contents of the container adaptor with those of other.
//			1) Copy assignment operator. Replaces the contents with a copy of the contents of other.
//					Effectively calls c = other.c; comp = other.comp;. (implicitly declared)
//			2) Move assignment operator. Replaces the contents with those of other using move semantics.
//					Effectively calls c = std::move(other.c); comp = std::move(other.comp); (implicitly declared)
//
//	Element access :
//		top : accesses the top element
//
//	Capacity:
//		empty : checks whether the underlying container is empty
//		size : returns the number of elements
//
//	Modifiers:
//		push : inserts element and sorts the underlying container
//		emplace : constructs element in-place and sorts the underlying container
//		pop : removes the top element
//		swap : swaps the contents
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------
//	Function Description :
//		push()	It inserts a new element in a priority queue.
//			push() adds a copy of an already constructed object into the queue as a parameter,
//			it takes an object of the queue's element type.
//		pop()	It removes the top element from the queue, which has the highest priority.
//		top()	This function is used to address the top most element of a priority queue.
//		size()	It determines the size of a priority queue.
//		empty()	It verifies whether the queue is empty or not. Based on the verification, it returns the status.
//		swap()	It swaps the elements of a priority queue with another queue having the same type and size.
//		emplace()	It inserts a new element at the top of the priority queue.
//			emplace() constructs a new object in-place at the end of the queue.
//			It takes as parameters the parameters that the queue's element types constructor takes.
//			If your usage pattern is one where you create a new object and add it to the container,
//			you shortcut a few steps (creation of a temporary object and copying it) by using emplace().
//-----------------------------------------------------------------------------------------------------------

class Student {
public:
	int rollNumber;
	int marks;
	string name;
	string surname;

	Student(int r, int m, string n, string s) {
		rollNumber = r;
		marks = m;
		name = n;
		surname = s;
	}
	void display() {
		cout << "rollNumber:" << rollNumber << " Marks:" << marks << " Name:"
				<< name << " Surname:" << surname << endl;
	}
};

int main() {

//	priority_queue<int> pQueue;
//	vector<int> v = { 1, 4, 2, 6, 9, 0, 8, 3, 5 };
//	for (vector<int>::iterator it = v.begin(); it != v.end(); ++it)
//		pQueue.push(*it);
//
//	while (!pQueue.empty()) {
//		cout << pQueue.top() << " "; // 9 8 6 5 4 3 2 1 0
//		pQueue.pop();
//	}

//	priority_queue<int, vector<int>, std::less<int>> pQueue;
//	vector<int> v = { 1, 4, 2, 6, 9, 0, 8, 3, 5 };
//
//	for (vector<int>::iterator it = v.begin(); it != v.end(); ++it)
//		pQueue.push(*it);
//
//	while (!pQueue.empty()) {
//		cout << pQueue.top() << " "; // 9 8 6 5 4 3 2 1 0
//		pQueue.pop();
//	}

//	priority_queue<int, vector<int>, std::greater<int>> pQueue;
//	vector<int> v = { 1, 4, 2, 6, 9, 0, 8, 3, 5 };
//
//	for (vector<int>::iterator it = v.begin(); it != v.end(); ++it)
//		pQueue.push(*it);
//
//	while (!pQueue.empty()) {
//		cout << pQueue.top() << " "; // 0 1 2 3 4 5 6 8 9
//		pQueue.pop();
//	}

//	auto comparator = [](int a, int b) {
//		return a < b;
//	};
//
//	priority_queue<int, vector<int>, decltype(comparator)> pQueue(comparator);
//	vector<int> v = { 1, 4, 2, 6, 9, 0, 8, 3, 5 };
//	for (vector<int>::iterator it = v.begin(); it != v.end(); ++it)
//		pQueue.push(*it);
//
//	while (!pQueue.empty()) {
//		cout << pQueue.top() << " "; // 9 8 6 5 4 3 2 1 0
//		pQueue.pop();
//	}

//	auto comparator = [](int a, int b) {
//		return a > b;
//	};
//
//	priority_queue<int, vector<int>, decltype(comparator)> pQueue(comparator);
//	vector<int> v = { 1, 4, 2, 6, 9, 0, 8, 3, 5 };
//	for (vector<int>::iterator it = v.begin(); it != v.end(); ++it)
//		pQueue.push(*it);
//
//	while (!pQueue.empty()) {
//		cout << pQueue.top() << " "; // 0 1 2 3 4 5 6 8 9
//		pQueue.pop();
//	}

//	struct CustomCompare {
//		bool operator()(const int &lhs, const int &rhs) {
//			return lhs < rhs;
//		}
//	};
//
//	priority_queue<int, vector<int>, CustomCompare> pQueue;
//	vector<int> v = { 1, 4, 2, 6, 9, 0, 8, 3, 5 };
//	for (vector<int>::iterator it = v.begin(); it != v.end(); ++it)
//		pQueue.push(*it);
//
//	while (!pQueue.empty()) {
//		cout << pQueue.top() << " "; // 0 1 2 3 4 5 6 8 9
//		pQueue.pop();
//	}

	Student s1(1, 80, "Suresh", "patil");
	Student s2(2, 90, "Mahesh", "Jadhav");
	Student s3(3, 60, "Rupesh", "Yadav");
	Student s4(4, 90, "Ganesh", "Swami");

	vector<Student> studentData = { s1, s2, s3, s4 };
	cout << "Before adding into priority_queue:" << endl;

	for (vector<Student>::iterator it = studentData.begin();
			it != studentData.end(); ++it)
		(*it).display();
//	rollNumber:1 Marks:80 Name:Suresh Surname:patil
//	rollNumber:2 Marks:90 Name:Mahesh Surname:Jadhav
//	rollNumber:3 Marks:60 Name:Rupesh Surname:Yadav
//	rollNumber:4 Marks:90 Name:Ganesh Surname:Swami

	auto comparatorLess = [](Student a, Student b) {
		if (a.marks > b.marks)
			return true;
		else if (a.marks == b.marks)
			if (a.rollNumber > b.rollNumber)
				return true;
		return false;
	};

	priority_queue<Student, vector<Student>, decltype(comparatorLess)> pqLessStudent(
			comparatorLess);
	for (vector<Student>::iterator it = studentData.begin();
			it != studentData.end(); ++it)
		pqLessStudent.push(*it);

	pqLessStudent.emplace(5, 100, "Jotirling Swamiriya", "Swami");

	cout << "After adding into priority_queue pqLessStudent:" << endl;
	while (!pqLessStudent.empty()) {
		Student s1 = pqLessStudent.top();
		s1.display();
//		rollNumber:3 Marks:60 Name:Rupesh Surname:Yadav
//		rollNumber:1 Marks:80 Name:Suresh Surname:patil
//		rollNumber:2 Marks:90 Name:Mahesh Surname:Jadhav
//		rollNumber:4 Marks:90 Name:Ganesh Surname:Swami
//		rollNumber:5 Marks:100 Name:Jotirling Swamiriya Surname:Swami
		pqLessStudent.pop();
	}

//	The below is the lambda comparator function which takes as argument two data member and use the logic two compare,
//	false means the current position is okay, that is no swap required, true means swap required.
	auto comparatorGreater = [](Student a, Student b) {
		if (a.marks < b.marks)
			return true;
		else if (a.marks == b.marks)
			if (a.rollNumber > b.rollNumber)
				return true;
		return false;
	};

	priority_queue<Student, vector<Student>, decltype(comparatorGreater)> pqGreaterStudent(
			comparatorGreater);
	for (vector<Student>::iterator it = studentData.begin();
			it != studentData.end(); ++it)
		pqGreaterStudent.push(*it);

	pqGreaterStudent.emplace(5, 100, "Jotirling Swamiriya", "Swami");

	cout << "After adding into priority_queue pqGreaterStudent:" << endl;
	while (!pqGreaterStudent.empty()) {
		Student s1 = pqGreaterStudent.top();
		s1.display();
//		rollNumber:5 Marks:100 Name:Jotirling Swamiriya Surname:Swami
//		rollNumber:2 Marks:90 Name:Mahesh Surname:Jadhav
//		rollNumber:4 Marks:90 Name:Ganesh Surname:Swami
//		rollNumber:1 Marks:80 Name:Suresh Surname:patil
//		rollNumber:3 Marks:60 Name:Rupesh Surname:Yadav
		pqGreaterStudent.pop();
	}

	cout << endl;

	return 0;
}

