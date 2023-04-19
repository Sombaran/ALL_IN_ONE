/*


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
*/