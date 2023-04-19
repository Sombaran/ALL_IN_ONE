#include <iostream>
using namespace std;

long findLength(const char *search_text)
{
    long size = 0;

    while (*search_text)
    {
        size += 1;
        search_text += 1;
    }
    return size;
}

int main()
{
    char text[] = "joti loved priya and priya loved joti also jotipriya loves prajot";
    char search_text[] = "jotipriya";
    int pos_search = 0, pos_text = 0;
    bool flag = false;

    for (pos_text = 0; pos_text < findLength(text); pos_text++)
    {
        if (text[pos_text] == search_text[pos_search])
        {
            pos_search++;
            if (pos_search == findLength(search_text))
            {
                cout << "match found from " << pos_text - pos_search << " to " << pos_text << endl;
                flag = true;
                break;
            }
        }
        else
        {
            pos_search = 0;
            flag = false;
        }
    }

    if (!flag)
        cout << "no match found" << endl; // no match
    else
        for (int i = pos_text - pos_search; i <= pos_text; i++)
        {
            cout << text[i] << " ";
        }

    return 0;
}