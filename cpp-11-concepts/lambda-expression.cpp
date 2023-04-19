// // Lambda expression in C++

//     C++ 11 introduced lambda expression to allow us write an inline function.
//     Which can be used for short snippets of code that are not going to be reuse.
//     Its not worth naming seperately.

//     In its simplest form lambda expression can be defined as follows:

//     [ capture clause ] (parameters) -> return-type
//     {
//         definition of method
//     }

// Note :
//  C++ Lambda expression allows us to define anonymous function objects(functors)
//  which can either be used inline or passed as an argument.

//  Lambda expression was introduced in C++11 for creating anonymous functors
//  in a more convenient and concise way.

//  They are more convenient because we do not need to overload the () operator
//  in a separate class or struct.

// Creating a Lambda Expression in C++
//     A basic lambda expression can look something like this:

//     auto greet = []() {
//     // lambda function body
//     };
// Here,
//     [] is called the lambda introducer which denotes the start of the lambda expression
//     () is called the parameter list which is similar to the () operator of a normal function

// Types of Lambda expression can be used in programming :

//     1. C++ Lambda Function With Parameters

#include <iostream>
using namespace std;
#ifndef NULL
int main()
{

    // lambda function that takes two integer
    //  parameters and displays their sum
    auto add = [](int a, int b)
    {
        cout << "Sum = " << a + b;
    };

    // call the lambda function
    add(100, 78);

    return 0;
}
#endif
//      2. C++ Lambda Function With Return Type

// syntex:
//     auto operation = []  (int a, int b,  string op) -> double {
//     if (op == "sum") {
//         // returns integer value
//         return a + b;
//     }
//     else {
//         // returns double value
//         return (a + b) / 2.0;
//     }
//     };

#include <iostream>
using namespace std;
#ifndef NULL
int main()
{

    // lambda function with explicit return type 'double'
    // returns the sum or the average depending on operation
    auto operation = [](int a, int b, string op) -> double
    {
        if (op == "sum")
        {
            return a + b;
        }
        else
        {
            return (a + b) / 2.0;
        }
    };

    int num1 = 1;
    int num2 = 2;

    // find the sum of num1 and num2
    auto sum = operation(num1, num2, "sum");
    cout << "Sum = " << sum << endl;

    // find the average of num1 and num2
    auto avg = operation(num1, num2, "avg");
    cout << "Average = " << avg;

    return 0;
}
#endif
// //       3. C++ Lambda Function Capture Clause

//     By default, lambda functions cannot access variables of the enclosing function.
//     In order to access those variables, we use the capture clause.

//     Note:
//     Here, we can only read the variable inside the lambda body but cannot modify it.

//     We can capture the variables in two ways:
//        1. Capture by Value

// syntex:

// void function()
// {

//     int num_main = 100;

//     // get access to num_main from the enclosing function
//     auto my_lambda = [num_main]()
//     {
//         cout << num_main;
//     };
// }

// Example of Lambda expression external veriable capture by value:

#include <iostream>
using namespace std;
#ifndef NULL
int main()
{

    int initial_sum = 100;

    // capture initial_sum by value
    auto add_to_sum = [initial_sum](int num)
    {
        // here inital_sum = 100 from local scope
        return initial_sum + num;
    };

    int final_sum = add_to_sum(78);
    cout << "100 + 78 = " << final_sum;

    return 0;
}
#endif
//        2. Capture by reference

// Syntex :

// void function()
// {

//     int num_main = 100;

//     // get access to num_main from the enclosing function
//     auto my_lambda = [&num_main]()
//     {
//         cout << num_main;
//     };
// }

// Example of Lambda expression external veriable capture by reference:
#ifndef NULL
int main()
{
    int length = 15;

    auto multiplay = [length](int height)
    {
        return length * height * 0.5;
    };

    double area = multiplay(25);

    cout << "area of riangle: " << area << endl;
}
#endif
//----------------------------------------------------------------------------------------
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
class Student
{

public:
    int marks;
    string name;

    Student(int val, string name) : marks(val), name(name)
    {
    }
};

struct StudentFunctor
{
    bool operator()(Student &lhs, Student &rhs)
    {
        return lhs.marks > rhs.marks;
    }
};

int main()
{
    vector<int> Vec{1, 2, 3, 4, 5, 6, 7, 8, 9};

    for_each(Vec.begin(), Vec.end(), [](int i)
             { cout << i << " "; });

    cout << endl;

    auto printEle = [](int i)
    { cout << i << " "; };

    for_each(begin(Vec), end(Vec), printEle);

    cout << endl;

    vector<int> v{4, 1, 3, 5, 2, 3, 1, 7};

    for_each(v.begin(), v.end(), [](int i)
             { if(i>3)cout << i << " "; });
    cout << endl;
    vector<Student> StrudentVec = {
        Student(10, "Swami"),
        Student(90, "Joti"),
        Student(100, "Priya"),
        Student(70, "Giri"),
    };

    for_each(begin(StrudentVec), end(StrudentVec), [](Student &obj)
             { cout << obj.marks << " " << obj.name << endl; });

    sort(begin(StrudentVec), end(StrudentVec), [](Student &lhs, Student &rhs)
         { return lhs.marks < rhs.marks; });

    cout << "Using functor Predicate: " << endl;
    
    for_each(begin(StrudentVec), end(StrudentVec), [](Student &obj)
             { cout << obj.marks << " " << obj.name << endl; });

    sort(begin(StrudentVec), end(StrudentVec), StudentFunctor()); // functor

    cout << endl;

    for_each(begin(StrudentVec), end(StrudentVec), [](Student &obj)
             { cout << obj.marks << " " << obj.name << endl; });

    auto StudentLambda = []() -> bool
    {
        return true;
    };

    auto pred = [](const Student &a, const Student &b) -> bool
    {
        return a.marks > b.marks;
    };

    sort(begin(StrudentVec), end(StrudentVec),
         pred);

    cout << "Using Lambda Predicate: " << endl;

    for_each(begin(StrudentVec), end(StrudentVec), [](Student &obj)
             { cout << obj.marks << " " << obj.name << endl; });

    return 0;
}