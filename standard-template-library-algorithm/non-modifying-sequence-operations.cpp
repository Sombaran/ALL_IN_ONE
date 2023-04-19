// Non-modifying sequence operations:

// all_of example
#include <iostream>  // std::cout
#include <algorithm> // std::all_of
#include <utility>   // std::pair
#include <vector>    // std::vector
#include <cctype>    // std::tolower

#define all_of_ 1

int main()
{

#if all_of_

// Test condition on all elements in range (function template )

// first, last
//     Input iterators to the initial and final positions in a sequence. The range used is [first,last), which contains all the elements between first and last, including the element pointed by first but not the element pointed by last.

// pred
//     Unary function that accepts an element in the range as argument and returns a value convertible to bool. The value returned indicates whether the element fulfills the condition checked by this function.
//     The function shall not modify its argument.
//     This can either be a function pointer or a function object.

// Return value
//     true if pred returns true for all the elements in the range or if the range is empty, and false otherwise.

// all_of example
#include <iostream>  // std::cout
#include <algorithm> // std::all_of
#include <array>     // std::array

    std::array<int, 8> foo = {3, 5, 7, 11, 13, 17, 19, 23};

    if (std::all_of(foo.begin(), foo.end(), [](int i)
                    { return i % 2; }))
        std::cout << "All the elements are odd numbers.\n";

        // Output:
        //     All the elements are odd numbers.

        // Complexity
        //     Up to linear in the distance between first and last: Calls pred for each element until a mismatch is found.

        // Data races
        //     Some (or all) of the objects in the range [first,last) are accessed (once at most).

        // Exceptions
        //     Throws if either pred or an operation on an iterator throws.
        //     Note that invalid parameters cause undefined behavior.

#elif any_of_
    // Test if any element in range fulfills condition (function template )

first, last
    Input iterators to the initial and final positions in a sequence. The range used is [first,last), which contains all the elements between first and last, including the element pointed by first but not the element pointed by last.

pred
    Unary function that accepts an element in the range as argument and returns a value convertible to bool. The value returned indicates whether the element fulfills the condition checked by this function.
    The function shall not modify its argument.
    This can either be a function pointer or a function object.

Return value
    true if pred returns true for any of the elements in the range [first,last), and false otherwise.
    If [first,last) is an empty range, the function returns false.


  std::array<int,7> foo = {0,1,-1,3,-3,5,-5};

  if ( std::any_of(foo.begin(), foo.end(), [](int i){
        return i < 0;}) )
    std::cout << "There are negative elements in the range.\n";



Output:
    There are negative elements in the range.

Complexity
    Up to linear in the distance between first and last: Calls pred for each element until a match is found.

Data races
    Some (or all) of the objects in the range [first,last) are accessed (once at most).

Exceptions
    Throws if either pred or an operation on an iterator throws.
    Note that invalid parameters cause undefined behavior.

#elif none_of_
    // Test if no elements fulfill condition (function template )

Parameters

first, last
    Input iterators to the initial and final positions in a sequence. The range used is [first,last), which contains all the elements between first and last, including the element pointed by first but not the element pointed by last.

pred
    Unary function that accepts an element in the range as argument and returns a value convertible to bool. The value returned indicates whether the element fulfills the condition checked by this function.
    The function shall not modify its argument.
    This can either be a function pointer or a function object.

Return value
    true if pred returns false for all the elements in the range [first,last) or if the range is empty, and false otherwise.


  std::array<int,8> foo = {1,2,4,8,16,32,64,128};

  if ( std::none_of(foo.begin(), foo.end(), [](int i){
        return i < 0;}) )
    std::cout << "There are no negative elements in the range.\n";



Output:
There are no negative elements in the range.

Complexity
    Up to linear in the distance between first and last: Calls pred for each element until a match is found.

Data races
    Some (or all) of the objects in the range [first,last) are accessed (once at most).

Exceptions
    Throws if either pred or an operation on an iterator throws.
    Note that invalid parameters cause undefined behavior.

#elif for_each_
    // Apply function to range (function template )

Parameters

first, last
    Input iterators to the initial and final positions in a sequence. The range used is [first,last), which contains all the elements between first and last, including the element pointed by first but not the element pointed by last.

fn
    Unary function that accepts an element in the range as argument.
    This can either be a function pointer or a move constructible function object.
    Its return value, if any, is ignored.

void myfunction (int i) {  // function:
        std::cout << ' ' << i;
}

struct myclass {           // function object type:
        void operator()(int i) { std::cout << ' ' << i; }
} myobject;


  std::vector<int> myvector;
  myvector.push_back(10);
  myvector.push_back(20);
  myvector.push_back(30);

  std::cout << "myvector contains:";
  for_each (myvector.begin(), myvector.end(), myfunction);
  std::cout << '\n';

  // or:
  std::cout << "myvector contains:";
  for_each (myvector.begin(), myvector.end(), myobject);
  std::cout << '\n';



Output:
myvector contains: 10 20 30
myvector contains: 10 20 30

Complexity
Linear in the distance between first and last: Applies fn to each element.

Data races
The objects in the range [first,last) are accessed (each object is accessed exactly once).
These objects may be modified if InputIterator is a mutable iterator type and fn is not a constant function.

Exceptions
Throws if fn throws or if any of the operations on iterators throws.
Note that invalid arguments cause undefined behavior.

#elif find_
    // Find value in range (function template )

Find value in range
Returns an iterator to the first element in the range [first,last) that compares equal to val. If no such element is found, the function returns last.
The function uses operator== to compare the individual elements to val.


Parameters

first, last
    Input iterators to the initial and final positions in a sequence. The range searched is [first,last), which contains all the elements between first and last, including the element pointed by first but not the element pointed by last.
val
    Value to search for in the range.
    T shall be a type supporting comparisons with the elements pointed by InputIterator using operator== (with the elements as left-hand side operands, and val as right-hand side).

Return value
    An iterator to the first element in the range that compares equal to val.
    If no elements match, the function returns last.




  // using std::find with vector and iterator:
  std::vector<int> myvector { 10, 20, 30, 40 };
  std::vector<int>::iterator it;

  it = find (myvector.begin(), myvector.end(), 40);
  if (it != myvector.end())
    std::cout << "Element found in myvector: " << *it << '\n';
  else
    std::cout << "Element not found in myvector\n";



Output:
    Element found in myvector: 30

Complexity
    Up to linear in the distance between first and last: Compares elements until a match is found.

Data races
    Some (or all) of the objects in the range [first,last) are accessed (once at most).

Exceptions
    Throws if either an element comparison or an operation on an iterator throws.
    Note that invalid arguments cause undefined behavior.

#elif find_if_
    // Find element in range (function template )

Find element in range
    Returns an iterator to the first element in the range [first,last) for which pred returns true.
    If no such element is found, the function returns last.

Parameters

first, last
    Input iterators to the initial and final positions in a sequence. The range used is [first,last), which contains all the elements between first and last, including the element pointed by first but not the element pointed by last.

pred
    Unary function that accepts an element in the range as argument and returns a value convertible to bool.
    The value returned indicates whether the element is considered a match in the context of this function.
    The function shall not modify its argument.
    This can either be a function pointer or a function object.

Return value
    An iterator to the first element in the range for which pred does not return false.
    If pred is false for all elements, the function returns last.


bool IsOdd (int i) {
        return ((i % 2) == 1);
}


  std::vector<int> myvector;

  myvector.push_back(10);
  myvector.push_back(25);
  myvector.push_back(40);
  myvector.push_back(55);

  std::vector<int>::iterator it = std::find_if (myvector.begin(), myvector.end(), IsOdd);
  std::cout << "The first odd value is " << *it << '\n';



Output:
    The first odd value is 25

Complexity
    Up to linear in the distance between first and last: Calls pred for each element until a match is found.

Data races
    Some (or all) of the objects in the range [first,last) are accessed (once at most).

Exceptions
    Throws if either pred or an operation on an iterator throws.
    Note that invalid parameters cause undefined behavior.

#elif find_if_not_
    // Find element in range (negative condition) (function template )

Parameters

first, last
    Input iterators to the initial and final positions in a sequence. The range used is [first,last), which contains all the elements between first and last, including the element pointed by first but not the element pointed by last.

pred
    Unary function that accepts an element in the range as argument and returns a value convertible to bool. The value returned indicates whether the element is considered a match in the context of this function.
    The function shall not modify its argument.
    This can either be a function pointer or a function object.

Return value
    An iterator to the first element in the range for which pred returns false.
    If pred is true for all elements, the function returns last.

  std::array<int,5> foo = {1,2,3,4,5};

  std::array<int,5>::iterator it =
    std::find_if_not (foo.begin(), foo.end(), [](int i){
        return i % 2;} );
  std::cout << "The first even value is " << *it << '\n';



Output:
    The first even value is 2

Complexity
    Up to linear in the distance between first and last: Calls pred for each element until a mismatch is found.

Data races
    Some (or all) of the objects in the range [first,last) are accessed (once at most).

Exceptions
    Throws if either pred or an operation on an iterator throws.
    Note that invalid parameters cause undefined behavior.

#elif find_end_
    // Find last subsequence in range (function template )

Parameters

first1, last1
    Forward iterators to the initial and final positions of the searched sequence. The range used is [first1,last1), which contains all the elements between first1 and last1, including the element pointed by first1 but not the element pointed by last1.

first2, last2
    Forward iterators to the initial and final positions of the sequence to be searched for. The range used is [first2,last2).
    For (1), the elements in both ranges shall be of types comparable using operator== (with the elements of the first range as left-hand side operands, and those of the second as right-hand side operands).

pred
    Binary function that accepts two elements as arguments (one of each of the two sequences, in the same order), and returns a value convertible to bool. The returned value indicates whether the elements are considered to match in the context of this function.
    The function shall not modify any of its arguments.
    This can either be a function pointer or a function object.

Return value
    An iterator to the first element of the last occurrence of [first2,last2) in [first1,last1).
    If the sequence is not found, the function returns last1.

bool myfunction (int i, int j) {
        return (i == j);
}


  int myints[] = {1,2,3,4,5,1,2,3,4,5};
  std::vector<int> haystack (myints,myints+10);

  int needle1[] = {1,2,3};

  // using default comparison:
  std::vector<int>::iterator it;
  it = std::find_end (haystack.begin(), haystack.end(), needle1, needle1+3);

  if (it!=haystack.end())
    std::cout << "needle1 last found at position " << (it-haystack.begin()) << '\n';

  int needle2[] = {4,5,1};

  // using predicate comparison:
  it = std::find_end (haystack.begin(), haystack.end(), needle2, needle2+3, myfunction);

  if (it!=haystack.end())
    std::cout << "needle2 last found at position " << (it-haystack.begin()) << '\n';




Output:
    needle1 last found at position 5
    needle2 last found at position 3

Complexity
    Up to linear in count2*(1+count1-count2), where countX is the distance between firstX and lastX: Compares elements until the last matching subsequence is found.

Data races
    Some (or all) of the objects in both ranges are accessed (possibly more than once).

Exceptions
    Throws if any element comparison (or call to pred) throws or if any of the operations on iterators throws.
    Note that invalid arguments cause undefined behavior.

#elif find_first_of_
    // Find element from set in range (function template )

Parameters

first1, last1

    Input iterators to the initial and final positions of the searched sequence. The range used is [first1,last1), which contains all the elements between first1 and last1, including the element pointed by first1 but not the element pointed by last1.

first2, last2
    Forward iterators to the initial and final positions of the element values to be searched for. The range used is [first2,last2). For (1), the elements in both ranges shall be of types comparable using operator== (with the elements of the first range as left-hand side operands, and those of the second as right-hand side operands).

pred
    Binary function that accepts two elements as arguments (one of each of the two sequences, in the same order), and returns a value convertible to bool. The value returned indicates whether the elements are considered to match in the context of this function.
    The function shall not modify any of its arguments.
    This can either be a function pointer or a function object.

Return value
    An iterator to the first element in [first1,last1) that is part of [first2,last2).
    If no matches are found, the function returns last1.


bool comp_case_insensitive (char c1, char c2) {
        return (std::tolower(c1) == std::tolower(c2));
}


  int mychars[] = {'a','b','c','A','B','C'};
  std::vector<char> haystack (mychars,mychars+6);
  std::vector<char>::iterator it;

  int needle[] = {'A','B','C'};

  // using default comparison:
  it = find_first_of (haystack.begin(), haystack.end(), needle, needle+3);

  if (it!=haystack.end())
    std::cout << "The first match is: " << *it << '\n';

  // using predicate comparison:
  it = find_first_of (haystack.begin(), haystack.end(),
                      needle, needle+3, comp_case_insensitive);

  if (it!=haystack.end())
    std::cout << "The first match is: " << *it << '\n';




Output:
    The first match is: A
    The first match is: a

Complexity
    Up to linear in count1*count2 (where countX is the distance between firstX and lastX): Compares elements until a match is found.

Data races
    Some (or all) of the objects in both ranges are accessed (once at most in the case of [first1,last1), and possibly more than once in [first2,last2)).

Exceptions
    Throws if any element comparison (or pred) throws or if any of the operations on iterators throws.
    Note that invalid arguments cause undefined behavior.

#elif adjacent_find_
    // Find equal adjacent elements in range (function template )

Parameters

first, last
    Forward iterators to the initial and final positions of the searched sequence. The range used is [first,last), which contains all the elements between first and last, including the element pointed by first but not the element pointed by last.

pred
    Binary function that accepts two elements as arguments, and returns a value convertible to bool. The returned value indicates whether the elements are considered to match in the context of this function.
    The function shall not modify any of its arguments.
    This can either be a function pointer or a function object.

Return value
    An iterator to the first element of the first pair of matching consecutive elements in the range [first,last).
    If no such pair is found, the function returns last.


bool myfunction (int i, int j) {
        std::cout << "i=" << i << " j =" << j << endl;
        return (i == j);
}


  int myints[] = {5,20,5,30,30,20,10,10,20};
  std::vector<int> myvector (myints,myints+8);
  std::vector<int>::iterator it;

  // using default comparison:
  it = std::adjacent_find (myvector.begin(), myvector.end());

  if (it!=myvector.end())
    std::cout << "the pair of repeated elements are: " << *it << '\n';

  //using predicate comparison:
  it = std::adjacent_find (myvector.begin(), myvector.end(), myfunction);

  if (it!=myvector.end())
    std::cout << "the pair of repeated elements are: " << *it << '\n';




Output:
    the first pair of repeated elements are: 30
    the second pair of repeated elements are: 10

Complexity
    Up to linear in the distance between first and last: Compares elements until a match is found.

Data races
    Some (or all) of the objects in the range [first,last) are accessed (once at most).

Exceptions
    Throws if any element comparison (or pred) throws or if any of the operations on iterators throws.
    Note that invalid arguments cause undefined behavior.

#elif count_
    // Count appearances of value in range (function template )

Parameters

first, last
    Input iterators to the initial and final positions of the sequence of elements. The range used is [first,last), which contains all the elements between first and last, including the element pointed by first but not the element pointed by last.

val
    Value to match.
    T shall be a type supporting comparisons with the elements pointed by InputIterator using operator== (with the elements as left-hand side operands, and val as right-hand side).

Return value
    The number of elements in the range [first,last) that compare equal to val.
    The return type (iterator_traits<InputIterator>::difference_type) is a signed integral type.



  // counting elements in array:
  int myints[] = {10,20,30,30,20,10,10,20};   // 8 elements
  int mycount = std::count (myints, myints+8, 10);
  std::cout << "10 appears " << mycount << " times.\n";

  // counting elements in container:
  std::vector<int> myvector (myints, myints+8);
  mycount = std::count (myvector.begin(), myvector.end(), 20);
  std::cout << "20 appears " << mycount  << " times.\n";





Output:
    10 appears 3 times.
    20 appears 3 times.

Complexity
    Linear in the distance between first and last: Compares once each element.

Data races
    The objects in the range [first,last) are accessed (each object is accessed exactly once).

Exceptions
    Throws if either an element comparison or an operation on an iterator throws.
    Note that invalid arguments cause undefined behavior.

#elif count_if_
    // Return number of elements in range satisfying condition (function template )

Parameters

first, last
    Input iterators to the initial and final positions of the sequence of elements. The range used is [first,last), which contains all the elements between first and last, including the element pointed by first but not the element pointed by last.

pred
    Unary function that accepts an element in the range as argument, and returns a value convertible to bool. The value returned indicates whether the element is counted by this function.
    The function shall not modify its argument.
    This can either be a function pointer or a function object.

Return value
    The number of elements in the range [first,last) for which pred does not return false.
    The return type (iterator_traits<InputIterator>::difference_type) is a signed integral type.

    It count the number of times pred return true from frist to end


bool IsOdd (int i) {
        return ((i % 2) == 1); }


  std::vector<int> myvector;
  for (int i=1; i<10; i++) myvector.push_back(i); // myvector: 1 2 3 4 5 6 7 8 9

  int mycount = count_if (myvector.begin(), myvector.end(), IsOdd);
  std::cout << "myvector contains " << mycount  << " odd values.\n";



Output:
    myvector contains 5 odd values.

Complexity
    Linear in the distance between first and last: Calls pred once for each element.

Data races
    The objects in the range [first,last) are accessed (each object is accessed exactly once).

Exceptions
    Throws if pred throws or if any of the operations on iterators throws.
    Note that invalid arguments cause undefined behavior.

#elif mismatch_
    // Return first position where two ranges differ (function template )

Parameters

first1, last1
    Input iterators to the initial and final positions of the first sequence. The range used is [first1,last1), which contains all the elements between first1 and last1, including the element pointed by first1 but not the element pointed by last1.

first2
    Input iterator to the initial position of the second sequence. Up to as many elements as in the range [first1,last1) can be accessed by the function.

pred
    Binary function that accepts two elements as argument (one of each of the two sequences, in the same order), and returns a value convertible to bool. The value returned indicates whether the elements are considered to match in the context of this function.
    The function shall not modify any of its arguments.
    This can either be a function pointer or a function object.

Return value
    A pair, where its members first and second point to the first element in both sequences that did not compare equal to each other.
    If the elements compared in both sequences have all matched, the function returns a pair with first set to last1 and second set to the element in that same relative position in the second sequence.
    If none matched, it returns make_pair(first1,first2).


bool mypredicate (int i, int j) {
        return (i == j);
}


  std::vector<int> myvector;
  for (int i=1; i<6; i++) myvector.push_back (i*10); // myvector: 10 20 30 40 50

  int myints[] = {10,20,80,320,1024};                //   myints: 10 20 80 320 1024

  std::pair<std::vector<int>::iterator,int*> mypair;

  // using default comparison:
  mypair = std::mismatch (myvector.begin(), myvector.end(), myints);
  std::cout << "First mismatching elements: " << *mypair.first;
  std::cout << " and " << *mypair.second << '\n';

  // using predicate comparison:
  mypair = std::mismatch (myvector.begin() , myvector.end(), myints, mypredicate);
  std::cout << "Second mismatching elements: " << *mypair.first;
  std::cout << " and " << *mypair.second << '\n';




Output:
First mismatching elements: 30 and 80
Second mismatching elements: 40 and 320

Complexity
Up to linear in the distance between first1 and last1: Compares elements until a mismatch is found.

Data races
Some (or all) of the objects in both ranges are accessed (once at most).

Exceptions
Throws if any element comparison (or pred) throws or if any of the operations on iterators throws.
Note that invalid parameters cause undefined behavior.

#elif equal_
    // Test whether the elements in two ranges are equal (function template )

Parameters
first1, last1
    Input iterators to the initial and final positions of the first sequence. The range used is [first1,last1), which contains all the elements between first1 and last1, including the element pointed by first1 but not the element pointed by last1.

first2
    Input iterator to the initial position of the second sequence. The comparison includes up to as many elements of this sequence as those in the range [first1,last1).

pred
    Binary function that accepts two elements as argument (one of each of the two sequences, in the same order), and returns a value convertible to bool. The value returned indicates whether the elements are considered to match in the context of this function.
    The function shall not modify any of its arguments.
    This can either be a function pointer or a function object.

Return value
    true if all the elements in the range [first1,last1) compare equal to those of the range starting at first2, and false otherwise.


bool mypredicate (int i, int j) {
        return (i == j);
}


  int myints[] = {20,40,60,80,100};               //   myints: 20 40 60 80 100
  std::vector<int>myvector (myints,myints+5);     // myvector: 20 40 60 80 100

  // using default comparison:
  if ( std::equal (myvector.begin(), myvector.end(), myints) )
    std::cout << "The contents of both sequences are equal.\n";
  else
    std::cout << "The contents of both sequences differ.\n";

  myvector[3]=81;                                 // myvector: 20 40 60 81 100

  // using predicate comparison:
  if ( std::equal (myvector.begin(), myvector.end(), myints, mypredicate) )
    std::cout << "The contents of both sequences are equal.\n";
  else
    std::cout << "The contents of both sequences differ.\n";





Output:
    The contents of both sequences are equal.
    The contents of both sequence differ.

Complexity
    Up to linear in the distance between first1 and last1: Compares elements until a mismatch is found.

Data races
    Some (or all) of the objects in both ranges are accessed (once at most).

Exceptions
    Throws if any of the element comparisons (or pred) throws, or if any of the operations on iterators throws.
    Note that invalid parameters cause undefined behavior.

#elif is_permutation_
    // Test whether range is permutation of another (function template )

Parameters

first1, last1
    Input iterators to the initial and final positions of the first sequence. The range used is [first1,last1), which contains all the elements between first1 and last1, including the element pointed by first1 but not the element pointed by last1.

first2
    Input iterator to the initial position of the second sequence.
    The function considers as many elements of this sequence as those in the range [first1,last1).
    If this sequence is shorter, it causes undefined behavior.

pred
    Binary function that accepts two elements as argument (one of each of the two sequences, in the same order), and returns a value convertible to bool. The value returned indicates whether the elements are considered to match in the context of this function.
    The function shall not modify any of its arguments.
    This can either be a function pointer or a function object.

    InputIterator1 and InputIterator2 shall point to the same type.

Return value
    true if all the elements in the range [first1,last1) compare equal to those of the range starting at first2 in any order, and false otherwise.


bool mypredicate (int i, int j) {
        //   std::cout << i << " " << j << std::endl;
        return (i == j);
}

  std::array<int,3> foo = {5,2,1};
  std::array<int,3> bar = {2,1,5};

  if ( std::is_permutation (foo.begin(), foo.end(), bar.begin()) )
    std::cout << "foo and bar contain the same elements.\n";
  else
    std::cout << "foo and bar contain the not same elements.\n";
 // using predicate comparison:
  if ( std::is_permutation (foo.begin(), foo.end(), bar.begin(), mypredicate) )
    std::cout << "foo and bar contain the same elements.\n";
  else
    std::cout << "foo and bar contain the not same elements.\n";



Output:
    foo and bar contain the same elements.
    foo and bar contain the same elements.

Complexity
    If both sequence are equal (with the elements in the same order), linear in the distance between first1 and last1.
    Otherwise, up to quadratic: Performs at most N2 element comparisons until the result is determined (where N is the distance between first1 and last1).

Data races
    Some (or all) of the objects in both ranges are accessed (possibly multiple times each).

Exceptions
    Throws if any of the element comparisons (or pred) throws, or if any of the operations on iterators throws.
    Note that invalid parameters cause undefined behavior.

#elif search_
    // Search range for subsequence (function template )

Parameters

first1, last1
    Forward iterators to the initial and final positions of the searched sequence. The range used is [first1,last1), which contains all the elements between first1 and last1, including the element pointed by first1 but not the element pointed by last1.

first2, last2
    Forward iterators to the initial and final positions of the sequence to be searched for. The range used is [first2,last2).
    For (1), the elements in both ranges shall be of types comparable using operator== (with the elements of the first range as left-hand side operands, and those of the second as right-hand side operands).

pred
    Binary function that accepts two elements as arguments (one of each of the two sequences, in the same order), and returns a value convertible to bool. The returned value indicates whether the elements are considered to match in the context of this function.
    The function shall not modify any of its arguments.
    This can either be a function pointer or a function object.


bool mypredicate (int i, int j) {
        return (i == j);
}


  std::vector<int> haystack;

  // set some values:        haystack: 10 20 30 40 50 60 70 80 90
  for (int i=1; i<10; i++) haystack.push_back(i*10);

  // using default comparison:
  int needle1[] = {40,50,60,70};
  std::vector<int>::iterator it;
  it = std::search (haystack.begin(), haystack.end(), needle1, needle1+4);

  if (it!=haystack.end())
    std::cout << "needle1 found at position " << (it-haystack.begin()) << '\n';
  else
    std::cout << "needle1 not found\n";

  // using predicate comparison:
  int needle2[] = {20,30,50};
  it = std::search (haystack.begin(), haystack.end(), needle2, needle2+3, mypredicate);

  if (it!=haystack.end())
    std::cout << "needle2 found at position " << (it-haystack.begin()) << '\n';
  else
    std::cout << "needle2 not found\n";





Output:
    needle1 found at position 3
    needle2 not found

Complexity
    Up to linear in count1*count2 (where countX is the distance between firstX and lastX): Compares elements until a matching subsequence is found.

Data races
    Some (or all) of the objects in both ranges are accessed (possibly more than once).

Exceptions
    Throws if any of the element comparisons (or pred) throws or if any of the operations on iterators throws.
    Note that invalid arguments cause undefined behavior.

#elif search_n_
    // Search range for elements (function template )

Parameters

first, last
    Forward iterators to the initial and final positions of the searched sequence. The range used is [first,last), which contains all the elements between first and last, including the element pointed by first but not the element pointed by last.

count
    Minimum number of successive elements to match.
    Size shall be (convertible to) an integral type.

val
    Individual value to be compared, or to be used as argument for pred (in the second version).
    for the first version, T shall be a type supporting comparisons with the elements pointed by InputIterator using operator== (with the elements as left-hand size operands, and val as right-hand side).

pred
    Binary function that accepts two arguments (one element from the sequence as first, and val as second), and returns a value convertible to bool. The value returned indicates whether the element is considered a match in the context of this function.
    The function shall not modify any of its arguments.
    This can either be a function pointer or a function object.

Return value
    An iterator to the first element of the sequence.
    If no such sequence is found, the function returns last.

bool mypredicate (int i, int j) {
        return (i == j);
}


  int myints[]={10,20,30,30,20,10,10,20};
  std::vector<int> myvector (myints,myints+8);

  std::vector<int>::iterator it;

  // using default comparison:
  it = std::search_n (myvector.begin(), myvector.end(), 2, 30);

  if (it!=myvector.end())
    std::cout << "two 30s found at position " << (it-myvector.begin()) << '\n';
  else
    std::cout << "match not found\n";

  // using predicate comparison:
  it = std::search_n (myvector.begin(), myvector.end(), 2, 10, mypredicate);

  if (it!=myvector.end())
    std::cout << "two 10s found at position " << int(it-myvector.begin()) << '\n';
  else
    std::cout << "match not found\n";



Output:
    Two 30s found at position 2
    Two 10s found at position 5
Means : Looking for the number of times conjugative number presnet in the array(vector)
Complexity
    Up to linear in the distance between first and last: Compares elements until a matching subsequence is found.

Data races
    Some (or all) of the objects in the range [first,last) are accessed (once at most).

Exceptions
    Throws if any of the element comparisons (or pred) throws or if any of the operations on iterators throws.
    Note that invalid parameters cause undefined behavior.

#else
//  <code statements>
#endif

    return 0;
}
