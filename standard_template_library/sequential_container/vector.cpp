/*
 * vector.cpp
 *
 *  Created on: 16-Mar-2022
 *      Author: Jotirling Swami
 */

// ***  Vector ***
#include <iostream>
#include <vector>
#include <cstring>

int main() {

	std::vector<int> myvector;

// Capacity:
// size
// Return size

	std::cout << "0. size: " << myvector.size() << '\n';

// max_size
// Return maximum size

	for (int i = 0; i < 3; i++)
		myvector.push_back(i);

	std::cout << "size: " << myvector.size() << "\n";
	std::cout << "capacity: " << myvector.capacity() << "\n";
	std::cout << "max_size: " << myvector.max_size() << "\n";

// resize
// Change size

	myvector.resize(5);
	myvector.resize(8, 100);
	myvector.resize(12);

	std::cout << "myvector contains:";
	for (int i = 0; i < myvector.size(); i++)
		std::cout << ' ' << myvector[i];
	std::cout << '\n';

// capacity
// Return size of allocated storage capacity
	std::vector<int> myvectorNew;
	// set some content in the vector:
	for (int i = 0; i < 5; i++)
		myvector.push_back(i);

	std::cout << "size: " << myvector.size() << "\n";
	std::cout << "capacity: " << myvector.capacity() << "\n";
	std::cout << "max_size: " << myvector.max_size() << "\n";

// empty
// Test whether vector is empty

	while (!myvector.empty()) {
		std::cout << "empty size: " << myvector.size() << "\n";
		myvector.pop_back();
	}
	std::cout << "empty size: " << myvector.size() << "\n";

// reserve
// Request a change in capacity

	for (int i = 0; i < 5; i++)
		myvector.push_back(i);

	myvector.reserve(100);

	std::cout << "reserve size: " << myvector.size() << "\n";
	std::cout << "reserve capacity: " << myvector.capacity() << "\n";
	std::cout << "reserve max_size: " << myvector.max_size() << "\n";

// shrink_to_fit
// Shrink to fit

	myvector.shrink_to_fit();

	std::cout << "shrink_to_fit size: " << myvector.size() << "\n";
	std::cout << "shrink_to_fit capacity: " << myvector.capacity() << "\n";
	std::cout << "shrink_to_fit max_size: " << myvector.max_size() << "\n";

// Element access:
// operator[]
// Access element

// A similar member function, vector::at, has the same behavior as this operator function,
// except that vector::at checks the vector bounds and
// signals whether n is out of range by throwing an exception.

// If the container size is greater than n, the function never throws exceptions (no-throw guarantee).
// Otherwise, the behavior is undefined.

	std::cout << ' ' << myvector[2] << std::endl;

// at
// Access element

// The function automatically checks whether n is within the bounds of valid elements
// in the container, throwing an out_of_range exception if it is not
// (i.e., if n is greater than, or equal to, its size).
// This is in contrast with member operator[], that does not check against bounds.

// Strong guarantee: if an exception is thrown, there are no changes in the container.
// It throws out_of_range if n is out of bounds.

	std::cout << ' ' << myvector.at(2) << std::endl;

// front
// Access first element

	std::vector<int> myvectortest;

	myvectortest.push_back(78);
	myvectortest.push_back(16);

	// now front equals 78, and back 16

	myvectortest.front() -= myvectortest.back();

	std::cout << "myvectortest.front() is now " << myvectortest.front() << '\n';

// back
// Access last element

	std::vector<int> myvectorback;

	myvectorback.push_back(10);

	while (myvectorback.back() != 0) {
		myvectorback.push_back(myvectorback.back() - 1);
	}

	std::cout << "myvectorback contains:";
	for (unsigned i = 0; i < myvectorback.size(); i++)
		std::cout << ' ' << myvectorback[i] << std::endl;

// data
// Access data

	std::vector<int> myvectorData(5);

	int *p = myvectorData.data();

	*p = 10;
	++p;
	*p = 20;
	p[2] = 100;

	std::cout << "myvectorData contains:";
	for (unsigned i = 0; i < myvectorData.size(); ++i)
		std::cout << ' ' << myvectorData[i] << std::endl;

// Modifiers:
// assign
// Assign vector content

	std::vector<int> first;
	std::vector<int> second;
	first.assign(7, 100);             // 7 ints with a value of 100

	std::cout << "first contains:\n";
	for (unsigned i = 0; i < first.size(); ++i)
		std::cout << ' ' << first[i] << std::endl;

	second.assign(first.begin(), first.end());

	std::cout << "Size of first: " << int(first.size()) << '\n';
	std::cout << "Size of second: " << int(second.size()) << '\n';

	std::cout << "second contains:\n";
	for (unsigned i = 0; i < second.size(); ++i)
		std::cout << ' ' << second[i] << std::endl;

// push_back
// Add element at the end

// Add element at the end
// Adds a new element at the end of the vector, after its current last element.
// The content of val is copied (or moved) to the new element.

// This effectively increases the container size by one, which causes an automatic
// reallocation of the allocated storage space if -and only if- the new vector size
// surpasses the current vector capacity.

	std::vector<int> myvectorpush_back;
	myvectorpush_back.push_back(10);
	myvectorpush_back.push_back(20);
	myvectorpush_back.push_back(30);
	myvectorpush_back.push_back(40);
	std::cout << "myvectorpush_back contains:\n";
	for (unsigned i = 0; i < myvectorpush_back.size(); ++i)
		std::cout << ' ' << myvectorpush_back[i] << std::endl;

// pop_back
// Delete last element

// Delete last element
// Removes the last element in the vector, effectively reducing the container size by one.

// This destroys the removed element.

	std::cout << "size before pop_back: " << myvectorpush_back.size() << "\n";

	for (unsigned i = 0; i < 4; i++) {
		std::cout << "size after pop_back: " << myvectorpush_back.size()
				<< "\n";
		myvectorpush_back.pop_back();
	}
	std::cout << "size after pop_back: " << myvectorpush_back.size() << "\n";

// insert
// Insert elements

// The vector is extended by inserting new elements before the element at the specified position,
// effectively increasing the container size by the number of elements inserted.
// This causes an automatic reallocation of the allocated storage space
// if -and only if- the new vector size surpasses the current vector capacity.
// Because vectors use an array as their underlying storage, inserting elements in positions
// other than the vector end causes the container to relocate all the elements
// that were after position to their new positions.
// This is generally an inefficient operation compared to the one performed
// for the same operation by other kinds of sequence containers (such as list or forward_list).

	std::vector<int> myvectorinsert(3, 100);
	for (unsigned i = 0; i < myvectorinsert.size(); i++) {
		std::cout << "before insert: " << myvectorinsert[i] << "\n";
	}
	int myarrayinsert[] = { 501, 502, 503 };
	myvectorinsert.insert(myvectorinsert.begin(), myarrayinsert,
			myarrayinsert + 3);

	for (unsigned i = 0; i < myvectorinsert.size(); i++) {
		std::cout << "after insert: " << myvectorinsert[i] << "\n";
	}

// erase
// Erase elements

// Removes from the vector either a single element (position) or a range of elements ([first,last)).
// This effectively reduces the container size by the number of elements removed, which are destroyed.

	std::vector<int> myvectorerase;
	// set some values (from 1 to 10)
	for (int i = 1; i <= 10; i++)
		myvectorerase.push_back(i);
	std::cout << "size before erase: " << myvectorerase.size() << "\n";
	// erase the 6th element
	myvectorerase.erase(myvectorerase.begin() + 5);
	std::cout << "size after erase 6 element: " << myvectorerase.size() << "\n";
	// erase the first 3 elements:
	myvectorerase.erase(myvectorerase.begin(), myvectorerase.begin() + 3);
	std::cout << "size after erase range: " << myvectorerase.size() << "\n";

// swap
// Swap content

// Exchanges the content of the container by the content of x, which is another
// vector object of the same type. Sizes may differ.
// After the call to this member function, the elements in this container are those
// which were in x before the call, and the elements of x are those which were in this.
// All iterators, references and pointers remain valid for the swapped objects.

	std::vector<int> swapOne(3, 100);   // three ints with a value of 100
	std::vector<int> swapTwo(5, 200);   // five ints with a value of 200

	swapOne.swap(swapTwo);

	for (unsigned i = 0; i < swapOne.size(); i++)
		std::cout << "swapOne contains:" << swapOne[i] << std::endl;
	for (unsigned i = 0; i < swapTwo.size(); i++)
		std::cout << "swapTwo contains:" << swapTwo[i] << std::endl;

// clear
// Clear content

	// Removes all elements from the vector (which are destroyed),
// leaving the container with a size of 0.
	std::vector<int> myvectorclear;
	myvectorclear.push_back(100);
	myvectorclear.push_back(200);
	myvectorclear.push_back(300);
	std::cout << "size before clear: " << myvectorclear.size() << "\n";
	myvectorclear.clear();
	std::cout << "size after clear: " << myvectorclear.size() << "\n";

// emplace
// Construct and insert element

// The container is extended by inserting a new element at position.
// This new element is constructed in place using args as the arguments for its construction.
// This effectively increases the container size by one.
// An automatic reallocation of the allocated storage space happens
// if -and only if- the new vector size surpasses the current vector capacity.

	std::vector<int> myvectoremplace = { 10, 20, 30 };
	std::cout << "myvectoremplace create:";
	for (unsigned i = 0; i < myvectoremplace.size(); i++)
		std::cout << myvectoremplace[i] << " ";
	std::cout << std::endl;

	auto it = myvectoremplace.emplace(myvectoremplace.begin() + 1, 100);
	std::cout << "myvectoremplace 100 place @ 1 pos:";
	for (unsigned i = 0; i < myvectoremplace.size(); i++)
		std::cout << myvectoremplace[i] << " ";
	std::cout << std::endl;

	myvectoremplace.emplace(it, 200);
	std::cout << "myvectoremplace 200 place @ it pos::";
	for (unsigned i = 0; i < myvectoremplace.size(); i++)
		std::cout << myvectoremplace[i] << " ";
	std::cout << std::endl;

	myvectoremplace.emplace(myvectoremplace.end(), 300);
	std::cout << "myvectoremplace 300 place @ end pos::";
	for (unsigned i = 0; i < myvectoremplace.size(); i++)
		std::cout << myvectoremplace[i] << " ";
	std::cout << std::endl;

// emplace_back
// Construct and insert element at the end

// Inserts a new element at the end of the vector, right after its current last element.
// This new element is constructed in place using args as the arguments for its constructor.

// This effectively increases the container size by one,
// which causes an automatic reallocation of the allocated storage space
// if- and only if- the new vector size surpasses the current vector capacity.

	std::vector<int> myvectoremplace_back = { 10, 20, 30 };

	std::cout << "before myvectoremplace_back:";
	for (unsigned i = 0; i < myvectoremplace_back.size(); i++)
		std::cout << myvectoremplace_back[i] << " ";
	std::cout << std::endl;

	myvectoremplace_back.emplace_back(100);
	myvectoremplace_back.emplace_back(200);

	std::cout << "after myvectoremplace_back:";
	for (unsigned i = 0; i < myvectoremplace_back.size(); i++)
		std::cout << myvectoremplace_back[i] << " ";
	std::cout << std::endl;

	return 0;
}

