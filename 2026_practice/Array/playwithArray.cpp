#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    // Merges nums2 into nums1 in-place in sorted order.
    void mergeWithoutSpace(vector<int>& nums1, int m, vector<int>& nums2, int n) {
        // i points to last valid element in nums1
        int i = m - 1;

        // j points to last element in nums2
        int j = n - 1;

        // k is the last index of nums1 (including 0 placeholders)
        int k = m + n - 1;

        // Fill nums1 from the end by comparing nums1[i] and nums2[j]
        while (i >= 0 && j >= 0) {
            // Place larger of the two at nums1[k]
            if (nums1[i] > nums2[j]) {
                //nums1[k--] = nums1[i--];
				std::swap(nums1[k--], nums1[i--]);
            } else {
                //nums1[k--] = nums2[j--];
				std::swap(nums1[k--], nums2[j--]);
            }
        }

        // If nums2 has remaining elements, copy them
        while (j >= 0) {
           // nums1[k--] = nums2[j--];
			std::swap(nums1[k--],  nums2[j--]);
        }

        // No need to copy remaining nums1 elements, as they are already in place
    }
	
	void mergeWithSpace(vector<int>& arr1, vector<int>& arr2, vector<int>& merged) {
		
		int n = arr1.size();
		int m = arr2.size();
		int i = 0, j = 0;
		
		//size_t newSize = m+n;
		// temporary array to store merged result
		//vector<int> merged.reserve(newSize);

		// merge elements in sorted order
		while (i < n && j < m) {
			if (arr1[i] <= arr2[j]) {
				merged.emplace_back(arr1[i++]);
			} else {
				merged.emplace_back(arr2[j++]);
			}
		}
		
		/*for(const auto&i: merged) {
			
			std::cout << i << " ";
		}
		
		std::cout << std::endl;*/
	}
};

int main() {
    vector<int> nums1 = {1, 3, 5, 0, 0, 0};
    vector<int> nums2 = {2, 4, 6};
    int m = 3, n = 3;

    Solution().mergeWithoutSpace(nums1, m, nums2, n);

    // Print merged array
    for (int num : nums1) cout << num << " ";
	std::cout << std::endl;
	
	vector<int> arr1 = {1, 3, 5, 7};
    vector<int> arr2 = {2, 4, 6, 8};
	size_t newSize = arr1.size()+arr2.size();
	vector<int> merged{};
	merged.reserve(newSize);
	std::cout << merged.size() << " " << newSize << std::endl;
	Solution().mergeWithSpace(arr1, arr2, merged);
	for (const auto& i: merged) std::cout << i << " " ;
	std::cout << std::endl;
	
    return 0;
}
