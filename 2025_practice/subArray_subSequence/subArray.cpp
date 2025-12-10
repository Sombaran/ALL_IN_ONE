#include<iostream>
#include<vector>

using namespace std;

int maxSubarraySum(vector<int> &arr) {
    int res = arr[0];

    // Outer loop for starting point of subarray
    for(int i = 0; i < arr.size(); i++) {
        int currSum = 0; 
        // Inner loop for ending point of subarray
        for(int j = i; j < arr.size(); j++) {
            currSum +=  arr[j];
            // Update res if currSum is greater than res
            res = max(res, currSum);
        }
    }
    return res;
}



int main(int argc, char** argv)
{
    std::vector<int> arr = {2, 3, -8, 7, -1, 2, 3};
    std::cout << maxSubarraySum(arr) << std::endl;
    return (0);
}