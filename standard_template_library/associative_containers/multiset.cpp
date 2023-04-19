//============================================================================
// Name        : multiset.cpp
// Author      : Jotirling Swami
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

//		***  Multisets  ***

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
//				1. insert(Key key)
//					Inserts key (if present insert at upper_bound())
//					Returns iterator to inserted element
//				2. insert(iterator posl, iterator p052)
//					Inserts elements in range [posl, posz)
//					Returns nothing (void) _
//				3. insert(initializer_list l) ’
//					Inserts elements From initializer list
//					Returns nothing (void)
//			Erase			Erase elements from the set. if success then return 1 else 0
//				1. erase(iterator pos)
//					Removes element at position pos
//					Returns iterator Following last element removed
//				2. erase(iterator posl, iterator posZ) '
//					Removes elements in range [posl, posZ)
//					Returns iterator Following last element removed
//				3. erase(key)
//					Removes all elements with Key (iF present) key
//					Returns number oF elements removed
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
//			upper_bound		Returns an iterator to upper bound.	is > of given value
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

//		Multisets are a type of associative containers similar to the set,
//		with the exception that multiple elements can have the same values.
//		Some Basic Functions associated with multiset:
//
//		Multisets :
//			A multiset is an unordered collection of elements, in which the multiplicity of an element may be one or more than one or zero.
//			The multiplicity of an element is the number of times the element repeated in the multiset. In other words,
//			we can say that an element can appear any number of times in a set.
//
//			Example:
//			A = {l, l, m, m, n, n, n, n}
//			B = {a, a, a, a, a, c}
//
//		Operations on Multisets :
//			1.  Union of Multisets:
//					The Union of two multisets A and B is a multiset such that the multiplicity of an element
//				is equal to the maximum of 	the multiplicity of an element in A and B and is denoted by A ∪ B.
//
//		Example:
//			Let A = {l, l, m, m, n, n, n, n}
//				B = {l, m, m, m, n},
//			A ∪ B = {l, l, m, m, m, n, n, n, n}
//
//		2. Intersections of Multisets:
//				The intersection of two multisets A and B, is a multiset such that the multiplicity of an element is equal to
//				the minimum of the multiplicity of an element in A and B and is denoted by A ∩ B.
//
//			Example:
//			Let A = {l, l, m, n, p, q, q, r}
//				B = {l, m, m, p, q, r, r, r, r}
//			A ∩ B = {l, m, p, q, r}.
//
//		3. Difference of Multisets:
//				The difference of two multisets A and B, is a multiset such that the multiplicity of an element is equal to the
//				multiplicity of the element in A minus the multiplicity of the element in B if the difference is +ve,
//				and is equal to 0 if the difference is 	0 or negative.
//
//			Example:
//			Let A = {l, m, m, m, n, n, n, p, p, p}
//				B = {l, m, m, m, n, r, r, r}
//			A - B = {n, n, p, p, p}
//
//		4. Sum of Multisets:
//				The sum of two multisets A and B, is a multiset such that the multiplicity of an element is equal to
//				the sum of the 	multiplicity of an element in A and B
//
//			Example:
//			Let A = {l, m, n, p, r}
//				B = {l, l, m, n, n, n, p, r, r}
//			A + B = {l, l, l, m, m, n, n, n, n, p, p, r, r, r}
//
//		5. Cardinality of Sets:
//				The cardinality of a multiset is the number of distinct elements in a multiset without considering the
//				multiplicity of an element
//
//			Example:
//			A = {l, l, m, m, n, n, n, p, p, p, p, q, q, q}
//			The cardinality of the multiset A is 5.

#include <iostream>
#include <set>
using namespace std;

class Student {
public:
	int rollNumber;
	int marks;
	Student(int r, int m) :
			rollNumber { r }, marks { m } {
	}

	void display() const {
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
void PrintElement(multiset<T> &Set) {
	for (typename multiset<T>::iterator it = Set.begin(); it != Set.end(); ++it)
		cout << *it << " ";
	cout << endl;
}

int main() {

	//-----------------------------------------------------------------------
	multiset<int> mset = { 1, 2, 9, 2, 5, 2, 7, 5, 8 };
	PrintElement(mset);

	mset.insert(5);
	PrintElement(mset);

	mset.emplace(3);
	PrintElement(mset);

	mset.insert( { 4, 2, 1, 9, 9 });
	PrintElement(mset); //1 1 2 2 2 2 3 4 5 5 5 7 8 9 9 9

	multiset<int> intSetNew(mset.begin(), mset.find(7));
	PrintElement(intSetNew); //1 1 2 2 2 2 3 4 5 5 5

	//-----------------------------------------------------------------------

	multiset<int, std::greater<int>> newmSet;
	newmSet.insert(mset.begin(), mset.end());
	cout << "use of std::greater<int> : ";
	for (multiset<int>::iterator it = newmSet.begin(); it != newmSet.end();
			++it)
		cout << *it << " ";
	cout << endl;
	//-----------------------------------------------------------------------

	multiset<int>::iterator it;
	it = mset.upper_bound(5);
	cout << "upper_bound(5): " << *it << endl; // if present upper_bound is 7
	it = mset.lower_bound(5);
	cout << "lower_bound(5): " << *it << endl; // if present lower_bound is 5

	it = mset.upper_bound(6);
	cout << "upper_bound(6): " << *it << endl; // if not present upper_bound is 7
	it = mset.lower_bound(6);
	cout << "lower_bound(6): " << *it << endl; // if not present lower_bound is 7

	//-----------------------------------------------------------------------

	mset.erase(5);
	PrintElement(mset); //1 1 2 2 2 2 3 4 7 8 9 9 9

	mset.erase(mset.find(7)); //1 1 2 2 2 2 3 4 8 9 9 9
	PrintElement(mset);

	//-----------------------------------------------------------------------

	multiset<int>::iterator it1 = mset.begin();
	it1 = mset.find(2);
	if (*it1 == 2)
		cout << "able find the element : " << *it1 << endl;
	else
		cout << "not able find the element : " << 2 << endl;

	it1 = mset.find(5);
	if (*it1 == 5)
		cout << "able find the element : " << 5 << endl;
	else
		cout << "not able find the element : " << 5 << endl;
	PrintElement(mset);

	//-----------------------------------------------------------------------

	Student s1(1, 60);
	Student s2(2, 90);
	Student s3(3, 70);

	multiset<Student, less<Student>> StudentData = { s1, s2, s3 };
	multiset<Student>::iterator result;
	result = StudentData.emplace(4, 100);
//	result->display();
	cout << "sizeof MultiSet : " << StudentData.size() << endl;
	result = StudentData.emplace(4, 100);
	cout << "sizeof MultiSet : " << StudentData.size() << endl;
	Student s4(4, 100);
	result = StudentData.insert(s4);
	cout << "sizeof MultiSet : " << StudentData.size() << endl;

	cout << "use of std::less<Student> : " << endl;
	for (auto it : StudentData)
		it.display();
	//-----------------------------------------------------------------------
	multiset<Student, greater<Student>> newStudent;
	newStudent.insert(StudentData.begin(), StudentData.end());
	cout << "use of std::greater<Student> : " << endl;
	for (auto it : newStudent)
		it.display();
	//-----------------------------------------------------------------------
	return 0;

}

