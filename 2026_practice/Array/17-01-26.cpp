


#include <bits/stdc++.h>



void rightRotate(std::vector<int>& arr, int d) {
    int n = static_cast<int>(arr.size());
    d = d % n; // In case d > n
    std::reverse(arr.begin(), arr.end());
    std::reverse(arr.begin(), arr.begin() + d);
    std::reverse(arr.begin() + d, arr.end());
}

void leftRotate(std::vector<int>& arr, int d) {
    int n = static_cast<int>(arr.size());
    d = d % n; // In case d > n
    std::reverse(arr.begin(), arr.begin() + d);
    std::reverse(arr.begin() + d, arr.end());
    std::reverse(arr.begin(), arr.end());
}

size_t majorityElement(std::vector<int>& arr) {

    int count {};
    int candidate {};

    for (const auto& num : arr) {
        if (count == 0) {
            candidate = num;
            count = 1;
        }
        else if (num == candidate) {
            count++;
        } 
        else {
            count--;
        }
    }
    // Verify if candidate is actually the majority element
    count = 0;
    for (const auto& num : arr) {
        if (num == candidate) {
            count++;
        }
    }
    if (count > arr.size() / 2) {
        return candidate;
    }
    return -1; // No majority element
}

    std::vector<int> majorityElementII(std::vector<int>& nums) {
        std::vector<int> result{};
        int cnt1{}, cnt2{};
        int cnd1 {}, cnd2 {};

        for (const auto&i : nums) {
            if (cnd1 == i) {
                cnt1++;
            }
            else if (cnd2 == i) {
                cnt2++;
            }
            else if (cnt1 == 0) {
                cnd1 = i;
                cnt1 = 1;
            }
            else if (cnt2 == 0) {
                cnd2 = i;
                cnt2 = 1;
            }
            else {
                cnt1--;
                cnt2--;
            }
        }
        cnt1=0,cnt2=0;
        for(const auto&i:nums){
            if (i == cnd1) {
                cnt1++;
            }
            else if (i == cnd2) {
                cnt2++;
            }
        }


        if (cnt1 > (int)nums.size() / 3) result.emplace_back(cnd1);
        if (cnt2 > (int)nums.size() / 3) result.emplace_back(cnd2);
        return result;
}


void d2MatrixPrintDiagonalElements(const std::vector<std::vector<int>>& matrixReceivedFromMain) {
    for (int i=0;i<matrixReceivedFromMain.size();i++) {
        //std::cout << matrixReceivedFromMain[i][i] << " ";
        for (int j=0;j<matrixReceivedFromMain[i].size();j++) {
            //if (i == j) {
                std::cout << matrixReceivedFromMain[i][j] << " ";
           // }
        }
    }
    std::cout << std::endl;
}

int main(int argc, char** argv)
{
    std::vector<int> arr = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};

    leftRotate(arr, 4);
    for (const auto& i: arr) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    rightRotate(arr, 4);
    for (const auto& i: arr) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    std::vector<int> arr2 = {2, 2, 1, 1, 1, 2, 2};

    int result = majorityElement(arr2);
    if (result != -1) {
        std::cout << "Majority Element: " << result << std::endl;
    }
    else {
        std::cout << "No Majority Element found" << std::endl;
    }

    std::vector<int> arr3 = {1,2,3};
    std::vector<int> result2 = majorityElementII(arr3);
    std::cout << "Majority Elements (> n/3): ";
    for (const auto& i: result2) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    std::vector<std::vector<int>> arr4 = {
        {1,2,3},
        {4,5,6},
        {7,8,9}
    };
    d2MatrixPrintDiagonalElements(arr4);

    return (0);
}