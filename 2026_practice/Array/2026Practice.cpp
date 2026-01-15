


 #include<bits/stdc++.h> 
 
 
 std::vector<std::vector<std::string>> anagrams (std::vector<std::string>& receivedArray) {
	 
	 std::vector<std::vector<std::string>> output {};
	 std::unordered_map<std::string, std::vector<std::string>> mp{};
	 
	 for (const auto& i: receivedArray) {
		 
		 //std::cout << i << " ";
		 std::string someString = i;
		 
		 std::sort(someString.begin(), someString.end());
		 std::cout << someString << " ";
		 
		 mp[someString].push_back(i);
	 }
	 
	 std::cout << std::endl;
	 
	 
	 for (const auto&i: mp) {
		 
		 output.emplace_back(i.second);
	 }
	 return {output};
 }
 
 
 
 int main(int argc, char** argv) {
	std::vector<std::string> arr = {"act", "god", "cat", "dog", "tac"};
    
    std::vector<std::vector<std::string>> res = anagrams(arr);
    for(int i = 0; i < res.size(); i++) {
    	for(int j = 0; j < res[i].size(); j++)
            std::cout << res[i][j] << " ";
        std::cout << "\n";
    }
	return (0);
 }