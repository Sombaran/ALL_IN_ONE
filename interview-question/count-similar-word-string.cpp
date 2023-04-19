#include <iostream>
#include <algorithm>
#include <string>
#include <map>

/*
Given a string as input, identify the word with most occurrence.
Ex:
    Input : Hello Good Morning. This is a test string to test the programming skill
    Output :
        Word: test
        Count : 2

    Input : I am good at cooking. I cook daily. My friends ask me how I learnt the skill.
    Output :
        Output : I
        Count : 3

Note: Not case sensitive. i.e THE=the=The, I=I

*/
using namespace std;

int main()
{
    string str = "Hello Good Morning. This is a test string to test the programming test skill";
    map<string, int> Map;

    string word;
    string temp = "";

    int len = str.length();

    for (int i = 0; i < len; i++)
    {
        if (str[i] != ' ')
        {
            temp.push_back(str[i]);
        }
        else
        {
            Map.insert(pair<string, int>(temp, 0));
            map<string, int>::iterator it;
            for (it = Map.begin(); it != Map.end(); it++)
            {
                if (it->first == temp)
                {
                    it->second++;
                    Map[temp] = it->second;
                }
            }
            temp = "";
        }
    }

    int count = 0;
    for_each(begin(Map), end(Map), [&](std::pair<std::string, int> element)
             {
        if (count < element.second)
        {
            word = element.first;
            count = element.second;
        } 
        });
        cout << word << " " << count << endl; 
    return 0;
}