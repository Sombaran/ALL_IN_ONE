/*
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
*/
