/*

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

*/