#include<iostream>
#include <vector>
#include <functional>

using namespace std;


std::function<int(const std::string&, const std::string& )> lcs = 
    [](const std::string& s1, const std::string& s2) {
    int n = s1.size();
    int m = s2.size();

};



int main(int argc, char** argv)
{
    string s1 = "AGGTAB";
    string s2 = "GXTXAYB";
    cout << lcs(s1, s2) << endl;
    return (0);
}