#include <stack> //Include STL stack

/*
*Program to check balanced parentheses
using stack in C++
*/

#include <iostream>
using namespace std;

class BalancedParenthesisChecker
{
public:
      bool isParenthesisBalanced(char s[])
      {

            // Char STL stack
            stack<char> Stack;
            int i = 0;

            /* Traverse the given string expression
            to check matching brackets */

            while (s[i])
            {
                  /*If the s[i] is a opening bracket then
                  push it to Stack*/

                  if (s[i] == '(' || s[i] == '{' || s[i] == '[')
                  {
                        Stack.push(s[i]);
                  }
                  /* If s[i] is a opening bracket then
                   *get top character from stack and match it
                   *to the current character if match
                   *found then pop it from Stack else
                   *return false*/

                  if (s[i] == ')' || s[i] == '}' || s[i] == ']')
                  {
                        if (Stack.empty() || !isEqual(Stack.top(), s[i]))
                        {
                              return false;
                        }
                        else
                        {
                              // Corresponding brackets matched
                              // pop it from stack
                              Stack.pop();
                        }
                  }
                  i++;
            }

            /*If Stack is empty then parenthesis
            are balanced or else not balanced
            */

            return Stack.empty();
      }

private:
      bool isEqual(char c1, char c2)
      {

            if (c1 == '(' && c2 == ')')
                  return true;
            else if (c1 == '{' && c2 == '}')
                  return true;
            else if (c1 == '[' && c2 == ']')
                  return true;
            else
                  return false;
      }
};

/* Test program */

int main()
{
      char str[50];
      BalancedParenthesisChecker cheker;

      cout << "Enter parenthesis expression:" << endl;
      cin.getline(str, 50);

      bool isBalanced = cheker.isParenthesisBalanced(str);

      if (isBalanced)
      {
            cout << "Balanced" << endl;
      }
      else
      {
            cout << "Not Balanced" << endl;
      }

      return 0;
}