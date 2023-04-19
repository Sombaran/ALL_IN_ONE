//============================================================================
// Name        : set.cpp
// Author      : Jotirling Swami
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

//		Member functions :
//
//		(constructor) : constructs the set
//						// (1) Default constructor
//								std::set<std::string> a;
//								a.insert("cat");
//								a.insert("dog");
//								a.insert("horse");
//								for(auto& str: a) std::cout << str << ' ';
//								std::cout << '\n';
//
//						// (2) Iterator constructor
//								std::set<std::string> b(a.find("dog"), a.end());
//								for(auto& str: b) std::cout << str << ' ';
//								std::cout << '\n';
//
//						// (3) Copy constructor
//								std::set<std::string> c(a);
//								c.insert("another horse");
//								for(auto& str: c) std::cout << str << ' ';
//								std::cout << '\n';
//
//						// (4) Move constructor
//								std::set<std::string> d(std::move(a));
//								for(auto& str: d) std::cout << str << ' ';
//								std::cout << '\n';
//								std::cout << "moved-from set is ";
//								for(auto& str: a) std::cout << str << ' ';
//								std::cout << '\n';
//
//						// (5) Initializer list constructor
//								std::set<std::string> e {"one", "two", "three", "five", "eight"};
//								for(auto& str: e) std::cout << str << ' ';
//								std::cout << '\n';
//
//		(destructor) : destructs the set
//				Destructs the set. The destructors of the elements are called and the used storage is deallocated. Note, that if the elements are pointers, the pointed-to objects are not destroyed.
//				Complexity : Linear in the size of the set.
//
//		operator= : assigns values to the container
//
//					std::set<int> x { 1, 2, 3 }, y, z;
//					const auto w = { 4, 5, 6, 7 };
//
//					std::cout << "Initially:\n";
//					print("x = ", x); // x = { 1, 2, 3 }
//					print("y = ", y); // y = {}
//					print("z = ", z); // z = {}
//
//					std::cout << "Copy assignment copies data from x to y:\n";
//					y = x;
//					print("x = ", x); //x = { 1, 2, 3 }
//					print("y = ", y); //y = { 1, 2, 3 }
//
//					std::cout << "Move assignment moves data from x to z, modifying both x and z:\n";
//					z = std::move(x);
//					print("x = ", x); //x = { }
//					print("z = ", z); //z = { 1, 2, 3 }
//
//					std::cout << "Assignment of initializer_list w to z:\n";
//					z = w;
//					print("w = ", w); //w = { 4, 5, 6, 7 }
//					print("z = ", z); //z = { 4, 5, 6, 7 }
//
//		Iterators
//			Begin		Returns an iterator pointing to the first element in the set.
//			cbegin		Returns a const iterator pointing to the first element in the set.
//			End			Returns an iterator pointing to the past-end.
//			Cend		Returns a constant iterator pointing to the past-end.
//			rbegin		Returns a reverse iterator pointing to the end.
//			Rend		Returns a reverse iterator pointing to the beginning.
//			crbegin		Returns a constant reverse iterator pointing to the end.
//			Crend		Returns a constant reverse iterator pointing to the beginning.
//
//		Capacity
//			empty			Returns true if set is empty.
//			Size			Returns the number of elements in the set.
//			max_size		Returns the maximum size of the set.
//
//		Modifiers
//			insert			Insert element in the set.
//			Erase			Erase elements from the set. if success then return 1 else 0
//			Swap			Exchange the content of the set.
//			Clear			Delete all the elements of the set.
//			emplace			Construct and insert the new elements into the set.
//			emplace_hint	Construct and insert new elements into the set by hint.
//
//		Observers
//			key_comp	Return a copy of key comparison object.
//			value_comp	Return a copy of value comparison object.
//
//		Operations
//			Find			Search for an element with given key.
//			count			Gets the number of elements matching with given key.
//			lower_bound		Returns an iterator to lower bound. is >= of given value
//			upper_bound		Returns an iterator to upper bound.	is > of gival value
//			equal_range		Returns the range of elements matches with given key.
//
//		Allocator
//			get_allocator	Returns an allocator object that is used to construct the set.
//
//		Non-Member Overloaded Functions
//			operator==	Checks whether the two sets are equal or not.
//			operator!=	Checks whether the two sets are equal or not.
//			operator<	Checks whether the first set is less than other or not.
//			operator<=	Checks whether the first set is less than or equal to other or not.
//			operator>	Checks whether the first set is greater than other or not.
//			operator>=	Checks whether the first set is greater than equal to other or not.
//			swap()		Exchanges the element of two sets.

//		*** Properties ***
//
//		1. The set stores the elements in sorted order.
//		2. All the elements in a set have unique values.
//		3. The value of the element cannot be modified once it is added to the set, though it is possible to remove and
//		   then add the modified value of that element. Thus, the values are immutable.
//		4. Sets follow the Binary search tree implementation.
//		5. The values in a set are un-indexed.

#include <iostream>
#include <set>
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

	bool operator <(const Student &obj) const {
		if (this->marks < obj.marks)
			return true;
		else if (this->marks == obj.marks) {
			if (this->rollNumber < obj.rollNumber)
				return true;
		}
		return false;
	}

	bool operator >(const Student &obj) const {
		if (this->marks > obj.marks)
			return true;
		else if (this->marks == obj.marks) {
			if (this->rollNumber > obj.rollNumber)
				return true;
		}
		return false;
	}

};

template<typename T>
void PrintElement(set<T> &Set) {
	for (typename set<T>::iterator it = Set.begin(); it != Set.end(); ++it)
		cout << *it << " ";
	cout << endl;
}

int main() {

	//-----------------------------------------------------------------------

	set<int> intSet = { 1, 4, 3, 5, 7, 9, 2, 8 };

	PrintElement(intSet);

	set<char> charSet = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H' };

	PrintElement(charSet);

	set<int> intSetNew(intSet.begin(), intSet.end());
	PrintElement(intSetNew);

	set<int> intSetNew1(intSet.find(3), intSet.end());
	PrintElement(intSetNew1);

	//-----------------------------------------------------------------------
	set<int, std::greater<int>> newSet;
	newSet.insert(intSet.begin(), intSet.end());
	cout << "use of std::greater<int> : ";
	for (set<int>::iterator it = newSet.begin(); it != newSet.end(); ++it)
		cout << *it << " ";
	cout << endl;
	//-----------------------------------------------------------------------

	intSetNew1.erase(5);
	PrintElement(intSetNew1);

	charSet.erase(charSet.find('G'));
	PrintElement(charSet);

	//-----------------------------------------------------------------------

	std::pair<std::set<char>::iterator, bool> ret = charSet.insert('B');
	cout << "insert : " << *ret.first << " " << ret.second << endl;

	PrintElement(charSet);

	//-----------------------------------------------------------------------

	cout << "upper_bound : " << *(charSet.upper_bound('B')) << " "
			<< "lower_bound : " << *(charSet.lower_bound('B')) << endl;
	cout << charSet.erase('B') << endl;
	cout << "upper_bound : " << *(charSet.upper_bound('B')) << " "
			<< "lower_bound : " << *(charSet.lower_bound('B'));
	PrintElement(charSet);

	//-----------------------------------------------------------------------

	Student s1(1, 60);
	Student s2(2, 90);
	Student s3(3, 70);

	set<Student> StudentData = { s1, s2, s3 };

	//-----------------------------------------------------------------------

	pair<set<Student>::iterator, bool> result;
	result = StudentData.emplace(4, 100);
	cout << "emplace: " << result.second << endl;  // success return 1
	result = StudentData.emplace(4, 100);
	cout << "emplace: " << result.second << endl;  // fail return 0
	for (auto it : StudentData)
		it.display();

	//-----------------------------------------------------------------------
	Student s4(4, 100);
	result = StudentData.insert(s4);
	cout << "insert: " << result.second << endl;  // fail return 0
	//-----------------------------------------------------------------------

	set<Student, greater<Student> > newStudent;
	newStudent.insert(StudentData.begin(), StudentData.end());
	cout << "use of std::greater<Student> : " << endl;
	for (auto it : newStudent)
		it.display();
	//-----------------------------------------------------------------------

	return 0;
}

