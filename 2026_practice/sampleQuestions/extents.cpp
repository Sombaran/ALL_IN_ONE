
/**
 * @description: The first thought is brute force
 * Total 3 functions used 
 * function 1 : This function is reading points from numbers.txt
 * function 2 : This function is reading ranges from extents.txt
 * function 3 : This function takes a list of points and a list of ranges, and for each point it counts how many ranges contain  * it. It does this with two nested loops: the outer loop goes through each point, and the inner loop checks every range to see   * if the point lies between the range’s start and end. The count is then printed. It’s a straightforward brute‑force solution   * with time complexity O(P\cdot R), which is fine for small inputs but not scalable for very large datasets
 * When I excute this code it took 65 seconds approx to complete the processing
 


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

/**
 * @brief: The second approach is using prefix sum pattern as it transform the ranges to boundary event values, which can be 
 * easily searched using binary search 
 */
class RangeCounter {
public:
    void run() {
        // Step 1: Read ranges and build events
        vector<pair<unsigned long long,int>> events;
        {
            ifstream fin("extents.txt");
            unsigned long long a,b;
            while(fin >> a >> b) {
                events.emplace_back(a, +1);      // range starts
                events.emplace_back(b+1, -1);    // range ends
            }
        }

        // Step 2: Sort events by coordinate
        sort(events.begin(), events.end());

        // Step 3: Build prefix sums of active ranges
        vector<unsigned long long> coords;
        vector<long long> prefix;
        long long active = 0;
        for(auto &e : events) {
            coords.push_back(e.first);
            active += e.second;
            prefix.push_back(active);
        }

        // Step 4: Process points one by one
        {
            ifstream fin("numbers.txt");
            unsigned long long p;
            while(fin >> p) {
                // Find last event <= p
                auto it = upper_bound(coords.begin(), coords.end(), p);
                if(it == coords.begin()) {
                    cout << 0 << "\n";
                } else {
                    size_t idx = (it - coords.begin()) - 1;
                    cout << prefix[idx] << "\n";
                }
            }
        }
    }
};

int main() {
    auto start = chrono::high_resolution_clock::now();
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    RangeCounter().run();
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::seconds>(end - start);
    //cout << "Execution time: " << duration.count() << " seconds" << endl;
    return 0;
}
