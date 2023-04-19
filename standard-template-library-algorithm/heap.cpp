/*

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

*/