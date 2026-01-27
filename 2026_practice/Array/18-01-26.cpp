#include <bits/stdc++.h>



int sumInWindow(const std::vector<int>& nums, int& k) {
    
    if (nums.size() < k) return -1;

    int sum {};
    for(int i = 0; i < k; ++i) {
        sum += nums[i];
    }
    int maxSum = sum;
    for(int i = k; i < nums.size(); ++i) {
        sum = sum + nums[i] - nums[i - k];
        maxSum = std::max(maxSum, sum);        
    }
    return maxSum;
}




int main(int argc, char** argv)
{
    std::vector <int> nums {2,1,5,1,3,2};
    int windwoSize = 3;
    std::cout << sumInWindow (nums, windwoSize) << std::endl;
    return (0);
}