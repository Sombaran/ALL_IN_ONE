// deleted function c++11
/*
"=delete" is used to make any function as deleted function, In other word that function will NOT be available for anyone (if someone call this function then will get compilation error (error: use of deleted function)).

Use Case: In some scenario, we want to prevent some function from calling so simple solution is that just don’t declare them but this solution will not work for "default functions" (default constructor, default destructor, copy constructor, assignment operator, move constructor and move assignment operator ) because IF we wont declare them then compiler will provide them for us and prevention of function will NOT happen.

QUESTION: How to prevent from default functionalities (like copy constructor) ?

SOLUTION 1: Don’t declare them then compiler will provide this function for us.

SOLUTION 2: Just only declare this(copy constructor) in private scope so that nobody can call them. Exception: Still member functions and friend functions can call this prevented function and will get linking error"undefined reference" because body of prevented function is not present.

SOLUTION 3: Use "=delete" to mark copy construction as "deleted function".

Now if some one try to call this "prevented function(copy constructor)" and will get  compilation error"error: use of deleted function "Sample::Sample(const Sample&temp)". Example:
Sample(const Sample&)=delete;
Sample& operator=(const Sample&)=delete;

ADVANTAGES OF "deleted function":

Using "deleted function" we will get early detection(its always better to get compiler error than linking error.).
We can use "=delete" in public scope also so no need declare "prevented function" in private scope.
"=delete" is also works with normal function.
"=delete" is also works with template.
Complete source code with example:
*/
#include <iostream>
using namespace std;
class Deleted
{
public:
    Deleted(){};
    ~Deleted(){};
    Deleted(const Deleted &obj) = delete;            // only declaration with delete and its public
    Deleted &operator=(const Deleted &obj) = delete; // only declaration with delete and its public

    void checkFun()
    {
    }
    void fun2() = delete;
};

int main()
{
    std::cout << "In Deleted Function" << std::endl;
    Deleted obj1;
    obj1.checkFun();
    obj1.fun2();
    // error: use of deleted function ‘void Deleted::fun2()’
    return 0;
}
/*
Output:
    g++ deleted.cpp
    deleted.cpp: In function ‘int main()’:
    deleted.cpp:34:15: error: use of deleted function ‘void Deleted::fun2()’
    obj1.fun2();
    deleted.cpp:26:10: note: declared here
    void fun2()=delete;
*/