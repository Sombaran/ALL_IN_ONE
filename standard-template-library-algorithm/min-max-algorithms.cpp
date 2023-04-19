/*

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


*/