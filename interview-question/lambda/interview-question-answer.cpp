// All about C++ Lambda expression :

/*
1. Can we use both capture by value and capture by reference in a single lambda capture clause?

    Yes, we can use both capture by variable and capture by reference in a single lambda expression.

    For example,

    [&num1, num2] - capture num1 by reference and num2 by value
    [&, num1, num2] - capture num1 and num2 by value and the rest by reference
    [=, &num1, &num2] - capture num1 and num2 by reference and the rest by value



2. How can we write a lambda expression without using the auto keyword?

    We can do that using the function<> template.
    In order to do that, we first need to import <functional> header file.

    Here,
        function<void(int, int)>
    explicitly tells that the lambda function is of void type and takes two integer parameters.

    #include <iostream>
    #include<functional>
    using namespace std;

    int main() {

        function<void(int, int)> add = [] (int a, int b) {
            cout << "Sum: " << a + b;
        };

        add(1, 2);

    return 0;
    }

    Output

        Sum: 3

3. How to use a generic lamba?
    In C++14, generic lambda was introduced to support generic template parameters in the lambda function.

    For example,

    #include <iostream>
    using namespace std;

    int main() {
        auto display = [] (auto s) {
            cout << s << endl;
        };

        display(1);
        display(2.5);
        display("Kathmandu");

        return 0;
    }

4. What is mutable keyword in Lambda?
    The lambda mutable keyword allows lambda to modify the variable captured by value inside the lambda body without affecting its original value in the enclosing function.

    However the variables captured by reference are not affected. For example,

    #include <iostream>
    using namespace std;

    int main() {

    int a = 1;
    int b = 1;

    cout << "In main():" << endl;
    cout << "a = " << a << ", ";
    cout << "b = " << b << endl;
    cout << endl;

    auto add_one = [a, &b] () mutable {

        // modify both a & b
        a++;
        b++;

        cout << "In add_one():" << endl;
        cout << "a = " << a << ", ";
        cout << "b = " << b << endl;
    };

    add_one();

    cout << endl;
    cout << "In main():" << endl;
    cout << "a = " << a << ", ";
    cout << "b = " << b << endl;

    return 0;
    }

5. How to create an immediately invoked lambda expression?
    Immediately invoked lambda expression is a lambda expression which is immediately invoked as soon as it is defined. For example,

    #include<iostream>
    using namespace std;

    int main(){

    int num1 = 1;
    int num2 = 2;

    // invoked as soon as it is defined
    auto sum = [] (int a, int b) {
        return a + b;
    } (num1, num2); // caller argument passed here

    cout << "The sum of " << num1 << " and " << num2  << " is " << sum;

    return 0;
    }

6.  What is the extended syntax of the C++ Lambda Expression?
    The extended syntax of the C++ Lambda Expression is:

    [capture_list] (parameter_list) mutable -> return_type {
    // lambda body
    }

    As you can see, this syntax is far more comprehensive compared to the basic syntax we have provided in the beginning of our tutorial.

    This is because it consists of all the lambda elements we have discussed up to this point:

    [capture_list] - captures the variables of the enclosing function
    (parameter_list) - parameters to be used inside the lambda expression
    mutable - allows variables captured by value to be modified inside the lambda
    return_type - explicitly defines the return type of the lambda expression


*/