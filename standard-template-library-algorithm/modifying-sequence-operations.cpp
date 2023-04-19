/*


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

*/