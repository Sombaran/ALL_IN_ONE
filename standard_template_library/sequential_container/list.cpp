/*
 * list.cpp
 *
 *  Created on: 16-Mar-2022
 *      Author: Jotirling Swami
 */

//	***  List  ***
//
//		1. Lists are sequence containers that allow non-contiguous memory allocation.
//		2. As compared to vector, the list has slow traversal, but once a position has been found, insertion and deletion are quick.
//		3. Normally, when we say a List, we talk about a doubly linked list.
//		4. For implementing a singly linked list, we use a forward list.
//		5. Below is the program to show the working of some functions of List:
//		6. ists are sequence containers that allow constant time insert and erase operations anywhere within the sequence, and iteration in both directions.
//
//		7. They are very similar to forward_list:
//				The main difference being that forward_list objects are single-linked lists, and thus they can only be iterated forwards,
//				in exchange for being somewhat smaller and more efficient.
//		8. Compared to other base standard sequence containers (array, vector and deque), lists perform generally better in
//				inserting, extracting and moving elements in any position within the container for which an iterator has already been obtained, and
//				therefore also in algorithms that make intensive use of these, like sorting algorithms.
//		9. The main drawback of lists and forward_lists compared to these other sequence containers is that
//				They lack direct access to the elements by their position; For example, to access the sixth element in a list,
//				one has to iterate from a known position (like the beginning or the end) to that position,
//				which takes linear time in the distance between these.
//				They also consume some extra memory to keep the linking information associated to each element
//				(which may be an important factor for large lists of small-sized elements).
//
//	Container properties :
//	Sequence
//		Elements in sequence containers are ordered in a strict linear sequence. Individual elements are accessed by their position in this sequence.
//	Doubly-linked list
//		Each element keeps information on how to locate the next and the previous elements, allowing constant time insert and erase operations before or after a specific element (even of entire ranges), but no direct random access.
//
//
//	Member functions
//
//	(constructor)	 Construct list
//	(destructor )	 List destructor
//	operator= 	Assign content
//
//	List Constructor
//		(1) empty container constructor (default constructor)
//			Constructs an empty container, with no elements.
//		(2) fill constructor
//			Constructs a container with n elements. Each element is a copy of val (if provided).
//		(3) range constructor
//			Constructs a container with as many elements as the range [first,last),
//			with each element emplace-constructed from its corresponding element in that range, in the same order.
//		(4) copy constructor (and copying with allocator)
//			Constructs a container with a copy of each of the elements in x, in the same order.
//		(5) move constructor (and moving with allocator)
//			Constructs a container that acquires the elements of x.
//			If alloc is specified and is different from x's allocator, the elements are moved. Otherwise,
//			no elements are constructed (their ownership is directly transferred). x is left in an unspecified but valid state.
//		(6) initializer list constructor
//			Constructs a container with a copy of each of the elements in il, in the same order.
//
//	List Destructor"
//		Destroys the container object.
//
//	List Operator:
//
//		(1) The copy assignment  - copies all the elements from x into the container (with x preserving its contents).
//		(2) The move assignment - moves the elements of x into the container (x is left in an unspecified but valid state).
//		(3) The initializer list assignment - copies the elements of il into the container.
//
//	Iterators:
//		Begin 	Return iterator to beginning
//		End 	Return iterator to end
//		Rbegin 	Return reverse iterator to reverse beginning
//		Rend 	Return reverse iterator to reverse end
//		cbegin  	Return const_iterator to beginning
//		cend  	Return const_iterator to end
//		crbegin 	Return const_reverse_iterator to reverse beginning
//		crend  	Return const_reverse_iterator to reverse end
//
//	Capacity:
//		Empty 	Test whether container is empty
//					Returns whether the list container is empty (i.e. whether its size is 0).
//		Size 	Return size
//					Returns the number of elements in the list container.
//		max_size	Return maximum size
//					 Returns the maximum number of elements that the list container can hold.
//
//	Element access:
//		Front 	Access first element
//					Returns a reference to the first element in the list container.
//					Unlike member list::begin, which returns an iterator to this same element,
//					this function returns a direct reference.
//					Calling this function on an empty container causes undefined behavior.
//							mylist.front()
//		Back 	Access last element
//					 Returns a reference to the last element in the list container.
//					Unlike member list::end, which returns an iterator just past this element, this function returns a direct reference.
//					Calling this function on an empty container causes undefined behavior.
//							mylist.back()
//
//	Modifiers:
//		Assign	 	Assign new content to container
//					1. In the range version (1), the new contents are elements constructed from each of the elements in the range between
//						first and last, in the same order.
//							second.assign (first.begin(),first.end());
//					2. In the fill version (2), the new contents are n elements, each initialized to a copy of val.
//							first.assign (7,100);
//					3. In the initializer list version (3), the new contents are copies of the values passed as initializer list, in the same order.
//							std::list<int>List = {1, 2, 3, 4, 5, 6};
//
//		emplace_front 	 Construct and insert element at beginning
//					Inserts a new element at the beginning of the list, right before its current first element.
//					This new element is constructed in place using args as the arguments for its construction.
//					This effectively increases the container size by one.
//					A similar member function exists, push_front, which either copies or moves an existing object into the container.
//
//		push_front	Insert element at beginning
//					Inserts a new element at the beginning of the list, right before its current first element.
//					The content of val is copied (or moved) to the inserted element.
//					This effectively increases the container size by one.
//
//		pop_front	Delete first element
//					 Removes the first element in the list container, effectively reducing its size by one.
//					This destroys the removed element.
//
//		emplace_back 	Construct and insert element at the end
//					Inserts a new element at the end of the list, right after its current last element.
//					This new element is constructed in place using args as the arguments for its construction.
//					This effectively increases the container size by one.
//					The element is constructed in-place by calling allocator_traits::construct with args forwarded.
//					A similar member function exists, push_back, which either copies or moves an existing object into the container.
//
//		push_back 	Add element at the end
//					Adds a new element at the end of the list container, after its current last element.
//					The content of val is copied (or moved) to the new element.
//					This effectively increases the container size by one.
//
//		pop_back 	Delete last element
//					Removes the last element in the list container, effectively reducing the container size by one.
//					This destroys the removed element.
//
//		emplace  	Construct and insert element
//					The container is extended by inserting a new element at position.
//					This new element is constructed in place using args as the arguments for its construction.
//					This effectively increases the container size by one.
//					Unlike other standard sequence containers, list and forward_list objects are specifically designed to be efficient inserting and
//					removing elements in any position, even in the middle of the sequence.
//					A similar member function exists, insert, which either copies or moves existing objects into the container.
//							mylist.emplace ( mylist.begin(), 100);
//
//		Insert 	Insert elements
//					The container is extended by inserting new elements before the element at the specified position.
//					This effectively increases the list size by the amount of elements inserted.
//					1. single element (1)
//							iterator insert (const_iterator position, const value_type& val);
//									mylist.insert (it,10);
//					2. fill (2)
//							iterator insert (const_iterator position, size_type n, const value_type& val);
//									mylist.insert (it,2,20);
//					3. range (3)
//							template <class InputIterator>
//							iterator insert (const_iterator position, InputIterator first, InputIterator last);
//									mylist.insert (it,myvector.begin(),myvector.end());
//					4. move (4)
//							iterator insert (const_iterator position, value_type&& val);
//					5. initializer list (5)
//							iterator insert (const_iterator position, initializer_list<value_type> il);
//
//		Erase 	Erase elements
//					Removes from the list container either a single element (position) or a range of elements ([first,last)).
//					This effectively reduces the container size by the number of elements removed, which are destroyed.
//							iterator erase (const_iterator position);
//							iterator erase (const_iterator first, const_iterator last);
//
//		Swap 	Swap content
//					Exchanges the content of the container by the content of x, which is another list of the same type. Sizes may differ.
//							first.swap(second);
//
//		Resize 	Change size
//					Resizes the container so that it contains n elements.
//					1. If n is smaller than the current container size, the content is reduced to its first n elements,
//						removing those beyond (and destroying them).
//					2. If n is greater than the current container size, the content is expanded by inserting at the end
//						as many elements as needed to reach a size of n. If val is specified, the new elements are initialized as copies of val,
//						otherwise, they are value-initialized.
//					3. Notice that this function changes the actual content of the container by inserting or erasing elements from it.
//							void resize (size_type n);
//							void resize (size_type n, const value_type& val);
//		Clear 	Clear content
//					 Removes all elements from the list container (which are destroyed), and leaving the container with a size of 0.
//								  mylist.clear();
//
//	Operations:
//		Splice 	Transfer elements from list to list
//		entire list (1)
//				void splice (const_iterator position, list& x);
//				void splice (const_iterator position, list&& x);
//		single element (2)
//				void splice (const_iterator position, list& x, const_iterator i);
//				void splice (const_iterator position, list&& x, const_iterator i);
//		element range (3)
//				void splice (const_iterator position, list& x, const_iterator first, const_iterator last);
//				void splice (const_iterator position, list&& x, const_iterator first, const_iterator last);
//
//				Transfers elements from x into the container, inserting them at position.
//				This effectively inserts those elements into the container and removes them from x, altering the sizes of both containers.
//				The operation does not involve the construction or destruction of any element.
//				They are transferred, no matter whether x is an lvalue or an rvalue, or whether the value_type supports move-construction or not.
//
//					The first version (1) transfers all the elements of x into the container.
//					The second version (2) transfers only the element pointed by i from x into the container.
//					The third version (3) transfers the range [first,last) from x into the container.
//
//	Remove 	Remove elements with specific value
//						void remove (const value_type& val);
//				Removes from the container all the elements that compare equal to val.
//				This calls the destructor of these objects and reduces the container size by the number of elements removed.
//				Unlike member function list::erase, which erases elements by their position (using an iterator),
//				this function (list::remove) removes elements by their value.
//
//	remove_if 	Remove elements fulfilling condition (public member function template )
//						void remove_if (Predicate pred);
//				Remove elements fulfilling condition
//				Removes from the container all the elements for which Predicate pred returns true.
//				This calls the destructor of these objects and reduces the container size by the number of elements removed.
//						bool single_digit (const int& value) { return (value<10); }
//						mylist.remove_if (single_digit);
//
//	Unique 	Remove duplicate values
//				The version with no parameters (1), removes all but the first element from every consecutive group of equal elements in the container.
//				Notice that an element is only removed from the list container if it compares equal to the element immediately preceding it.
//				Thus, this function is especially useful for sorted lists.
//				The second version (2), takes as argument a specific comparison function that determine the "uniqueness" of an element.
//					bool same_integral_part (double first, double second)
//					{ return ( int(first)==int(second) ); }
//				mylist.sort();
//				mylist.unique();
//				mylist.unique (same_integral_part);
//
//	Merge 	Merge sorted lists
//				Merges x into the list by transferring all of its elements at their respective ordered positions into the container
//				(both containers shall already be ordered).
//
//				This effectively removes all the elements in x (which becomes empty), and inserts them into their ordered position
//					within container (which expands in size by the number of elements transferred).
//				The operation is performed without constructing nor destroying any element:
//					they are transferred, no matter whether x is an lvalue or an rvalue, or whether the value_type supports move-construction or not.
//				(1) void merge (list& x);
//						  void merge (list&& x);
//				(2) template <class Compare>
//						  void merge (list& x, Compare comp);
//						template <class Compare>
//						  void merge (list&& x, Compare comp);
//
//	Sort 	Sort elements in container
//				(1)   void sort();
//				(2) template <class Compare>
//						  void sort (Compare comp);
//				Sorts the elements in the list, altering their position within the container.
//				The sorting is performed by applying an algorithm that uses either operator< (in version (1)) or comp (in version (2)) to compare elements.
//				This comparison shall produce a strict weak ordering of the elements
//					(i.e., a consistent transitive comparison, without considering its reflexiveness).
//				The resulting order of equivalent elements is stable:
//					i.e., equivalent elements preserve the relative order they had before the call.
//
//	Reverse 	Reverse the order of elements
//				Reverses the order of the elements in the list container.
//				mylist.reverse();
//
//
#include <iostream>
#include<list>
using namespace std;

class Base {
	int value;

public:
	Base(int v) :
			value { v } {
	}
	void printLists() {
		cout << value << " " << endl;
	}

	bool operator <(const Base &obj) {
		if (value < obj.value)
			return true;
		return false;
	}

};
void printList(list<string> &List) {
	for (list<string>::iterator it = List.begin(); it != List.end(); it++)
		cout << *it << " ";
	cout << endl;
}

void printList(list<int> &List) {
	for (list<int>::iterator it = List.begin(); it != List.end(); it++)
		cout << *it << " ";
	cout << endl;
}

void printList(list<Base> &List) {
	for (list<Base>::iterator it = List.begin(); it != List.end(); it++)
		it->printLists();
	cout << endl;
}

int main() {
	//-----------------------------------------------------------------------
	{
		cout << "insert" << endl;
		list<int> List = { 1, 2, 3, 4 };
		list<int>::iterator itr = List.begin();
		List.insert(itr, 5);
		printList(List);
	}
	//-----------------------------------------------------------------------
	{
		cout << "insert" << endl;
		list<string> stringList = { "C is a language" };
		list<string>::iterator stringitr = stringList.begin();
		stringList.insert(stringitr, "java ");
		stringList.insert(stringitr, 2, "project ");
		printList(stringList);
	}
	//-----------------------------------------------------------------------
	{
		cout << "insert" << endl;
		list<int> List = { 1, 2, 3, 4, 5 };
		list<int> intList1 = { 6, 7, 8, 9 };
		list<int>::iterator itr = List.begin();
		List.insert(itr, intList1.begin(), intList1.end());
		printList(List);
	}
	//-----------------------------------------------------------------------
	{
		cout << "push_back" << endl;
		list<string> List { "projot", "Joti", "Priya" };
		List.push_back("swami");
		printList(List);
	}
	//-----------------------------------------------------------------------
	{
		cout << "pop_back" << endl;
		list<string> List { "projot", "Joti", "Priya" };
		List.pop_back();
		printList(List);
	}
	//-----------------------------------------------------------------------
	{
		cout << "push_front" << endl;
		list<string> List { "projot", "Joti", "Priya" };
		List.push_front("swami");
		printList(List);
	}
	//-----------------------------------------------------------------------
	{
		cout << "pop_front" << endl;
		list<string> List { "projot", "Joti", "Priya" };
		List.pop_front();
		printList(List);
	}
	//-----------------------------------------------------------------------
	{
		cout << "max_size, empty clear, front, back" << endl;
		list<string> List { "projot", "Joti", "Priya" };
		printList(List);
		cout << "size: " << List.size() << endl;
		cout << "max_size: " << List.max_size() << endl;
		cout << "empty(): " << List.empty() << endl;
		cout << "front(): " << List.front() << endl;
		cout << "back(): " << List.back() << endl;
		List.clear();
		cout << "after clear empty(): " << List.empty() << endl;
	}
	//-----------------------------------------------------------------------
	{
		cout << "swap" << endl;
		list<string> List { "projot", "Joti", "Priya" };
		list<string> List1 { "cat", "dog", "horse" };
		cout << "Before Swap" << endl;
		printList(List);
		printList(List1);
		cout << "After Swap" << endl;
		List1.swap(List);
		printList(List);
		printList(List1);
	}
	//-----------------------------------------------------------------------
	{
		cout << "reverse" << endl;
		list<string> List { "projot", "Joti", "Priya" };
		list<string> List1 { "cat", "dog", "horse" };
		printList(List);
		printList(List1);
		cout << "After Reverse" << endl;
		List.reverse();
		List1.reverse();
		printList(List);
		printList(List1);
	}
	//-----------------------------------------------------------------------
	{
		cout << "reverse" << endl;
		list<int> List { 1, 4, 6, 3, 2, 9 };
		printList(List);
		cout << "After sort" << endl;
		List.sort();
		printList(List);

		list<string> List1 { "zebra", "dog", "cat", "horse" };
		printList(List1);
		cout << "After sort" << endl;
		List1.sort();
		printList(List1);
	}
	//-----------------------------------------------------------------------
	{
		cout << "sort" << endl;
		Base b1(60);
		Base b2(10);
		Base b3(140);
		Base b4(50);

		list<Base> List { b1, b2, b3, b4 };
		printList(List);
		cout << "After sort" << endl;
		List.sort();
		printList(List);
	}
	//-----------------------------------------------------------------------
	return 0;
}

