//Non-modifying sequence operations:

// all_of example
#include <iostream>     // std::cout
#include <algorithm>    // std::all_of
#include <utility>      // std::pair
#include <vector>       // std::vector
#include <cctype>       // std::tolower

#define all_of_ 1

int main(){


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
#include <iostream>     // std::cout
#include <algorithm>    // std::all_of
#include <array>        // std::array


  std::array<int,8> foo = {3,5,7,11,13,17,19,23};

  if ( std::all_of(foo.begin(), foo.end(), [](int i){return i%2;}) )
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
//Test if any element in range fulfills condition (function template )

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

  if ( std::any_of(foo.begin(), foo.end(), [](int i){return i<0;}) )
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
//Test if no elements fulfill condition (function template )

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

  if ( std::none_of(foo.begin(), foo.end(), [](int i){return i<0;}) )
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
//Apply function to range (function template )


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
  void operator() (int i) {std::cout << ' ' << i;}
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
//Find value in range (function template )

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
//Find element in range (function template )

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
  return ((i%2)==1);
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
//Find element in range (negative condition) (function template )

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
    std::find_if_not (foo.begin(), foo.end(), [](int i){return i%2;} );
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
//Find last subsequence in range (function template )

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
  return (i==j);
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
//Find element from set in range (function template )

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
  return (std::tolower(c1)==std::tolower(c2));
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
//Find equal adjacent elements in range (function template )

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
  std::cout << "i=" << i << " j =" << j <<endl ;
  return (i==j);
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
//Count appearances of value in range (function template )

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
//Return number of elements in range satisfying condition (function template )

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


bool IsOdd (int i) { return ((i%2)==1); }


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
//Return first position where two ranges differ (function template )

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
  return (i==j);
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
//Test whether the elements in two ranges are equal (function template )

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
  return (i==j);
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
//Test whether range is permutation of another (function template )

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
  return (i==j);
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
//Search range for subsequence (function template )

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
  return (i==j);
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
//Search range for elements (function template )

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
  return (i==j);
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


Modifying sequence operations:
copy
Copy range of elements (function template )

        std::copy ( myints, myints+7, myvector.begin() );


// copy algorithm example
#include <iostream>     // std::cout
#include <algorithm>    // std::copy
#include <vector>       // std::vector

void myfunction(int i){
    std::cout << i << std::endl;
}
int main () {
    std::vector<int> myints {10,20,30,40,50,60,70};
  std::vector<int> myvector;// (7);
  myvector.reserve(70);
    myvector.resize(7);
  std::copy ( myints.begin(), myints.end(), myvector.begin() );

  std::cout << "myvector contains:"<< std::endl;
  std::cout << "myvector capacity:"<< myvector.capacity() << std::endl;
  for_each (myvector.begin(), myvector.end(), myfunction); 

  return 0;
}


copy_n 
Copy elements (function template )


// copy algorithm example
#include <iostream>     // std::cout
#include <algorithm>    // std::copy
#include <vector>       // std::vector

void myfunction(int i){
    std::cout << i << std::endl;
}
int main () {
    std::vector<int> myints {10,20,30,40,50,60,70};
  std::vector<int> myvector;// (7);
  myvector.reserve(70);
    myvector.resize(7);
  std::copy_n ( myints.begin(), 5, myvector.begin() );

  std::cout << "myvector contains:"<< std::endl;
  std::cout << "myvector capacity:"<< myvector.capacity() << std::endl;
  for_each (myvector.begin(), myvector.end(), myfunction); 

  return 0;
}

copy_if 
Copy certain elements of range (function template )

Add if the pred is true

    std::copy_if (foo.begin(), foo.end(), bar.begin(), [](int i){return !(i<0);} );



copy_backward
Copy range of elements backward (function template )


  std::copy_backward ( myvector.begin(), myvector.begin()+5, myvector.end() );


move 
Move range of elements (function template )


// move_backward example
#include <iostream>     // std::cout
#include <algorithm>    // std::move_backward
#include <string>       // std::string

int main () {
  std::string elems[10] = {"air","water","fire","earth"};
  std::string elems1[10] = {"air","water","fire","earth"};

  // insert new element at the beginning:
  std::move (elems,elems+3,elems1);
  elems[0]="ether";

  std::cout << "elems contains:";
  for (int i=0; i<10; ++i)
    std::cout << " [" << elems[i] << "]";
  std::cout << '\n';

  std::cout << "elems1 contains:";
  for (int i=0; i<10; ++i)
    std::cout << " [" << elems1[i] << "]";
  std::cout << '\n';
  
  return 0;
}



move_backward 
Move range of elements backward (function template )

// move_backward example
#include <iostream>     // std::cout
#include <algorithm>    // std::move_backward
#include <string>       // std::string

int main () {
  std::string elems[10] = {"air","water","fire","earth"};

  // insert new element at the beginning:
  std::move_backward (elems,elems+3,elems+10);
  elems[0]="ether";

  std::cout << "elems contains:";
  for (int i=0; i<10; ++i)
    std::cout << " [" << elems[i] << "]";
  std::cout << '\n';

  return 0;
}

swap
Exchange values of two objects (function template )

// swap algorithm example (C++98)
#include <iostream>     // std::cout
#include <algorithm>    // std::swap
#include <vector>       // std::vector

int main () {

  int x=10, y=20;                              // x:10 y:20
  std::swap(x,y);                              // x:20 y:10

  std::vector<int> foo (4,x), bar (6,y);       // foo:4x20 bar:6x10
  std::swap(foo,bar);                          // foo:6x10 bar:4x20

  std::cout << "foo contains:";
  for (std::vector<int>::iterator it=foo.begin(); it!=foo.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}

swap_ranges
Exchange values of two ranges (function template )


// swap algorithm example (C++98)
#include <iostream>     // std::cout
#include <algorithm>    // std::swap
#include <vector>       // std::vector

int main () {

  std::vector<int> foo {10, 30, 50, 70, 90, 100};
  std::vector<int> bar {20, 40, 60, 80, 0};
  std::swap_ranges(foo.begin(),foo.end(), bar.begin() );                          

  std::cout << "foo contains:";
  for (std::vector<int>::iterator it=foo.begin(); it!=foo.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  std::cout << "bar contains:";
  for (std::vector<int>::iterator it=bar.begin(); it!=bar.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}


iter_swap
Exchange values of objects pointed to by two iterators (function template )

// iter_swap example
#include <iostream>     // std::cout
#include <algorithm>    // std::iter_swap
#include <vector>       // std::vector

int main () {

  int myints[]={10,20,30,40,50 };              //   myints:  10  20  30  40  50
  std::vector<int> myvector (4,99);            // myvector:  99  99  99  99

 std::cout << "myvector contains:";
  for (std::vector<int>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';


  std::iter_swap(myints,myvector.begin());     //   myints: [99] 20  30  40  50
                                               // myvector: [10] 99  99  99

 std::cout << "myvector contains:";
  for (std::vector<int>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';


  std::iter_swap(myints+3,myvector.begin()+2); //   myints:  99  20  30 [99] 50
                                               // myvector:  10  99 [40] 99

  std::cout << "myvector contains:";
  for (std::vector<int>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}


transform
Transform range (function template )

// transform algorithm example
#include <iostream>     // std::cout
#include <algorithm>    // std::transform
#include <vector>       // std::vector
#include <functional>   // std::plus

int op_increase (int i) { return 1+i; }

int main () {
  std::vector<int> foo;
  std::vector<int> bar;

  // set some values:
  for (int i=1; i<6; i++)
    foo.push_back (i*10);                         // foo: 10 20 30 40 50

  std::cout << "foo contains:";
  for (std::vector<int>::iterator it=foo.begin(); it!=foo.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';
  
  bar.resize(foo.size());                         // allocate space

  std::transform (foo.begin(), foo.end(), bar.begin(), op_increase);
                                                  // bar: 11 21 31 41 51

  std::cout << "bar contains:";
  for (std::vector<int>::iterator it=bar.begin(); it!=bar.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';
  
  // std::plus adds together its two arguments:
  std::transform (bar.begin(), bar.end(), bar.begin(), op_increase);
                                                  // foo: 12 22 32 42 52

  std::cout << "bar contains:";
  for (std::vector<int>::iterator it=bar.begin(); it!=bar.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';


// std::plus adds together its two arguments:
  std::transform (foo.begin(), foo.end(), bar.begin(), foo.begin(), std::plus<int>());
                                                  // foo: 22 42 62 82 102

  std::cout << "foo contains:";
  for (std::vector<int>::iterator it=foo.begin(); it!=foo.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';
  
  return 0;
}

replace
Replace value in range (function template )


// replace algorithm example
#include <iostream>     // std::cout
#include <algorithm>    // std::replace
#include <vector>       // std::vector

int main () {
  int myints[] = { 10, 20, 30, 30, 20, 10, 10, 20 };
  std::vector<int> myvector (myints, myints+8);            // 10 20 30 30 20 10 10 20

  std::replace (myvector.begin(), myvector.end(), 20, 99); // 10 99 30 30 99 10 10 99

  std::cout << "myvector contains:";
  for (std::vector<int>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}

replace_if // Modifies same vector
Replace values in range (function template )



// replace algorithm example
#include <iostream>     // std::cout
#include <algorithm>    // std::replace
#include <vector>       // std::vector

bool IsOdd (int i) { std::cout << "((i%2)==1) :" << ((i%2)==1) << std::endl; return ((i%2)==1); }

// If  IsOdd return true then it will update the value 
int main () {
  int myints[] = { 10, 99, 30, 30, 99, 55, 10, 99};
  std::vector<int> myvector (myints, myints+8);            // 10 99 30 30 99 55 10 99

  std::replace_if (myvector.begin(), myvector.end(), IsOdd, 20);  // 10 20 30 30 20 20 10 20
 
  std::cout << "myvector contains:";
  for (std::vector<int>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}


replace_copy // Modifies other vector
Copy range replacing value (function template )

// replace_copy example
#include <iostream>     // std::cout
#include <algorithm>    // std::replace_copy
#include <vector>       // std::vector

int main () {
  int myints[] = { 10, 20, 30, 30, 20, 10, 10, 20 };

  std::vector<int> myvector (8);
  std::replace_copy (myints, myints+8, myvector.begin(), 20, 99);

  std::cout << "myvector contains:";
  for (std::vector<int>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}

replace_copy_if
Copy range replacing value (function template )



// replace_copy example
#include <iostream>     // std::cout
#include <algorithm>    // std::replace_copy
#include <vector>       // std::vector

bool IsOdd (int i) { std::cout << "((i%2)==1) :" << ((i%2)==1) << std::endl; return ((i%2)==1); }

int main () {
  int myints[] = { 10, 20, 30, 30, 20, 10, 10, 20 };

  std::vector<int> myvector (8);
  std::replace_copy_if (myints, myints+8, myvector.begin(), IsOdd, 99);

  std::cout << "myvector contains:";
  for (std::vector<int>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}


fill
Fill range with value (function template )

// fill algorithm example
#include <iostream>     // std::cout
#include <algorithm>    // std::fill
#include <vector>       // std::vector

int main () {
  std::vector<int> myvector (8);                       // myvector: 0 0 0 0 0 0 0 0

  std::fill (myvector.begin(),myvector.begin()+4,5);   // myvector: 5 5 5 5 0 0 0 0
  std::fill (myvector.begin()+3,myvector.end()-2,8);   // myvector: 5 5 5 8 8 8 0 0

  std::cout << "myvector contains:";
  for (std::vector<int>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}

fill_n
Fill sequence with value (function template )

// fill_n example
#include <iostream>     // std::cout
#include <algorithm>    // std::fill_n
#include <vector>       // std::vector

int main () {
  std::vector<int> myvector (8,10);        // myvector: 10 10 10 10 10 10 10 10

  std::fill_n (myvector.begin(),4,20);     // myvector: 20 20 20 20 10 10 10 10
  std::fill_n (myvector.begin()+3,3,33);   // myvector: 20 20 20 33 33 33 10 10

  std::cout << "myvector contains:";
  for (std::vector<int>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}


generate
Generate values for range with function (function template )

// generate algorithm example
#include <iostream>     // std::cout
#include <algorithm>    // std::generate
#include <vector>       // std::vector
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand

// function generator:
int RandomNumber () { return (std::rand()%100); }

// class generator:
struct c_unique {
  int current;
  c_unique() {current=0;}
  int operator()() {return ++current;}
} UniqueNumber;

int main () {
  std::srand ( unsigned ( std::time(0) ) );

  std::vector<int> myvector (8);

  std::generate (myvector.begin(), myvector.end(), RandomNumber);

  std::cout << "myvector contains:";
  for (std::vector<int>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  std::generate (myvector.begin(), myvector.end(), UniqueNumber);

  std::cout << "myvector contains:";
  for (std::vector<int>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';
 
  return 0;
}

generate_n
Generate values for sequence with function (function template )

// generate_n example
#include <iostream>     // std::cout
#include <algorithm>    // std::generate_n

int current = 0;
int UniqueNumber () { return ++current; }

int main () {
  int myarray[9];

  std::generate_n (myarray, 9, UniqueNumber);

  std::cout << "myarray contains:";
  for (int i=0; i<9; ++i)
    std::cout << ' ' << myarray[i];
  std::cout << '\n';

  return 0;
}


remove
Remove value from range (function template )

// remove algorithm example
#include <iostream>     // std::cout
#include <algorithm>    // std::remove

int main () {
  int myints[] = {10,20,30,30,20,10,10,20};      // 10 20 30 30 20 10 10 20

  // bounds of range:
  int* pbegin = myints;                          // ^
  int* pend = myints+sizeof(myints)/sizeof(int); //                        ^

  pend = std::remove (pbegin, pend, 20);         // 10 30 30 10 10 ?  ?  ?
                                                 // ^              ^
  std::cout << "range contains:";
  for (int* p=pbegin; p!=pend; ++p)
    std::cout << ' ' << *p;
  std::cout << '\n';

  return 0;
}

remove_if
Remove elements from range (function template )
// remove_if example
#include <iostream>     // std::cout
#include <algorithm>    // std::remove_if

bool IsOdd (int i) { return ((i%2)==1); }

//if IsOdd return true then remove the value from array

int main () {
  int myints[] = {1,2,3,4,5,6,7,8,9};            // 1 2 3 4 5 6 7 8 9

  // bounds of range:
  int* pbegin = myints;                          // ^
  int* pend = myints+sizeof(myints)/sizeof(int); // ^                 ^

  pend = std::remove_if (pbegin, pend, IsOdd);   // 2 4 6 8 ? ? ? ? ?
                                                 // ^       ^
  std::cout << "the range contains:";
  for (int* p=pbegin; p!=pend; ++p)
    std::cout << ' ' << *p;
  std::cout << '\n';

  return 0;
}

remove_copy
Copy range removing value (function template )

// remove_copy example
#include <iostream>     // std::cout
#include <algorithm>    // std::remove_copy
#include <vector>       // std::vector

int main () {
  int myints[] = {10,20,30,30,20,10,10,20};               // 10 20 30 30 20 10 10 20
  std::vector<int> myvector (8);

  std::remove_copy (myints,myints+8,myvector.begin(),20); // 10 30 30 10 10 0 0 0

  std::cout << "myvector contains:";
  for (std::vector<int>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}

remove_copy_if
Copy range removing values (function template )


// remove_copy example
#include <iostream>     // std::cout
#include <algorithm>    // std::remove_copy
#include <vector>       // std::vector

bool IsOdd (int i) { std::cout << "((i%2)==1) :" << ((i%2)==1) << std::endl; return ((i%2)==1); }

int main () {
  int myints[] = {1,2,3,4,5,6,7,8};               // 1 2 3 4 5 6 7 8 
  std::vector<int> myvector (8);

  std::remove_copy_if (myints,myints+8,myvector.begin(), IsOdd); // 2 4 6 8 0 0 0 0

  std::cout << "myvector contains:";
  for (std::vector<int>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}

unique
Remove consecutive duplicates in range (function template )

// unique algorithm example
#include <iostream>     // std::cout
#include <algorithm>    // std::unique, std::distance
#include <vector>       // std::vector

bool myfunction (int i, int j) {
  return (i==j);
}

int main () {
  int myints[] = {10,20,20,20,30,30,20,20,10};           // 10 20 20 20 30 30 20 20 10
  std::vector<int> myvector (myints,myints+9);

  // using default comparison:
  std::vector<int>::iterator it;
  it = std::unique (myvector.begin(), myvector.end());   // 10 20 30 20 10 ?  ?  ?  ?
                                                         //                ^

  myvector.resize( std::distance(myvector.begin(),it) ); // 10 20 30 20 10

  // using predicate comparison:
  std::unique (myvector.begin(), myvector.end(), myfunction);   // (no changes)

  // print out content:
  std::cout << "myvector contains:";
  for (it=myvector.begin(); it!=myvector.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}

unique_copy
Copy range removing duplicates (function template )

// unique_copy example
#include <iostream>     // std::cout
#include <algorithm>    // std::unique_copy, std::sort, std::distance
#include <vector>       // std::vector

bool myfunction (int i, int j) {
  return (i==j);
}

int main () {
  int myints[] = {10,20,20,20,30,30,20,20,10};
  std::vector<int> myvector (9);                            // 0  0  0  0  0  0  0  0  0

  // using default comparison:
  std::vector<int>::iterator it;
  it=std::unique_copy (myints,myints+9,myvector.begin());   // 10 20 30 20 10 0  0  0  0
                                                            //                ^

  // print out content:
  std::cout << "myvector contains:";
  for (it=myvector.begin(); it!=myvector.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';
  
//   std::sort (myvector.begin(),myvector.end(), std::greater<>());                          // 10 10 20 20 30 0  0  0  0
  std::sort(std::begin(myvector), std::end(myvector), std::greater<int>());                                          //                ^

  // print out content:
  std::cout << "myvector contains:";
  for (it=myvector.begin(); it!=myvector.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';
  
  it=std::unique_copy (myints,myints+9,myvector.begin());

  // print out content:
  std::cout << "myvector contains:";
  for (it=myvector.begin(); it!=myvector.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';
  
    std::sort(myvector.begin(), myvector.end(), std::greater<int>()); //                ^
   it=std::unique (myvector.begin(), myvector.end());
  myvector.resize( std::distance(myvector.begin(),it) );    // 10 20 30

  // print out content:
  std::cout << "myvector contains:";
  for (it=myvector.begin(); it!=myvector.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}

reverse
Reverse range (function template )

// reverse algorithm example
#include <iostream>     // std::cout
#include <algorithm>    // std::reverse
#include <vector>       // std::vector

int main () {
  std::vector<int> myvector;

  // set some values:
  for (int i=1; i<10; ++i) myvector.push_back(i);   // 1 2 3 4 5 6 7 8 9

  std::reverse(myvector.begin(),myvector.end());    // 9 8 7 6 5 4 3 2 1

  // print out content:
  std::cout << "myvector contains:";
  for (std::vector<int>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}

reverse_copy
Copy range reversed (function template )

// reverse algorithm example
#include <iostream>     // std::cout
#include <algorithm>    // std::reverse
#include <vector>       // std::vector

int main () {
    
  int myints[] ={1,2,3,4,5,6,7,8,9};
  std::vector<int> myvector (9);
  
  std::reverse_copy(myints, myints+9, myvector.begin());    // 9 8 7 6 5 4 3 2 1

  // print out content:
  std::cout << "myvector contains:";
  for (std::vector<int>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}

rotate
Rotate left the elements in range (function template )

// rotate algorithm example
#include <iostream>     // std::cout
#include <algorithm>    // std::rotate
#include <vector>       // std::vector

int main () {
  std::vector<int> myvector;

  // set some values:
  for (int i=1; i<10; ++i) myvector.push_back(i); // 1 2 3 4 5 6 7 8 9

  std::rotate(myvector.begin(),myvector.begin()+3,myvector.end());
                                                  // 4 5 6 7 8 9 1 2 3
  // print out content:
  std::cout << "myvector contains:";
  for (std::vector<int>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}

rotate_copy
Copy range rotated left (function template )


// rotate algorithm example
#include <iostream>     // std::cout
#include <algorithm>    // std::rotate
#include <vector>       // std::vector

int main () {
  int myints[] ={1,2,3,4,5,6,7};
  std::vector<int> myvector (7, 9);

  std::cout << "myvector contains:";
  for (std::vector<int>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';
  
  std::rotate_copy(myints, myints+3, myints+7, myvector.begin());
                                                  // 4 5 6 1 2 3
  // print out content:
  std::cout << "myvector contains:";
  for (std::vector<int>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}



random_shuffle
Randomly rearrange elements in range (function template )

// random_shuffle example
#include <iostream>     // std::cout
#include <algorithm>    // std::random_shuffle
#include <vector>       // std::vector
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand

// random generator function:
int myrandom (int i) { return std::rand()%i;}

int main () {
  std::srand ( unsigned ( std::time(0) ) );
  std::vector<int> myvector;

  // set some values:
  for (int i=1; i<10; ++i) myvector.push_back(i); // 1 2 3 4 5 6 7 8 9

  // using built-in random generator:
  std::random_shuffle ( myvector.begin(), myvector.end() );

  // using myrandom:
  std::random_shuffle ( myvector.begin(), myvector.end(), myrandom);

  // print out content:
  std::cout << "myvector contains:";
  for (std::vector<int>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
    std::cout << ' ' << *it;

  std::cout << '\n';

  return 0;
}

shuffle 
Randomly rearrange elements in range using generator (function template )


// shuffle algorithm example
#include <iostream>     // std::cout
#include <algorithm>    // std::shuffle
#include <array>        // std::array
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock

int main () {
  std::array<int,5> foo {1,2,3,4,5};

  // obtain a time-based seed:
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

  shuffle (foo.begin(), foo.end(), std::default_random_engine(seed));

  std::cout << "shuffled elements:";
  for (int& x: foo) std::cout << ' ' << x;
  std::cout << '\n';

  return 0;
}

***** Partitions *****

is_partitioned 
// Test whether range is partitioned (function template )

// is_partitioned example
#include <iostream>     // std::cout
#include <algorithm>    // std::is_partitioned
#include <array>        // std::array

bool IsOdd (int i) { return (i%2)==1; }

int main () {
  std::array<int,7> foo {1,2,3,4,5,6,7};

  // print contents:
  std::cout << "foo:"; for (int& x:foo) std::cout << ' ' << x;
  if ( std::is_partitioned(foo.begin(),foo.end(),IsOdd) )
    std::cout << " (partitioned)\n";
  else
    std::cout << " (not partitioned)\n";

  // partition array:
  std::partition (foo.begin(),foo.end(),IsOdd);

  // print contents again:
  std::cout << "foo:"; for (int& x:foo) std::cout << ' ' << x;
  if ( std::is_partitioned(foo.begin(),foo.end(),IsOdd) )
    std::cout << " (partitioned)\n";
  else
    std::cout << " (not partitioned)\n";

  return 0;
}


partition
Partition range in two (function template )

// partition algorithm example
#include <iostream>     // std::cout
#include <algorithm>    // std::partition
#include <vector>       // std::vector

bool IsOdd (int i) { return (i%2)==1; }

int main () {
  std::vector<int> myvector;

  // set some values:
  for (int i=1; i<10; ++i) myvector.push_back(i); // 1 2 3 4 5 6 7 8 9

  std::vector<int>::iterator bound;
  bound = std::partition (myvector.begin(), myvector.end(), IsOdd);
  

std::cout << "bound:" << *bound <<std::endl;

  // print out content:
  std::cout << "odd elements:";
  for (std::vector<int>::iterator it=myvector.begin(); it!=bound; ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';
  std::cout << "even elements:";
  for (std::vector<int>::iterator it=bound; it!=myvector.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}


stable_partition
Partition range in two - stable ordering (function template )


partition_copy 
Partition range into two (function template )


// partition_copy example
#include <iostream>     // std::cout
#include <algorithm>    // std::partition_copy, std::count_if
#include <vector>       // std::vector

bool IsOdd (int i) { return (i%2)==1; }
//if IsOdd return true then add into odd else add into even vector

int main () {
  std::vector<int> foo {1,2,3,4,5,6,7,8,9};
  std::vector<int> partOdd, partEven;

  // resize vectors to proper size:
  unsigned n = std::count_if (foo.begin(), foo.end(), IsOdd);
  partOdd.resize(n); 
  partEven.resize(foo.size()-n);

  // partition:
  std::partition_copy (foo.begin(), foo.end(), partOdd.begin(), partEven.begin(), IsOdd);

  // print contents:
  std::cout << "partOdd: ";  for (int& x:partOdd)  std::cout << ' ' << x; std::cout << '\n';
  std::cout << "partEven: "; for (int& x:partEven) std::cout << ' ' << x; std::cout << '\n';

  return 0;
}


partition_point 
Get partition point (function template )

// partition_point example
#include <iostream>     // std::cout
#include <algorithm>    // std::partition, std::partition_point
#include <vector>       // std::vector

bool IsOdd (int i) { return (i%2)==1; }

int main () {
  std::vector<int> foo {1,2,3,4,5,6,7,8,9};
  std::vector<int> odd;
  std::vector<int> even;

  std::partition (foo.begin(),foo.end(),IsOdd);

  auto it = std::partition_point(foo.begin(),foo.end(),IsOdd);
  odd.assign (foo.begin(),it);
  even.assign (it, foo.end());
  
    // print contents of foo:
  std::cout << "foo:";
  for (int& x:foo) std::cout << ' ' << x;   // foo: 1 9 3 7 5 6 4 8 2
  std::cout << '\n';

  // print contents of odd:
  std::cout << "odd:";
  for (int& x:odd) std::cout << ' ' << x; // odd: 1 9 3 7 5
  std::cout << '\n';

  // print contents of even:
  std::cout << "even:";
  for (int& x:even) std::cout << ' ' << x; // even: 6 4 8 2
  std::cout << '\n';

  return 0;
}

***** Sorting *****
sort
// Sort elements in range (function template )

// sort algorithm example
#include <iostream>     // std::cout
#include <algorithm>    // std::sort
#include <vector>       // std::vector

bool myfunction (int i,int j) { return (i<j); }

struct myclass {
  bool operator() (int i,int j) { return (i<j);}
} myobject;

int main () {
  int myints[] = {32,71,12,45,26,80,53,33};
  std::vector<int> myvector (myints, myints+8);               // 32 71 12 45 26 80 53 33

  // using default comparison (operator <):
  std::sort (myvector.begin(), myvector.begin()+4);           //(12 32 45 71)26 80 53 33

  // using function as comp
  std::sort (myvector.begin()+4, myvector.end(), myfunction); // 12 32 45 71(26 33 53 80)

  // using object as comp
  std::sort (myvector.begin(), myvector.end(), myobject);     //(12 26 32 33 45 53 71 80)

  // print out content:
  std::cout << "myvector contains:";
  for (std::vector<int>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}

stable_sort
// Sort elements preserving order of equivalents (function template )
// Its is used to sort the decimal values in correct order.
// It gives corrent answer than the int value compare function.

// stable_sort example
#include <iostream>     // std::cout
#include <algorithm>    // std::stable_sort
#include <vector>       // std::vector

bool compare_as_ints (double i,double j)
{
  return (int(i)<int(j));
}

int main () {
  double mydoubles[] = {3.14, 1.41, 2.72, 4.67, 1.73, 1.32, 1.62, 2.58};

  std::vector<double> myvector;

  myvector.assign(mydoubles,mydoubles+8);

  std::cout << "using default comparison:";
  std::stable_sort (myvector.begin(), myvector.end());
  for (std::vector<double>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  myvector.assign(mydoubles,mydoubles+8);

  std::cout << "using 'compare_as_ints' :";
  std::stable_sort (myvector.begin(), myvector.end(), compare_as_ints);
  for (std::vector<double>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}

partial_sort
// Partially sort elements in range (function template )
// Its sort elememnt in the given range only

// partial_sort example
#include <iostream>     // std::cout
#include <algorithm>    // std::partial_sort
#include <vector>       // std::vector

bool myfunction (int i,int j) { return (i<j); }

int main () {
  int myints[] = {9,8,7,6,5,4,3,2,1};
  std::vector<int> myvector (myints, myints+9);

  std::cout << "myvector contains:";
  for (std::vector<int>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';
  
  // using default comparison (operator <):
  std::partial_sort (myvector.begin(), myvector.begin()+5, myvector.end());

  std::cout << "myvector contains:";
  for (std::vector<int>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';
  
  // using function as comp
  std::partial_sort (myvector.begin(), myvector.begin()+5, myvector.end(), myfunction);

  // print out content:
  std::cout << "myvector contains:";
  for (std::vector<int>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}

partial_sort_copy
Copy and partially sort range (function template )
// Its sort elememnt in the given range only and copy into the another vector from begin to end.

// partial_sort_copy example
#include <iostream>     // std::cout
#include <algorithm>    // std::partial_sort_copy
#include <vector>       // std::vector

bool myfunction (int i,int j) { return (i<j); }

int main () {
  int myints[] = {9,8,7,6,5,4,3,2,1};
  std::vector<int> myvector (5);


  // using default comparison (operator <):
  std::partial_sort_copy (myints, myints+9, myvector.begin(), myvector.end());

// print out content:
  std::cout << "myvector contains:";
  for (std::vector<int>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';
  // using function as comp
  std::partial_sort_copy (myints, myints+9, myvector.begin(), myvector.end(), myfunction);

  // print out content:
  std::cout << "myvector contains:";
  for (std::vector<int>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}


is_sorted 
Check whether range is sorted (function template )

// is_sorted example
#include <iostream>     // std::cout
#include <algorithm>    // std::is_sorted, std::prev_permutation
#include <array>        // std::array

int main () {
  std::array<int,4> foo {2,4,1,3};

  do {
    // try a new permutation:
    std::prev_permutation(foo.begin(),foo.end());

    // print range:
    std::cout << "foo:";
    for (int& x:foo) std::cout << ' ' << x;
    std::cout << '\n';

  } while (!std::is_sorted(foo.begin(),foo.end()));

  std::cout << "the range is sorted!\n";

  return 0;
}



Output:
foo: 2 3 4 1
foo: 2 3 1 4
foo: 2 1 4 3
foo: 2 1 3 4
foo: 1 4 3 2
foo: 1 4 2 3
foo: 1 3 4 2
foo: 1 3 2 4
foo: 1 2 4 3
foo: 1 2 3 4
the range is sorted!


is_sorted_until 
Find first unsorted element in range (function template )



// is_sorted_until example
#include <iostream>     // std::cout
#include <algorithm>    // std::is_sorted_until, std::prev_permutation
#include <array>        // std::array

int main () {
  std::array<int,5> foo {1, 4, 6, 7, 3};
  std::array<int,5>::iterator it =std::is_sorted_until(foo.begin(),foo.end());
    std::cout << " till " << (it-foo.begin()) << " elements sorted\n";

  return 0;
}

// Till the 7 element is sorted

nth_element 
Sort element in range (function template )
//need to cehck

// nth_element example
#include <iostream>     // std::cout
#include <algorithm>    // std::nth_element, std::random_shuffle
#include <vector>       // std::vector

bool myfunction (int i,int j) { return (i<j); }

int main () {
  std::vector<int> myvector;

  // set some values:
  for (int i=1; i<8; i++) myvector.push_back(i);   // 1 2 3 4 5 6 7 8 9

  // print out content:
  std::cout << "myvector contains:";
  for (std::vector<int>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';
  
  std::random_shuffle (myvector.begin(), myvector.end());

  // print out content:
  std::cout << "myvector contains:";
  for (std::vector<int>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';
  
  // using default comparison (operator <):
  std::nth_element (myvector.begin(), myvector.begin()+4, myvector.end());

  // print out content:
  std::cout << "myvector contains:";
  for (std::vector<int>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';
  
  // using function as comp
  std::nth_element (myvector.begin(), myvector.begin()+4, myvector.end(),myfunction);

  // print out content:
  std::cout << "myvector contains:";
  for (std::vector<int>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}


***** Binary search (operating on partitioned/sorted ranges) *****

lower_bound
Return iterator to lower bound (function template )

// lower_bound/upper_bound example
#include <iostream>     // std::cout
#include <algorithm>    // std::lower_bound, std::upper_bound, std::sort
#include <vector>       // std::vector

int main () {
  int myints[] = {10,20,30,30,20,10,10,20};
  std::vector<int> v(myints,myints+8);           // 10 20 30 30 20 10 10 20

  std::sort (v.begin(), v.end());                // 10 10 10 20 20 20 30 30

  std::vector<int>::iterator low,up;
  low=std::lower_bound (v.begin(), v.end(), 20); //          ^
  up= std::upper_bound (v.begin(), v.end(), 20); //                   ^

  std::cout << "lower_bound at position " << (low- v.begin()) << '\n';
  std::cout << "upper_bound at position " << (up - v.begin()) << '\n';

  return 0;
}

upper_bound
Return iterator to upper bound (function template )

// lower_bound/upper_bound example
#include <iostream>     // std::cout
#include <algorithm>    // std::lower_bound, std::upper_bound, std::sort
#include <vector>       // std::vector

int main () {
  int myints[] = {10,20,30,30,20,10,10,20};
  std::vector<int> v(myints,myints+8);           // 10 20 30 30 20 10 10 20

  std::sort (v.begin(), v.end());                // 10 10 10 20 20 20 30 30

  std::vector<int>::iterator low,up;
  low=std::lower_bound (v.begin(), v.end(), 20); //          ^
  up= std::upper_bound (v.begin(), v.end(), 20); //                   ^

  std::cout << "lower_bound at position " << (low- v.begin()) << '\n';
  std::cout << "upper_bound at position " << (up - v.begin()) << '\n';

  return 0;
}

equal_range
Get subrange of equal elements (function template )


// equal_range example
// equal_range example
#include <iostream>     // std::cout
#include <algorithm>    // std::equal_range, std::sort
#include <vector>       // std::vector

bool mygreater (int i,int j) { return (i>j); }

int main () {
  int myints[] = {10,20,30,30,20,10,10,20};
  std::vector<int> v(myints,myints+8);                         // 10 20 30 30 20 10 10 20
  std::pair<std::vector<int>::iterator,
  std::vector<int>::iterator> bounds;

  // using default comparison:
  std::sort (v.begin(), v.end());                              // 10 10 10 20 20 20 30 30
  bounds=std::equal_range (v.begin(), v.end(), 20);            //          ^        ^

  std::cout << "bounds at positions " << (bounds.first - v.begin());
  std::cout << " and " << (bounds.second - v.begin()) << '\n';

  std::cout << "bounds at :" << *bounds.first  << std::endl;
  std::cout << "bounds at :" << *bounds.second  << std::endl;
  
  // using "mygreater" as comp:
  std::sort (v.begin(), v.end(), mygreater);                   // 30 30 20 20 20 10 10 10
  bounds=std::equal_range (v.begin(), v.end(), 20, mygreater); //       ^        ^

  std::cout << "bounds at :" << *bounds.first  << std::endl;
  std::cout << "bounds at :" << *bounds.second  << std::endl;

  std::cout << "bounds at positions " << (bounds.first - v.begin());
  std::cout << " and " << (bounds.second - v.begin()) << '\n';

  return 0;
}


binary_search
// Test if value exists in sorted sequence (function template )
// Need to check

// binary_search example
#include <iostream>     // std::cout
#include <algorithm>    // std::binary_search, std::sort
#include <vector>       // std::vector

bool myfunction (int i,int j) { return (i<j); }

int main () {
  int myints[] = {1,2,3,4,5,4,3,2,1};
  std::vector<int> v(myints,myints+9);                         // 1 2 3 4 5 4 3 2 1

  // using default comparison:
  std::sort (v.begin(), v.end());

  std::cout << "looking for a 3... ";
  if (std::binary_search (v.begin(), v.end(), 3))
    std::cout << "found!\n"; else std::cout << "not found.\n";

  // using myfunction as comp:
  std::sort (v.begin(), v.end(), myfunction);

  std::cout << "looking for a 6... ";
  if (std::binary_search (v.begin(), v.end(), 6, myfunction))
    std::cout << "found!\n"; else std::cout << "not found.\n";

  return 0;
}

Output:
looking for a 3... found!
looking for a 6... not found.


**** Merge (operating on sorted ranges) *****

merge
Merge sorted ranges (function template )


// merge algorithm example
#include <iostream>     // std::cout
#include <algorithm>    // std::merge, std::sort
#include <vector>       // std::vector

int main () {
    std::vector<int>  first {5,10,15,20,25};
    std::vector<int>  second {50,40,30,20,10};
    std::vector<int> vec(14);

  std::sort (first.begin(),first.end());
    std::cout << "The resulting first contains:";
  for (std::vector<int>::iterator it=first.begin(); it!=first.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';
  std::sort (second.begin(),second.end());
    std::cout << "The resulting second contains:";
  for (std::vector<int>::iterator it=second.begin(); it!=second.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';
  std::merge (first.begin(),first.end(),second.begin(),second.end(),vec.begin());

  std::cout << "The resulting vector contains:";
  for (std::vector<int>::iterator it=vec.begin(); it!=vec.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}

Output :
    The resulting first contains: 5 10 15 20 25
    The resulting second contains: 10 20 30 40 50
    The resulting vector contains: 5 10 10 15 20 20 25 30 40 50 0 0 0 0

inplace_merge
    // Merge consecutive sorted ranges (function template )
    // Need to check
// inplace_merge example
#include <iostream>     // std::cout
#include <algorithm>    // std::inplace_merge, std::sort, std::copy
#include <vector>       // std::vector

int main () {
  int first[] = {5,10,15,20,25};
  int second[] = {50,40,30,20,10};
  std::vector<int> v(10);
  std::vector<int>::iterator it;

  std::sort (first,first+5);
  std::sort (second,second+5);

  it=std::copy (first, first+5, v.begin());
     std::copy (second,second+5,it);

  std::cout << "The resulting vector contains:";
  for (it=v.begin(); it!=v.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';
  
  std::inplace_merge (v.begin(),v.begin()+5,v.end());

  std::cout << "The resulting vector contains:";
  for (it=v.begin(); it!=v.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}

includes
Test whether sorted range includes another sorted range (function template )

// includes algorithm example
#include <iostream>     // std::cout
#include <algorithm>    // std::includes, std::sort

bool myfunction (int i, int j) { return i<j; }

int main () {
  int container[] = {5,10,15,20,25,30,35,40,45,50};
  int continent[] = {40,30,20,10};

  std::sort (container,container+10);
  std::sort (continent,continent+4);

  // using default comparison:
  if ( std::includes(container,container+10,continent,continent+4) )
    std::cout << "container includes continent!\n";

  // using myfunction as comp:
  if ( std::includes(container,container+10,continent,continent+4, myfunction) )
    std::cout << "container includes continent!\n";

  return 0;
}

Output:
    container includes continent!
    container includes continent!

set_union
Union of two sorted ranges (function template )
// copy all single number between two vector into new vector

// set_union example
#include <iostream>     // std::cout
#include <algorithm>    // std::set_union, std::sort
#include <vector>       // std::vector

int main () {
  int first[] = {5,10,15,20,25};
  int second[] = {50,40,30,20,10};
  std::vector<int> v(10);                      // 0  0  0  0  0  0  0  0  0  0
  std::vector<int>::iterator it;

  std::sort (first,first+5);     //  5 10 15 20 25
  std::sort (second,second+5);   // 10 20 30 40 50

  it=std::set_union (first, first+5, second, second+5, v.begin());
                                               // 5 10 15 20 25 30 40 50  0  0
  v.resize(it-v.begin());                      // 5 10 15 20 25 30 40 50

  std::cout << "The union has " << (v.size()) << " elements:\n";
  for (it=v.begin(); it!=v.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}
 


Output:
    The union has 8 elements:
    5 10 15 20 25 30 40 50



set_intersection
Intersection of two sorted ranges (function template )
// Exactg similar number copy into the new vector

// set_intersection example
#include <iostream>     // std::cout
#include <algorithm>    // std::set_intersection, std::sort
#include <vector>       // std::vector

int main () {
  int first[] = {5,10,15,20,25};
  int second[] = {50,40,30,20,10};
  std::vector<int> v(10);                      // 0  0  0  0  0  0  0  0  0  0
  std::vector<int>::iterator it;

  std::sort (first,first+5);     //  5 10 15 20 25
  std::sort (second,second+5);   // 10 20 30 40 50

  it=std::set_intersection (first, first+5, second, second+5, v.begin());
                                               // 10 20 0  0  0  0  0  0  0  0
  v.resize(it-v.begin());                      // 10 20

  std::cout << "The intersection has " << (v.size()) << " elements:\n";
  for (it=v.begin(); it!=v.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}

set_difference
Difference of two sorted ranges (function template )
// List out the numbers only from first vector which is not present in second vector 
// and copy only those into new vector

// set_difference example
#include <iostream>     // std::cout
#include <algorithm>    // std::set_difference, std::sort
#include <vector>       // std::vector

int main () {
  int first[] = {5,10,15,20,25};
  int second[] = {50,40,30,20,10};
  std::vector<int> v(10);                      // 0  0  0  0  0  0  0  0  0  0
  std::vector<int>::iterator it;

  std::sort (first,first+5);     //  5 10 15 20 25
  std::sort (second,second+5);   // 10 20 30 40 50

  it=std::set_difference (first, first+5, second, second+5, v.begin());
                                               //  5 15 25  0  0  0  0  0  0  0
  v.resize(it-v.begin());                      //  5 15 25

  std::cout << "The difference has " << (v.size()) << " elements:\n";
  for (it=v.begin(); it!=v.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}

set_symmetric_difference
Symmetric difference of two sorted ranges (function template )
// copy single number which is not duplicate into first and second vector and copy into the new vector

// set_symmetric_difference example
#include <iostream>     // std::cout
#include <algorithm>    // std::set_symmetric_difference, std::sort
#include <vector>       // std::vector

int main () {
  int first[] = {5,10,15,20,25};
  int second[] = {50,40,30,20,10};
  std::vector<int> v(10);                      // 0  0  0  0  0  0  0  0  0  0
  std::vector<int>::iterator it;

  std::sort (first,first+5);     //  5 10 15 20 25
  std::sort (second,second+5);   // 10 20 30 40 50

  it=std::set_symmetric_difference (first, first+5, second, second+5, v.begin());
                                               //  5 15 25 30 40 50  0  0  0  0
  v.resize(it-v.begin());                      //  5 15 25 30 40 50

  std::cout << "The symmetric difference has " << (v.size()) << " elements:\n";
  for (it=v.begin(); it!=v.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}

***** Heap *****

push_heap
Push element into heap range (function template )
pop_heap
Pop element from heap range (function template )
make_heap
Make heap from range (function template )
sort_heap
Sort elements of heap (function template )

// Need to cehck the heap concept

// range heap example
#include <iostream>     // std::cout
#include <algorithm>    // std::make_heap, std::pop_heap, std::push_heap, std::sort_heap
#include <vector>       // std::vector

int main () {
  int myints[] = {10,20,30,5,15};
  std::vector<int> v(myints,myints+5);

  std::make_heap (v.begin(),v.end());
  std::cout << "initial max heap   : " << v.front() << '\n';

  std::pop_heap (v.begin(),v.end()); v.pop_back();
  std::cout << "max heap after pop : " << v.front() << '\n';

  v.push_back(99); std::push_heap (v.begin(),v.end());
  std::cout << "max heap after push: " << v.front() << '\n';

  std::sort_heap (v.begin(),v.end());

  std::cout << "final sorted range :";
  for (unsigned i=0; i<v.size(); i++)
    std::cout << ' ' << v[i];

  std::cout << '\n';

  return 0;
}



Output:
initial max heap   : 30
max heap after pop : 20
max heap after push: 99
final sorted range : 5 10 15 20 99



is_heap 
Test if range is heap (function template )

// is_heap example
#include <iostream>     // std::cout
#include <algorithm>    // std::is_heap, std::make_heap, std::pop_heap
#include <vector>       // std::vector

int main () {
  std::vector<int> foo {9,5,2,6,4,1,3,8,7};

  if (!std::is_heap(foo.begin(),foo.end()))
    std::make_heap(foo.begin(),foo.end());

  std::cout << "Popping out elements:";
  while (!foo.empty()) {
    std::pop_heap(foo.begin(),foo.end());   // moves largest element to back
    std::cout << ' ' << foo.back();         // prints back
    foo.pop_back();                         // pops element out of container
  }
  std::cout << '\n';

  return 0;
}
 
 
Output:
    Popping out elements: 9 8 7 6 5 4 3 2 1

is_heap_until 
Find first element not in heap order (function template )


// is_heap example
#include <iostream>     // std::cout
#include <algorithm>    // std::is_heap_until, std::sort, std::reverse
#include <vector>       // std::vector

int main () {
  std::vector<int> foo {2,6,9,3,8,4,5,1,7};

  std::sort(foo.begin(),foo.end());
  std::reverse(foo.begin(),foo.end());

  auto last = std::is_heap_until (foo.begin(),foo.end());

  std::cout << "The " << (last-foo.begin()) << " first elements are a valid heap:";
  for (auto it=foo.begin(); it!=last; ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}

Possible output: Most implementations consider a range sorted in reverse order a valid heap:
    The 9 first elements are a valid heap: 9 8 7 6 5 4 3 2 1


Min/max:
min
Return the smallest (function template )

// min example
#include <iostream>     // std::cout
#include <algorithm>    // std::min

int main () {
  std::cout << "min(1,2)==" << std::min(1,2) << '\n';
  std::cout << "min(2,1)==" << std::min(2,1) << '\n';
  std::cout << "min('a','z')==" << std::min('a','z') << '\n';
  std::cout << "min(3.14,2.72)==" << std::min(3.14,2.72) << '\n';
  return 0;
}
  


Output:
min(1,2)==1
min(2,1)==1
min('a','z')==a
min(3.14,2.72)==2.72



max
Return the largest (function template )


// max example
#include <iostream>     // std::cout
#include <algorithm>    // std::max

int main () {
  std::cout << "max(1,2)==" << std::max(1,2) << '\n';
  std::cout << "max(2,1)==" << std::max(2,1) << '\n';
  std::cout << "max('a','z')==" << std::max('a','z') << '\n';
  std::cout << "max(3.14,2.73)==" << std::max(3.14,2.73) << '\n';
  return 0;
}
  


Output:
max(1,2)==2
max(2,1)==2
max('a','z')==z
max(3.14,2.73)==3.14


minmax 
Return smallest and largest elements (function template )
// Return pair
// template <class T> pair <const T&,const T&> minmax (const T& a, const T& b) {
//   return (b<a) ? std::make_pair(b,a) : std::make_pair(a,b);
// }

// minmax example
#include <iostream>     // std::cout
#include <algorithm>    // std::minmax

int main () {
  auto result = std::minmax({1,2,3,4,5});

  std::cout << "minmax({1,2,3,4,5}): ";
  std::cout << result.first << ' ' << result.second << '\n';
  return 0;
}
  


Output:
minmax({1,2,3,4,5}): 1 5


min_element
Return smallest element in range (function template )
max_element
Return largest element in range (function template )


// min_element/max_element example
#include <iostream>     // std::cout
#include <algorithm>    // std::min_element, std::max_element

bool myfn(int i, int j) { return i<j; }

struct myclass {
  bool operator() (int i,int j) { return i<j; }
} myobj;

int main () {
  int myints[] = {3,7,2,5,6,4,9};

  // using default comparison:
  std::cout << "The smallest element is " << *std::min_element(myints,myints+7) << '\n';
  std::cout << "The largest element is "  << *std::max_element(myints,myints+7) << '\n';

  // using function myfn as comp:
  std::cout << "The smallest element is " << *std::min_element(myints,myints+7,myfn) << '\n';
  std::cout << "The largest element is "  << *std::max_element(myints,myints+7,myfn) << '\n';

  // using object myobj as comp:
  std::cout << "The smallest element is " << *std::min_element(myints,myints+7,myobj) << '\n';
  std::cout << "The largest element is "  << *std::max_element(myints,myints+7,myobj) << '\n';

  return 0;
}


Output:
The smallest element is 2
The largest element is 9
The smallest element is 2
The largest element is 9
The smallest element is 2
The largest element is 9




minmax_element 
Return smallest and largest elements in range (function template )

// template <class ForwardIterator>
//   pair<ForwardIterator,ForwardIterator>
//     minmax_element (ForwardIterator first, ForwardIterator last);

// minmax_element
#include <iostream>     // std::cout
#include <algorithm>    // std::minmax_element
#include <array>        // std::array

int main () {
  std::array<int,7> foo {3,7,2,9,5,8,6};

  auto result = std::minmax_element (foo.begin(),foo.end());

  // print result:
  std::cout << "min is " << *result.first;
  std::cout << ", at position " << (result.first-foo.begin()) << '\n';
  std::cout << "max is " << *result.second;
  std::cout << ", at position " << (result.second-foo.begin()) << '\n';

  return 0;
}


Output:
min is 2, at position 2
max is 9, at position 3


Other:
lexicographical_compare
Lexicographical less-than comparison (function template )

// lexicographical_compare example
#include <iostream>     // std::cout, std::boolalpha
#include <algorithm>    // std::lexicographical_compare
#include <cctype>       // std::tolower

// a case-insensitive comparison function:
bool mycomp (char c1, char c2)
{  std::cout << c1<< std::tolower(c1)  << c2 << std::tolower(c2) << std::endl;
    return std::tolower(c1)<std::tolower(c2); }

int main () {
  char foo[]="Apple";
  char bar[]="apartment";

  std::cout << std::boolalpha;

  std::cout << "Comparing foo and bar lexicographically (foo<bar):\n";

  std::cout << "Using default comparison (operator<): ";
  std::cout << std::lexicographical_compare(foo,foo+5,bar,bar+9);
  std::cout << '\n';

  std::cout << "Using mycomp as comparison object: ";
  std::cout << std::lexicographical_compare(foo,foo+5,bar,bar+9,mycomp);
  std::cout << '\n';

  return 0;
}

Output:
    Comparing foo and bar lexicographically (foo<bar):
    Using default comparison (operator<): true
    Using mycomp as comparison object: A97a97
    a97A97
    p112p112
    p112p112
    p112a97
    a97p112
    false

next_permutation
Transform range to next permutation (function template )

// next_permutation example
#include <iostream>     // std::cout
#include <algorithm>    // std::next_permutation, std::sort

int main () {
  int myints[] = {1,2,3};

  std::sort (myints,myints+3);

  std::cout << "The 3! possible permutations with 3 elements:\n";
  do {
    std::cout << myints[0] << ' ' << myints[1] << ' ' << myints[2] << '\n';
  } while ( std::next_permutation(myints,myints+3) );

  std::cout << "After loop: " << myints[0] << ' ' << myints[1] << ' ' << myints[2] << '\n';

  return 0;
}
 
Output:
The 3! possible permutations with 3 elements:
1 2 3
1 3 2
2 1 3
2 3 1
3 1 2
3 2 1
After loop: 1 2 3


prev_permutation
Transform range to previous permutation (function template )


// next_permutation example
#include <iostream>     // std::cout
#include <algorithm>    // std::next_permutation, std::sort, std::reverse

int main () {
  int myints[] = {1,2,3};

  std::sort (myints,myints+3);
  std::reverse (myints,myints+3);

  std::cout << "The 3! possible permutations with 3 elements:\n";
  do {
    std::cout << myints[0] << ' ' << myints[1] << ' ' << myints[2] << '\n';
  } while ( std::prev_permutation(myints,myints+3) );

  std::cout << "After loop: " << myints[0] << ' ' << myints[1] << ' ' << myints[2] << '\n';

  return 0;
}
  
Output:
3 2 1
3 1 2
2 3 1
2 1 3
1 3 2
1 2 3
After loop: 3 2 1


Extra example 
// next_permutation example
#include <iostream>     // std::cout
#include <algorithm>    // std::next_permutation, std::sort

int main () {
  int myints[] = {1,2,3,4,5};
 int count = 0;
  std::sort (myints,myints+3);

  std::cout << "The 3! possible permutations with 3 elements:\n";
  do {
      count++;
    std::cout << myints[0] << ' ' << myints[1] << ' ' << myints[2] << ' ' << myints[3] << ' ' << myints[4] << '\n';
  } while ( std::next_permutation(myints,myints+3) );

  std::cout << "After loop: " << count << " : " << myints[0] << ' ' << myints[1] << ' ' << myints[2] << ' ' << myints[3] << ' ' << myints[4] << '\n';

  return 0;
}
 
Output:
The 3! possible permutations with 3 elements:
1 2 3 4 5
1 3 2 4 5
2 1 3 4 5
2 3 1 4 5
3 1 2 4 5
3 2 1 4 5
After loop: 6 : 1 2 3 4 5