


// day month year
// bits fields --> 
/*

0-32 --> bits 2^5 --> 16 8 4 2 1
0-12 --> bits 8 4 2 1  1 1 0 0

*/

#include <bits/stdc++.h>

using Date = struct {
	int mDay: 5;
	int mMonth: 4;
	int mYear: 4;
};


size_t maxProfitFromShare(std::vector<int> &prices) {
	int buy = prices[0];
	int maxProfit = INT_MIN;

	if (prices.size() == 0) return 0;

	for (int i=1;i<prices.size();++i){
		/*maxProfit = std::max(maxProfit, prices[i]-buy);
		buy = std::min(buy, prices[i]);*/
		if (prices[i] < buy) {
			buy = prices[i];
		}
		else {
			maxProfit = std::max(maxProfit, prices[i]-buy);
		}
	}
	return maxProfit;

}

int main() {
	
	Date mObj1;
	std::shared_ptr<Date>mObj2 = std::make_shared<Date>();
	std::cout << sizeof(mObj1) << " " << sizeof(mObj2) << std::endl;
	std::cout << "hailo \n";
	std::vector<int>arr = {7,1,5,3,6,4};
	std::cout << maxProfitFromShare(arr) << std::endl;
	return (0);
}