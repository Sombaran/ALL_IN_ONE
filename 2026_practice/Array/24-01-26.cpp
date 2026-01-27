#include <bits/stdc++.h>
using std::cout;
using std::endl;

bool checkKthBit(int n, int k) {
    int mask = 1 << k;
    int mask1 = 1 << (n - 1);
    std::cout << "Mask: " << mask << endl;
    std::cout << "Mask1: " << mask1 << endl;
    return (n & mask) != 0;
}

std::string formatRange(long start, long end) {
    if (start == end) {
        return std::to_string(start);
    } else {
        return std::to_string(start) + "->" + std::to_string(end);
    }
}


std::vector<std::string> missingRange(std::vector<int>& nums, int lower, int upper) {
    std::vector<std::string> result{};
    long prev = static_cast<long>(lower) - 1;

    for (int i=0;i<=nums.size();++i) {
        long curr = (i < nums.size()) ? static_cast<long>(nums[i]) : static_cast<long>(upper) + 1;

        if (curr - prev >= 2) {
            result.push_back(formatRange(prev + 1, curr - 1));
        }
        prev = curr;
    }
    return {result};
}

void moveZeroes(std::vector<int>& nums) {
    int count {};

    for (int i=0;i<nums.size();++i) {
        if (nums[i] != 0) {
            std::swap (nums[count++], nums[i]);
        }
    }
}

bool isPalindrome(const std::string& s) {
    int left {};
    int right = s.length() -1;



    while (left < right) {

        if (s[left] != s[right]) {
            return false;
        }
        if (s[left] == s[right]) {
            left++;
            right--;
        }
    }
    return true;
}

double powerof2(double number, int exponent) {
    
    if (exponent == 0) return 1.0;

    if (exponent < 0 ) return 1/powerof2(number, -exponent);

    double half = powerof2(number, exponent / 2);

    if ((exponent & 1 ) == 0) {
        return half * half;
    } else {
        return half * half * number;
    }
}

std::vector<int> countBits(int n) {
    std::vector<int> ans(n + 1, 0);
    for (int i = 1; i <= n; i++) {
        // Remove lowest set bit and add 1
        ans[i] = ans[i & (i - 1)] + 1;
    }
    return ans;
}




int main(int argc, char** argv)
{
    int n = 7, k = 2;

    if (checkKthBit(n, k)) {
        cout << "true" << endl;
    } else {
        cout << "false" << endl;
    }


    std::vector<int> nums {0,1,3,50,75};
    int lower = 0, upper = 99;
    std::vector<std::string> result = missingRange(nums, lower, upper);
    for (const auto& str : result) {
        cout << str << " ";
    }
    cout << endl;
    std::vector<int> arr {1,2,0,4,3,0,5,0};
    moveZeroes(arr);
    for (const auto& num : arr) {
        cout << num << " ";
    }
    cout << endl;

    isPalindrome("racecar") ? cout << "Palindrome" << endl : cout << "Not a Palindrome" << endl;

    double answer = powerof2(2.10000, 3);
    cout << "Answer: " << answer << endl;


    int number = 5;
    std::vector<int> res = countBits(number);
    for (int x : res) cout << x << " ";
    std::cout << std::endl;
    return (0);
}
