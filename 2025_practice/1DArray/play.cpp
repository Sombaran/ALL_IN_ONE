#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>

using namespace std;



void printArray( vector<int>& arr) {

    for (int i = 0; i < arr.size(); i++)
    {
        if (arr[i] % 2 == 0)
        {
            arr[i] = arr[i] * 2; // 4 8 12 16 20
        }
        cout << arr[i] << " ";
    }
    cout << endl;
}

void waveArrayUnsorted(vector<int>& arr) {
    for (int i = 0; i < arr.size(); i += 2) {
        // Ensure arr[i] >= arr[i-1]
        if (i > 0 && arr[i] < arr[i-1]) {
            std::swap(arr[i], arr[i-1]);
        }
        // Ensure arr[i] >= arr[i+1]
        if (i < arr.size()-1 && arr[i] < arr[i+1]) {
            std::swap(arr[i], arr[i+1]);
        }
    }
}
int  findDuplicates(std::vector<int>&arr) {
    int xorAll = 0;
    int result = 0;
    for (int i = 0; i < arr.size(); i++) {
        xorAll ^= arr[i];
        result = arr[i];

    }
    return result;
}

void rotateLeft(std::vector<int>& arr, int& rotateBy) {
    // To be implemented
    // 1, 2, 3, 4, 5 rotateBy = 2
    // O/P -> 3, 4, 5, 1, 2
    std::reverse(arr.begin(), arr.begin() + rotateBy);
    std::reverse(arr.begin() + rotateBy, arr.end());
    std::reverse(arr.begin(), arr.end());
}

void rotateRight(std::vector<int>& arr, int& rotateBy) {
    // To be implemented
    // 3, 4, 5, 1, 2 rotateBy = 2
    // O/P -> 1, 2, 3, 4, 5
    int n = arr.size();
    //rotateBy = rotateBy % n; // In case rotateBy is greater than array size
    std::reverse(arr.begin(), arr.end()); // 1 2 5 4 3
    std::reverse(arr.begin(), arr.begin() + rotateBy); //2 1 5 4 3
    std::reverse(arr.begin() + rotateBy, arr.end()); //1 2 3 4 5
}


void printVector(const std::vector<int>& vec) {
    for (const int& val : vec) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
}

int removeDuplicateInPlace(std::vector<int>& arr) {
    int  j = 1;
    for (int i = 1; i < arr.size(); i++) { 
        if(arr[i] != arr[i-1]) { // 1 ~= 1
            arr[j++] = arr[i];   // 
        }
    }
    return j;
}

void reverseArray(std::vector<int>& arr, int start, int end) {
    while (start < end) {
        std::swap(arr[start++], arr[end--]);
    }
}


int missingNumber(std::vector<int>& arr) {
    int n = arr.size()+1;
    int expected =  (n *1LL* (n + 1)) / 2;
    std::cout << "Expected Sum: " << expected << std::endl;
    int total = 0;
    for (int num=0; num < arr.size(); num++) {
        total += arr[num];
    }
    return (expected - total); // The missing number
}

int main(int argc, char** argv)
{
    vector<int> arr1 = {10,9,1,3,5,7,2,4,6,8};

    printArray(arr1);
    waveArrayUnsorted(arr1);
    printVector(arr1);
    vector<int> arr2 = {2, -1, 1, 2, 3};
    int detectedDuplicate = findDuplicates(arr2);
    cout << "Detected Duplicate: " << detectedDuplicate << endl;
    std::vector<int> arr3 = {1, 2, 3, 4, 5};
    int rotateBy = 2;
    rotateLeft(arr3, rotateBy);
    printVector(arr3);
    rotateRight(arr3, rotateBy);
    printVector(arr3);

    std::vector<int> arr4 = {1, 1, 2};
    int newSize = removeDuplicateInPlace(arr4);
    for (int i = 0; i < newSize; i++){
        cout << arr4[i] << " ";
    }
    cout << endl;
    std::vector<int> arr5 = {0,1,2,3,4,5,6,7,8,9};

    reverseArray(arr5, 0, arr5.size() - 1);
    printVector(arr5);

    std::vector<int> arr6 = {8, 2, 4, 5, 3, 7, 1};
    int result = missingNumber(arr6);
    cout << "Missing Number: " << result << endl;
    return (0);
}