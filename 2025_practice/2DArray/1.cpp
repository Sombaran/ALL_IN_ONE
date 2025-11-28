#include<iostream>
#include<functional>
using namespace std;


/*

*
* *
* * *
* * * *
* * * * *

*/

std::function<void()> patternStar1 = [] {
    for (int i = 1; i <= 5; i++)
    {
        for (int j = 1; j <= i; j++)
        {
            cout << "* ";           
        }
        cout << endl;
    }
};

/**
1
2 2
3 3 3
4 4 4 4
5 5 5 5 5
 */
std::function<void()> patternNumber1 = [] {
    for (int i = 1; i <= 5; i++)
    {
        for (int j = 1; j <= i; j++)
        {
            cout << i << " ";
        }
        cout << endl;
    }
};

/*
1
1 2
1 2 3
1 2 3 4
1 2 3 4 5
*/

std::function<void()> patternNumber2 = [] {
    for (int i = 1; i <= 5; i++)
    {
        for (int j = 1; j <= i; j++)
        {
            cout << j << " ";
        }
        cout << endl;
    }
};


/*
A
B B
C C C
D D D D
E E E E E
*/

std::function<void()> patternAlphabet1 = [] {
    for (char i = 'A'; i <= 'E'; i++)
    {
        for (char j = 'A'; j <= i; j++)
        {
            cout << i << " ";
        }
        cout << endl;
    }
};

/* 
A
A B
A B C
A B C D
A B C D E
*/

std::function<void()> patternAlphabet2 = [] {
    for (char i = 'A'; i <= 'E'; i++)
    {
        for (char j = 'A'; j <= i; j++)
        {
            cout << j << " ";
        }
        cout << endl;
    }
};

/*
         * 
       * * 
     * * * 
   * * * * 
 * * * * * 

*/

std::function<void()>patternLeftHalfPattern = [] {
    for (int i = 0; i < 5; i++) {
        // Print leading spaces
        for (int j = 0; j < 5 - i; j++) {
            cout << "  ";  // Two spaces for alignment
        }
        // Print stars
        for (int k = 0; k <= i; k++) {
            cout << "* ";
        }

        cout << endl;
    }
};

int main(int argc, char** argv)
{
    /**
     * @brief: Right Half Pyramid Pattern
     */
    patternStar1();
    patternNumber1();
    patternNumber2();
    patternAlphabet1();
    patternAlphabet2();
    /**
     * @brief:Left Half Pyramid Pattern
     */
    patternLeftHalfPattern();
    return (0);
}