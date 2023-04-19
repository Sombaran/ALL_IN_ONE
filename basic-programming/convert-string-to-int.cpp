// 1. Using stringstream class or sscanf()
//      stringstream() : This is an easy way to convert strings of digits into
//      ints, floats or doubles.

// 1. stringstream to convert string to int.

#include <iostream>
#include <sstream>
using namespace std;

int main()
{
    string s = "12345";

    // object from the class stringstream
    stringstream geek(s);

    // The object has the value 12345 and stream
    // it to the integer x
    int x = 0;
    geek >> x;

    // Now the variable x holds the value 12345
    cout << "Value of x : " << x;

    return 0;
}

// 2. sscanf() is a C style function similar to scanf().
//      It reads input from a string rather that standard input.

#include <stdio.h>
int main()
{
    const char *str = "12345";
    int x;
    sscanf(str, "%d", &x);
    printf("\nThe value of x : %d", x);
    return 0;
}

// 3. String conversion using stoi() or atoi()
//          stoi() : The stoi() function takes a string as an argument
//                  returns its value

int main()
{
    string str1 = "45";
    string str2 = "3.14159";
    string str3 = "31337 geek";

    int myint1 = stoi(str1);
    int myint2 = stoi(str2);
    int myint3 = stoi(str3);

    cout << "stoi(\"" << str1 << "\") is "
         << myint1 << '\n';
    cout << "stoi(\"" << str2 << "\") is "
         << myint2 << '\n';
    cout << "stoi(\"" << str3 << "\") is "
         << myint3 << '\n';

    return 0;
}

// Output:
//     stoi("45") is 45
//     stoi("3.14159") is 3
//     stoi("31337 geek") is 31337

// 4. atoi() : The atoi() function takes a character array or string literal
//           as an argument and returns its value

int main()
{

    char *str1 = "42";
    char *str2 = "3.14159";
    char *str3 = "31337 geek";

    int myint1 = atoi(str1);
    int myint2 = atoi(str2);
    int myint3 = atoi(str3);

    printf("stoi(%s) is %d \n", str1, myint1);
    printf("stoi(%s) is %d \n", str2, myint2);
    printf("stoi(%s) is %d \n", str3, myint3);

    // This Code is Contributed by Harshit Srivastava
    return 0;
}
// Output:
//     atoi("42") is 42
//     atoi("3.14159") is 3
//     atoi("31337 geek") is 31337
// Notes:

// stoi() vs atoi()
//     atoi() is a legacy C-style function. stoi() is added in C++ 11.
//     atoi() works only for C-style strings (character array and
//     string literal), stoi() works for both C++ strings and C style strings

//     atoi() takes only one parameter and returns integer value.
//         int atoi (const char * str);

//     stoi() can take upto three parameters, the second parameter is for
//     starting index and third parameter is for base of input number.
//         int stoi (const string&  str, size_t* index = 0, int base = 10);