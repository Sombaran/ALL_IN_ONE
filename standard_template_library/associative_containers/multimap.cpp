/*
 * multimap.cpp
 *
 *  Created on: 14-Mar-2022
 *      Author: Jotirling Swami
 */

//	Multimaps are part of the C++ STL (Standard Template Library).
//	Multimaps are the associative containers like map that stores sorted key-value pair,
//		but unlike maps which store only unique keys, multimap can have duplicate keys.
//		By default it uses < operator to compare the keys.
//
//		For example:
//			A multimap of Employees where employee age is the key and
//			name is the value can be represented as:
//
//				Keys	Values
//				23		Nikita
//				28		Robin
//				25		Deep
//				25		Aman
//
//	Syntax
//		template < class Key,                                     // multimap::key_type
//				   class T,                                                     // multimap::mapped_type
//				   class Compare = less<Key>,                        // multimap::key_compare
//				   class Alloc = allocator<pair<const Key,T> >    // multimap::allocator_type
//				   > class multimap;
//	Parameter
//	key: 		The key data type to be stored in the multimap.
//	type: 		The data type of value to be stored in the multimap.
//	compare:	A comparison class that takes two arguments of the same type bool and
//					returns a value. This argument is optional and
//					the binary predicate less<"key"> is the default value.
//	Member Functions
//	Constructor/Destructor/operator
//
//		constructor	Construct multimap
//		destructor	Multimap destructor
//		operator=	Copy elements of the multimap to another multimap.
//
//	Iterators
//
//		begin	Returns an iterator pointing to the first element in the multimap.
//		cbegin	Returns a const_iterator pointing to the first element in the multimap.
//		end	Returns an iterator pointing to the past-end.
//		cend	Returns a constant iterator pointing to the past-end.
//		rbegin	Returns a reverse iterator pointing to the end.
//		rend	Returns a reverse iterator pointing to the beginning.
//		crbegin	Returns a constant reverse iterator pointing to the end.
//		crend	Returns a constant reverse iterator pointing to the beginning.
//
//	Capacity
//
//		empty	Return true if multimap is empty.
//		size	Returns the number of elements in the multimap.
//		max_size	Returns the maximum size of the multimap.
//
//	Modifiers
//
//		insert	Insert element in the multimap.
//					single element (1)
//						pair<iterator,bool> insert (const value_type& val);//until C++ 11
//					hint (2)
//						iterator insert (iterator position, const value_type& val);//until C++ 11
//					range (3)
//						template <class InputIterator>
//						void insert (InputIterator first, InputIterator last);//until C++ 11
//					single element (1)
//						pair<iterator,bool> insert (const value_type& val);
//						template<class P> pair<iterator, bool> insert(P &&val); //since C++ 11
//					hint (2)
//						iterator insert (const_iterator position, const value_type& val);
//						template<class P> iterator insert(const_iterator position, P &&val);
//					range (3)
//						template <class InputIterator>
//						void insert (InputIterator first, InputIterator last); //since C++ 11
//					initializer list (4)
//						void insert (initializer_list<value_type> il);//since C++ 11
//		erase	Erase elements from the multimap.
//					void erase (iterator position);                           //until C++ 11
//					size_type erase (const key_type& k);              //until C++ 11
//					void erase (iterator first, iterator last);           //until C++ 11
//					iterator  erase (const_iterator position);        //since C++ 11
//					size_type erase (const key_type& k);          //since C++ 11
//					iterator  erase (const_iterator first, const_iterator last); //since C++ 11
//		swap	Exchange the content of the multimap.
//		clear	Delete all the elements of the multimap.
//		emplace	Construct and insert the new elements into the multimap.
//		emplace_hint	Construct and insert new elements into the multimap by hint.
//
//	Observers
//
//		key_comp	Return a copy of key comparison object.
//		value_comp	Return a copy of value comparison object.
//
//	Operations
//
//		find	Search for an element with given key.
//		count	Gets the number of elements matching with given key.
//		lower_bound	Returns an iterator to lower bound.
//		upper_bound	Returns an iterator to upper bound.
//		equal_range()	Returns the range of elements matches with given key.
//
//	Non-Member Overloaded Functions
//
//		operator==	Checks whether the two multimaps are equal or not.
//		operator!=	Checks whether the two multimaps are equal or not.
//		operator<	Checks whether the first multimap is less than other or not.
//		operator<=	Checks whether the first multimap is less than or equal to other or not.
//		operator>	Checks whether the first multimap is greater than other or not.
//		operator>=	Checks whether the first multimap is greater than equal to other or not.
//		swap()		Exchanges the element of two multimaps.
#include <iostream>
#include <map>
using namespace std;

class Student {
public:
	int rollNumber;
	int marks;
	Student(int r, int m) {
		rollNumber = r;
		marks = m;
	}

	void display() {
		cout << "rollNumber: " << rollNumber << ", marks: " << marks << endl;
	}

};

int main() {

	//-----------------------------------------------------------------------
	multimap<int, string> mmap { { 1, "priya" }, { 2, "prajot" }, { 3, "Joti" },
			{ 4, "swami" } };

	for (multimap<int, string>::iterator it = mmap.begin(); it != mmap.end();
			++it)
		cout << "mmap:" << it->first << " " << it->second << endl;
	cout << endl;

	multimap<int, string> mmapNew = mmap;
	for (auto a : mmapNew)
		cout << "mmapNew:" << a.first << " " << a.second << endl;
	cout << endl;

	multimap<int, string> mmapNew1(mmap.begin(), mmap.end());
	for (auto a : mmapNew1)
		cout << "mmapNew1:" << a.first << " " << a.second << endl;
	cout << endl;

	multimap<int, string> mmapNew2(mmap.find(3), mmap.end());
	for (auto a : mmapNew2)
		cout << "mmapNew2:" << a.first << " " << a.second << endl;
	cout << endl;

	//-----------------------------------------------------------------------

	if (!mmapNew.empty()) { //  map.empty()
		for (auto a : mmapNew)
			cout << "mmapNew1:" << a.first << " " << a.second << endl;
		cout << endl;
	}

	mmapNew.clear(); //  map.clear()
	if (!mmapNew.empty())
		cout << "non empty map" << endl;
	else
		cout << "empty map" << endl;
	cout << endl;
	//----------------------------------------------------------------------
	mmapNew1.swap(mmapNew2); //	map.swap()
	for (auto a : mmapNew1)
		cout << "mmapNew1:" << a.first << " " << a.second << endl;
	cout << endl;
	for (auto a : mmapNew2)
		cout << "mmapNew2:" << a.first << " " << a.second << endl;
	cout << endl;
	//----------------------------------------------------------------------

	multimap<int, string>::iterator it;
	it = mmap.insert(pair<int, string>(1, "priyanka"));
	cout << it->first << " " << it->second << endl;
	it = mmap.insert(pair<int, string>(3, "jotirling"));
	cout << it->first << " " << it->second << endl;

	for (auto a : mmap)
		cout << "mmap:" << a.first << " " << a.second << endl;
	cout << endl;

	//----------------------------------------------------------------------

	multimap<int, char> intMap4 { { 1, 'a' }, { 2, 'b' }, { 3, 'c' }, { 4, 'd' } };
	multimap<int, char> intMap9 { { 5, 'e' }, { 6, 'f' }, { 7, 'g' }, { 8, 'h' } };

	intMap9.insert(intMap4.begin(), intMap4.end());

	for (auto a : intMap9)
		cout << "intMap9:" << a.first << " " << a.second << endl;
	cout << endl;

	multimap<int, char> intMapNew;
	intMapNew.insert(intMap4.find(3), intMap4.end());

	intMapNew.insert(pair<int, char>(1, 'P'));
	intMapNew.emplace(1, 'P');

	for (auto a : intMapNew)
		cout << "intMapNew:" << a.first << " " << a.second << endl;
	cout << endl;
	//----------------------------------------------------------------------

	multimap<int, char, greater<>> intMapGrt;
	intMapGrt.insert(intMap4.begin(), intMap4.end());
	for (auto a : intMapGrt)
		cout << "intMapGrt:" << a.first << " " << a.second << endl;
	cout << endl;

	multimap<int, char, less<>> intMapLes;
	intMapLes.insert(intMap4.begin(), intMap4.end());
	for (auto a : intMapLes)
		cout << "intMapGrt:" << a.first << " " << a.second << endl;
	cout << endl;

	//----------------------------------------------------------------------

	Student s1(1, 100);
	Student s2(2, 200);
	Student s3(3, 300);
	Student s4(3, 400);

	multimap<int, Student> mstudent { { 10, s1 }, { 20, s2 }, { 30, s3 } };

	mstudent.insert(pair<int, Student>(30, s4));
	mstudent.insert(pair<int, Student>(40, Student(4, 400)));

//	emplace works as fine with the parameter list of the insert.
//	mstudent.emplace(pair<int, Student>(30, s4));
//	mstudent.emplace(pair<int, Student>(40, Student(4, 400)));

	mstudent.emplace(40, Student(4, 400));
	for (auto a : mstudent) {
		cout << "mstudent:" << a.first << " ";
		a.second.display();
	}
	cout << endl;

//----------------------------------------------------------------------

	multimap<int, Student, greater<>> mstudentGrt;

	mstudentGrt.insert(mstudent.begin(), mstudent.end());
	for (auto a : mstudentGrt) {
		cout << "mstudent:" << a.first << " ";
		a.second.display();
	}
	cout << endl;
	//----------------------------------------------------------------------
	map<char, int> m = { { 'a', 1 }, { 'b', 2 }, { 'c', 3 }, { 'c', 4 }, { 'e',
			5 }, };

	auto ret = m.equal_range('c'); // if the key is not present the it will through the garbege

	cout << "Lower bound of c is: " << ret.first->first << " = "
			<< ret.first->second << endl;

	cout << "Upper bound of c is: " << ret.second->first << " = "
			<< ret.second->second << endl;

	multimap<char, int>::iterator itt = m.lower_bound('c'); // if the key is not present the it will through the garbege
	cout << "lower_bound of c is: " << itt->first << endl;
	cout << "lower_bound of c is: " << itt->second << endl;

	itt = m.upper_bound('c'); // if the key is not present the it will through the garbege
	cout << "upper_bound of c is: " << itt->first << endl;
	cout << "upper_bound of c is: " << itt->second << endl;

	//----------------------------------------------------------------------

	return 0;
}
