/*
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

*/