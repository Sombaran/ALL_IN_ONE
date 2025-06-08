/**
* @Brief: Used map to calculate charecter frequency of a string
*/


#include <iostream>
#include <map>
#include <array>


int main() {
	std::array <int, 5>aObj {{1,1,2,2,4}};
	std::map <int, int> frequnecyCount {};

	for (int i=0;i<aObj.size();++i) {
		frequnecyCount[aObj[i]]++;
	}

	for (const auto& i:frequnecyCount) {
		std::cout << i.first << " " << i.second << std::endl; 
	}
	std::cout << std::endl;
	return (0);
}
