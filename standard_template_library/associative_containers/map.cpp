/*
 * map.cpp
 *
 *  Created on: 14-Mar-2022
 *      Author: Jotirling Swami
 */

//	*** Map Member Functions ***
//
//		1. Maps are part of the C++ STL (Standard Template Library).
//		2. Maps are the associative containers that store sorted key-value pair,
// 			in which each key is unique and
//		3. It can be inserted or deleted but cannot be altered.
//		4. Values associated with keys can be changed.
//
//		For example:
//				Keys	Values
//				101		Nikita
//				102		Robin
//				103		Deep
//				104		John
//
//		Syntax :
//			template < class Key,                                             // map::key_type
//					   class T,                                                     // map::mapped_type
//					   class Compare = less<Key>,                        // map::key_compare
//					   class Alloc = allocator<pair<const Key,T> >    // map::allocator_type
//					   > class map;
//
//		Parameter :
//			key: The key data type to be stored in the map.
//			type: The data type of value to be stored in the map.
//			compare: A comparison class that takes two arguments of the same type bool and returns a value.
//				This argument is optional and the binary 	predicate less<"key"> is the default value.
//
//		*** map functionality ***
//	Constructor/Destructor/operator
//		constructors	Construct map
//		destructors	Map destructor
//		operator=	Copy elements of the map to another map.
//
//	Iterators
//
//		begin	Returns an iterator pointing to the first element in the map.
//		cbegin	Returns a const iterator pointing to the first element in the map.
//		end	Returns an iterator pointing to the past-end.
//		cend	Returns a constant iterator pointing to the past-end.
//		rbegin	Returns a reverse iterator pointing to the end.
//		rend	Returns a reverse iterator pointing to the beginning.
//		crbegin	Returns a constant reverse iterator pointing to the end.
//		crend	Returns a constant reverse iterator pointing to the beginning.
//
//	Capacity
//		empty	Returns true if map is empty.
//		size	Returns the number of elements in the map.
//		max_size	Returns the maximum size of the map.
//
//	Element Access
//		operator[]	Retrieve the element with given key.
//		at			Retrieve the element with given key.
//
//	Modifiers
//		insert	Insert element in the map.
//		erase	Erase elements from the map.
//			void erase(iterator position);                           //until C++ 11
//			size_type erase(const key_type &k);              //until C++ 11
//			void erase(iterator first, iterator last);           //until C++ 11
//			iterator erase(const_iterator position);        //since C++ 11
//			size_type erase(const key_type &k);          //since C++ 11
//			iterator erase(const_iterator first, const_iterator last); //since C++ 11
//		swap	Exchange the content of the map.
//		clear	Delete all the elements of the map.
//		emplace	Construct and insert the new elements into the map.
//		emplace_hint	Construct and insert new elements into the map by hint.
//
//	Observers
//		key_comp	Return a copy of key comparison object.
//		value_comp	Return a copy of value comparison object.
//
//	Operations
//		find	Search for an element with given key.
//		count	Gets the number of elements matching with given key.
//		lower_bound	Returns an iterator to lower bound.
//		upper_bound	Returns an iterator to upper bound.
//		equal_range	Returns the range of elements matches with given key.
//
//	Non-Member Overloaded Functions
//		operator==	Checks whether the two maps are equal or not.
//		operator!=	Checks whether the two maps are equal or not.
//		operator<	Checks whether the first map is less than other or not.
//		operator<=	Checks whether the first map is less than or equal to other or not.
//		operator>	Checks whether the first map is greater than other or not.
//		operator>=	Checks whether the first map is greater than equal to other or not.
//		swap()	Exchanges the element of two maps.

#include <iostream>
#include <map>
using namespace std;

class Student
{
public:
	int rollNumber;
	int marks;
	Student(int r, int m) : rollNumber{r}, marks{m}
	{
	}

	void display() const
	{
		cout << "rollNumber: " << rollNumber << ", marks: " << marks << endl;
	}

	bool operator<(const Student &obj) const
	{
		if (this->marks < obj.marks)
			return true;
		else if (this->marks == obj.marks)
		{
			if (this->rollNumber < obj.rollNumber)
				return true;
		}
		return false;
	}

	bool operator>(const Student &obj) const
	{
		if (this->marks > obj.marks)
			return true;
		else if (this->marks == obj.marks)
		{
			if (this->rollNumber > obj.rollNumber)
				return true;
		}
		return false;
	}
};

void printMap(map<int, string> &Map)
{
	// map.empty()
	if (!Map.empty())
	{
		for (map<int, string>::iterator it = Map.begin(); it != Map.end(); ++it)
			cout << " key: " << it->first << " " << it->second << endl;
	}
	cout << endl;
}

int main()
{
	//-----------------------------------------------------------------------
	map<int, string> Map{{1, "one"}, {2, "two"}, {3, "three"}, {4, "four"}, {5, "five"}};
	printMap(Map);

	map<int, string> Map1(Map.begin(), Map.end());
	printMap(Map1);

	map<int, string> Map2(Map.find(3), Map.end());
	printMap(Map2);
	//-----------------------------------------------------------------------
	cout << "Map2.max_size(): " << Map2.max_size() << endl;
	cout << "Map2.size(): " << Map2.size() << endl;
	Map2.clear();
	cout << "Map2.max_size(): " << Map2.max_size() << endl;
	cout << "Map2.size(): " << Map2.size() << endl;
	//-----------------------------------------------------------------------
	map<int, string>::iterator it = Map.find(4);
	if (it != Map.end())
		cout << "found key: " << it->first << " " << it->second << endl;

	Map.erase(4);
	it = Map.find(4);
	if (it != Map.end())
		cout << "key found" << endl;
	else
		cout << "key not found" << endl;
	//-----------------------------------------------------------------------
	it = Map.find(2);
	if (it != Map.end())
		Map.erase(it);

	printMap(Map);

	cout << "print Map1" << endl;
	printMap(Map1);
	it = Map1.begin();
	for (int i = 0; i < 3; i++)
		Map1.erase(it++);
	cout << "print Map1" << endl;
	printMap(Map1);

	map<int, string> Map3 = Map1;
	cout << "print Map3" << endl;
	printMap(Map3);
	//-----------------------------------------------------------------------
	Map.insert(Map.begin(), pair<int, string>(8, "new"));

	pair<map<int, string>::iterator, bool> ret;

	ret = Map.insert(pair<int, string>(6, "six"));
	cout << "insert ret: " << ret.second << endl; // success return 1
	ret = Map.emplace(6, "six");
	cout << "emplace ret: " << ret.second << endl; // fail return 0
	cout << "print Map" << endl;
	printMap(Map);
	//-----------------------------------------------------------------------
	map<int, string, greater<>> Mapp{{1, "aa"}, {2, "bb"}, {3, "cc"}, {4, "dd"}, {5, "ff"}};
	for (map<int, string>::iterator it = Mapp.begin(); it != Mapp.end(); ++it)
		cout << " key: " << it->first << " " << it->second << endl;
	cout << endl;
	//-----------------------------------------------------------------------

	map<int, char> map4{{1, 'A'}, {2, 'B'}, {3, 'C'}, {4, 'D'}};
	map<int, char> map8{{5, 'E'}, {6, 'F'}, {7, 'G'}, {8, 'H'}};

	for (auto &a : map4)
	{
		cout << a.first << " - " << a.second << " ";
	}
	cout << endl;
	for (auto &a : map8)
	{
		cout << a.first << " - " << a.second << " ";
	}
	cout << endl;
	map4.swap(map8); // map swap
	cout << endl;
	for (auto &a : map4)
	{
		cout << a.first << " - " << a.second << " ";
	}
	cout << endl;
	for (auto &a : map8)
	{
		cout << a.first << " - " << a.second << " ";
	}
	cout << endl;
	//-----------------------------------------------------------------------

	Student s1(1, 60);
	Student s2(2, 90);
	Student s3(3, 70);

	map<int, Student, less<>> studentMap{{1, s1}, {2, s2}, {3, s3}};

	Student s4(4, 100);
	studentMap.insert(pair<int, Student>(4, s3));

	studentMap.emplace(5, Student(4, 200));

	map<int, Student> studentMapNew(studentMap.begin(), studentMap.end());
	map<int, Student>::iterator it1 = studentMapNew.begin();
	it1 = studentMapNew.find(3);
	if (it1 != studentMapNew.end())
	{
		cout << it1->first << " ";
		it1->second.display();
	}
	cout << "studentMap :" << endl;
	for (map<int, Student>::iterator it = studentMap.begin();
		 it != studentMap.end(); ++it)
	{
		cout << " key: " << it->first << " value :";
		it->second.display();
	}

	map<int, Student, greater<>> studentMap1(studentMap.begin(),
											 studentMap.end());
	cout << "studentMap1 :" << endl;
	for (map<int, Student>::iterator it = studentMap1.begin();
		 it != studentMap1.end(); ++it)
	{
		cout << " key: " << it->first << " value :";
		it->second.display();
	}

	cout << endl;

	//-------------------------------------------------------------------------
	{
		// declare Multimap
		typedef std::multimap<std::string, int> MapType;
		MapType myMap;

		// insertion
		myMap.insert(MapType::value_type("test", 42));
		myMap.insert(MapType::value_type("test", 45));
		myMap.insert(MapType::value_type("other-test", 0));

		auto it = myMap.find("test");
		if (it != myMap.end())
			std::cout << "value for " << it->first << " is " << it->second << std::endl;
		else
			std::cout << "value not found" << std::endl;
	}
	//-------------------------------------------------------------------------
	return 0;
}
