#include <bits/stdc++.h>
using namespace std;

constexpr size_t zerothIdx 	{0};
constexpr size_t firstIdx 	{1};	

vector<vector<int>> mergeOverlapWithLognComplexity(vector<vector<int>>& arr, std::vector<int>& newInterval ) {

	arr.push_back(newInterval);
    // Sort intervals based on start values
    sort(arr.begin(), arr.end());
  
    vector<vector<int>> res;
    res.push_back(arr[0]);
	//res.push_back(arr[1]);
    //res.push_back(arr[2]);
    //res.push_back(arr[3]);

	

    for (int i = 1; i < arr.size(); i++) {
        vector<int>& last = res.back();
        vector<int>& curr = arr[i];
		//std::cout << last[i] << " ";
		//std::cout << curr[i] << " ";
        // If current interval overlaps with the last merged
        // interval, merge them 
        if (curr[zerothIdx] <= last[firstIdx]) 
            last[firstIdx] = max(last[firstIdx], curr[firstIdx]);
        else 
            res.push_back(curr);
    }

    return res;
}

vector<vector<int>> mergeOverlapWithLinearComplexity(vector<vector<int>>& arr, std::vector<int>& newInterval ) {
	vector<vector<int>> out{};
	int i = 0;
    int n = arr.size();
	// Add all intervals that come before the new interval
	
	while (i<n and arr[i][firstIdx] and newInterval[zerothIdx]) {
		out.emplace_back(arr[i++]);
	}
	// Merge all overlapping intervals with the new interval
	while (i<n and arr[i][zerothIdx] and newInterval[firstIdx]) {
		newInterval[zerothIdx] = std::min(newInterval[zerothIdx], arr[i][zerothIdx]);
		newInterval[firstIdx]  = std::max(newInterval[firstIdx], arr[i][firstIdx]);
		++i;		
	}
	out.emplace_back(newInterval);
	// Add all the remaining intervals
	while (i < n) {
		out.push_back(arr[i++]);
    }
	return out;
}


int main() {
    vector<vector<int>> arr = {{7, 8}, {1, 5}, {2, 4}, {4, 6}};
	std::vector<int> newInterval = {3,7};
    vector<vector<int>> res = mergeOverlapWithLognComplexity(arr, newInterval);

  	for (vector<int>& interval: res) 
        cout << interval[0] << " " << interval[1] << endl;
	
	vector<vector<int>> intervals = {{1, 3}, {4, 5}, {6, 7}, {8, 10}};
	vector<vector<int>> out = mergeOverlapWithLinearComplexity(intervals, newInterval);
	//for (vector<int>& interval: out) 
    //    cout << interval[0] << " " << interval[1] << endl;
    return 0;
}