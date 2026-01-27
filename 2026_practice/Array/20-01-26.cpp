
#include <bits/stdc++.h>




void rangeSum (std::vector<int>& nums){
    for (int i = 1; i < nums.size(); ++i){
        nums[i] += nums[i - 1];
    }
}



int main(int argc, char** argv)
{

    std::vector <int> nums {2,1,5,1,3,2};
    rangeSum (nums);
    for (const auto& num : nums){
        std::cout << num << " ";
    }
    std::cout << std::endl;
    return (0);
}