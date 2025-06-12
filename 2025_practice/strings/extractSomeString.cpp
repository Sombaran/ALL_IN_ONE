// Online C++ compiler to run C++ program online
#include <iostream>
#include <string>
#include <cstring>

const char*  remove(const char* const input, const char* const strToRemove) {
    std::string inputString = input;
    std::string stringToRemove = strToRemove;
    
    int lengthOfString =  strlen(input);

    int startPosition = inputString.find(stringToRemove);
    int endPostion =  lengthOfString - startPosition;
    std::cout << startPosition << " " << lengthOfString << " "<< endPostion << std::endl;
    std::string remainingString = inputString.substr(0, startPosition); // Som 
    std::cout << remainingString  << std::endl;

    const char* output = remainingString.c_str();
    return (output);
}


int main() {
    const char* const inputString = "Sombaran";
    const char* const extractString = "baran";
    const char* result = remove(inputString, extractString);
    std::cout << result << std::endl;
    return 0;
}