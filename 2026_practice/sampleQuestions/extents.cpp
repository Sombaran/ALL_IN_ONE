
/**
 * @attention: The first thought is brute force
 * Total 3 functions used 
 * function 1 : This function is reading points from numbers.txt
 * function 2 : This function is reading ranges from extents.txt
 * function 3 : This function takes a list of points and a list of ranges, and for each point it counts how many ranges contain  * it. It does this with two nested loops: the outer loop goes through each point, and the inner loop checks every range to see   * if the point lies between the range’s start and end. The count is then printed. It’s a straightforward brute‑force solution   * with time complexity O(P\cdot R), which is fine for small inputs but not scalable for very large datasets
 * When I excute this code it took 65 seconds approx to complete the processing
 * Executed using g++ extents.cpp -o 2c -std=c++17 ;./2c


#include <bits/stdc++.h>
using namespace std;


void processCountRangesAndPoints(const std::vector<int>& points,
                                 const std::vector<std::pair<int, int>>& ranges) {
    for (int point : points) {
        int count = 0;
        for (const auto& range : ranges) {
            if (range.first <= point && point <= range.second) {
                count++;
            }
        }
        std:cout << count << '\n';  // goes to output.txt
    }
}

std::vector<int> readPoints(ifstream& numbersFileReceived) {
    if (!numbersFileReceived.is_open()) return {};

    std::vector<int> points{};
    int p{};
    while (numbersFileReceived >> p) {
        points.emplace_back(p);
    }
    return points;
}

std::vector<std::pair<int, int>> readRanges(ifstream& extentsFileReceived) {
    if (!extentsFileReceived.is_open()) return {};

    std::vector<std::pair<int, int>> resultReturn{};
    int x{}, y{};
    while (extentsFileReceived >> x >> y) {
        resultReturn.emplace_back(x, y);
    }
    return resultReturn;
}

int main1(int argc, char** argv) {
    auto start = chrono::high_resolution_clock::now();

    ifstream extentsFile("extents.txt");
    ifstream numbersFile("numbers.txt");

    // Read ranges and points
    std::vector<std::pair<int,int>> ranges = readRanges(extentsFile);
    std::vector<int> points = readPoints(numbersFile);

    // Process
    processCountRangesAndPoints(points, ranges);
    
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::seconds>(end - start);
    cout << "Execution time: " << duration.count() << " seconds" << endl;
    return (0);
}
*/
#include <bits/stdc++.h>
/**
 * @description: The second approach is using prefix sum pattern as it transform the ranges to boundary event values, which can 
 * be easily searched using binary search.
 * Time complexity :
 * sort for events range: (e log e)
 * binary search and prefix checkup: (p log e)
 * This approach executed within a seconds which is much better than the brute force approach which took 65 seconds.
 * Executed using g++ extents.cpp -o 2c -std=c++17 ;./2c
 */
class Solution {
public:
    void run() {
        // 1: Read ranges and build events
        std::vector<std::pair<int, int>> events{};
        {
            std::ifstream fin("extents.txt"); // loading extents.txt file
            
            if (!fin.is_open()) return; // checks  If the file exists and can be opened


            int a{},b{};
            while(fin >> a >> b) {               // read into a & b from extents.txt
                events.emplace_back(a, +1);      // range starts
                events.emplace_back(b+1, -1);    // range ends
            }
        }

        // 2: Sort events by coordinate 
        {
            std::sort(events.begin(), events.end());
        }

        /**
        Dry run till here
        Sample ranges: [2,5],[4,7]
        Event vector : (2,+1), (6,-1), (4,+1), (8,-1)
        Sort event vector: (2,+1),(4,+1),(6,-1),(8,-1)
        */

        // 3: Build prefixSum sums of active ranges
        std::vector<int> coordinates{};
        std::vector<int> prefixSum{};
        {
            int active = 0;
            for(const auto &e : events) {
                coordinates.push_back(e.first);
                active += e.second;
                prefixSum.push_back(active);
            }
        }
        /**
        Dry run till here
        At 2: active = 0 + 1 = 1
        At 4: active = 1 + 1 = 2
        At 6: active = 2 - 1 = 1
        At 8: active = 1 - 1 = 0
        So:
        coordinates vector = [2,4,6,8]
        prefixSum vector = [1,2,1,0]

        */

        // 4: Process points one by one
        {
            std::ifstream fin("numbers.txt");   // opens the file containing the list of points
            if (!fin.is_open()) return; // checks  If the file exists and can be opened
            int p{};                            
            while(fin >> p) {                   // streams each integer point from the file into p
                auto it = std::upper_bound(coordinates.begin(), coordinates.end(), p); // finds the first event coordinate greater than p.

                if(it == coordinates.begin()) {     // means there are no events before or at p, so coverage is zero.
                    std::cout << 0 << "\n";
                } else {
                    size_t idx = (it - coordinates.begin());  // holda number of active ranges at that event
                    std::cout << prefixSum[idx-1] << "\n";
                }
            }
        }
    }
};

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    std::ios::sync_with_stdio(false);       // faster C++ streams
    std::cin.tie(nullptr);                  // prevents unnecessary flushes, faster input/output
    std::unique_ptr<Solution> obj = std::make_unique<Solution>();
    obj -> run();
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    //cout << "Execution time: " << duration.count() << " seconds" << endl;
    return 0;
}
