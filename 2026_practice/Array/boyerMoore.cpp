
#include <bits/stdc++.h>
using namespace std;

// Build bad character table: last occurrence index of each char in the pattern
vector<int> buildBadChar(const string& pat) {
    const int ALPH = 256;              // ASCII
    vector<int> bad(ALPH, -1);
    for (int i = 0; i < (int)pat.size(); ++i) {
        bad[(unsigned char)pat[i]] = i;
    }
    return bad;
}

vector<int> boyerMoore(const string& text, const string& pat) {
    int n = text.size(), m = pat.size();
    if (m == 0 || n < m) return {};

    auto bad = buildBadChar(pat);
    vector<int> positions;

    int s = 0; // shift of the pattern relative to text
    while (s <= n - m) {
        int j = m - 1;

        // Compare from right to left
        while (j >= 0 && pat[j] == text[s + j]) {
            j--;
        }

        if (j < 0) {
            // Match found at s
            positions.push_back(s);
            // Shift so that the next char in text aligns with last occurrence
            s += (s + m < n) ? (m - bad[(unsigned char)text[s + m]]) : 1;
        } else {
            // Bad character rule shift
            int last = bad[(unsigned char)text[s + j]];
            s += max(1, j - last);
        }
    }
    return positions;
}

int main() {
    string text = "HERE IS A SIMPLE EXAMPLE";
    string pat  = "EXAMPLE";
    auto pos = boyerMoore(text, pat);

    for (int p : pos) {
        cout << "Pattern found at index " << p << "\n";
    }
    return 0;
}
