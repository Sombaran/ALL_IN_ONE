
#include <bits/stdc++.h>


bool searchMatrix(std::vector<std::vector<int>>& matrix, int target) {
    int m = matrix.size();
    int n = matrix[0].size();
    int low = 0, high = m * n - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        int row = mid / n;
        int col = mid % n;
        int curr = matrix[row][col];
        if (curr == target) return true;
        else if (curr > target) high = mid - 1;
        else low = mid + 1;
    }
    return false;
}

int search(std::vector<int>& nums, int target) {
    int low = 0, high = nums.size() - 1;
    while (low <= high) {
        int mid = low + (high - low)/2;
        
        if (nums[mid] == target) return mid;
        // Left half is sorted
        if (nums[low] <= nums[mid]) {
            if (nums[low] <= target && target <= nums[mid]) {
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        } else { // Right half is sorted
            if (nums[mid] <= target && target <= nums[high]) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
    }
    return -1;
}


std::string encode(std::vector<std::string>& strs) {
        std::string res;
        for (const std::string& s : strs) {
            res += std::to_string(s.size()) + "#" + s;
        }
        return res;
    }

std::vector<std::string> decode(std::string s) {
        std::vector<std::string> res;
        int i = 0;
        while (i < s.size()) {
            int j = i;
            while (s[j] != '#') {
                j++;
            }
            int length = std::stoi(s.substr(i, j - i));
            i = j + 1;
            j = i + length;
            res.push_back(s.substr(i, length));
            i = j;
        }
        return res;
    }

int stringToInterger(std::string& value) {

	if (value.empty())  return 0;
	int result{};
	for (const char&i: value) {
		result = ((result * 10) + (i - '0'));
	}
	return (result);
}

int main(int argc, char** argv)
{
    std::vector<int> result {};
    result.reserve(5);
    std::cout << result.capacity() << std::endl;
    for (int i = 1; i <= 10; ++i) {
        result.emplace_back(i * i);
    }
    for (const auto& num : result) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    std::cout << result.capacity() << std::endl;

    std::vector<std::vector<int>> matrix {
        {1, 3, 5, 7},
        {10, 11, 16, 20},
        {23, 30, 34, 60}
    };
    int target = 134;
    if (searchMatrix(matrix, target)) {
        std::cout << "Element found in the matrix" << std::endl;
    } else {
        std::cout << "Element not found in the matrix" << std::endl;
    }
    std::vector<int> nums {4,5,6,7,0,1,2};
    target = 0;
    int index = search(nums, target);
    if (index != -1) {
        std::cout << "Element found at index: " << index << std::endl;
    } else {
        std::cout << "Element not found in the array" << std::endl;
    }
    std::vector <std::string> param {"Hello","World"};
	std::string R1 = encode(param);
	std::cout << "Result:  " << R1 << std::endl;
	std::vector<std::string> R2 {};
	R2 =  decode(R1);
	for (const auto&i: R2) std::cout << i << " " ;
	std::cout << std::endl;
	std::string input = "111";
	std::cout <<"Result " << stringToInterger(input)<< std::endl;

    return (0);
}