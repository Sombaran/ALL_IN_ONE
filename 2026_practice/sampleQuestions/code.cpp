#include <bits/stdc++.h>

// Strict binary comparison (size + bytes)
bool compareFilesBinary(const std::string& file1, const std::string& file2) {
    std::ifstream f1(file1, std::ios::binary);
    std::ifstream f2(file2, std::ios::binary);

    if (!f1.is_open() || !f2.is_open()) {
        std::cerr << "Error: Could not open one of the files." << std::endl;
        return false;
    }

    return std::equal(std::istreambuf_iterator<char>(f1),
                      std::istreambuf_iterator<char>(),
                      std::istreambuf_iterator<char>(f2),
                      std::istreambuf_iterator<char>());
}
/*
// Content comparison ignoring line endings
bool compareFilesContent(const std::string& file1, const std::string& file2) {
    std::ifstream f1(file1);
    std::ifstream f2(file2);

    if (!f1.is_open() || !f2.is_open()) {
        std::cerr << "Error: Could not open one of the files." << std::endl;
        return false;
    }

    std::string line1, line2;
    while (true) {
        bool ok1 = static_cast<bool>(std::getline(f1, line1));
        bool ok2 = static_cast<bool>(std::getline(f2, line2));
        if (!ok1 && !ok2) break; // both ended
        if (line1 != line2) return false;
    }
    return true;
}*/

// Helper: normalize line endings and trim trailing spaces
static inline void normalize(std::string& s) {
    if (!s.empty() && s.back() == '\r') s.pop_back(); // remove CR from Windows line endings
    // Optional: trim trailing spaces/tabs
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         [](unsigned char ch) { return !std::isspace(ch); }).base(),
            s.end());
}



bool compareFilesContent(const std::string& file1, const std::string& file2) {
    std::ifstream f1(file1);
    std::ifstream f2(file2);

    if (!f1 || !f2) {
        std::cerr << "Error: Could not open one of the files." << std::endl;
        return false;
    }

    std::string line1, line2;
    int lineNumber = 1;
    bool identical = true;

    while (true) {
        bool ok1 = static_cast<bool>(std::getline(f1, line1));
        bool ok2 = static_cast<bool>(std::getline(f2, line2));

        if (!ok1 && !ok2) break; // both ended

        normalize(line1);
        normalize(line2);

        if (line1 != line2) {
            std::cout << "Difference at line " << lineNumber << ":\n";
            std::cout << "File1: \"" << line1 << "\"\n";
            std::cout << "File2: \"" << line2 << "\"\n\n";
            identical = false;
        }
        lineNumber++;
    }

    if (identical) {
        std::cout << "Files have the same content (ignoring line endings/whitespace)." << std::endl;
    }
    return identical;
}
 

// Process points against ranges
void processCountRangesAndPoints(const std::vector<int>& points,
                                const std::vector<std::pair<int, int>>& ranges) {
    for (int point : points) {
        int count = 0;
        for (const auto& range : ranges) {
            if (range.first <= point && point <= range.second) {
                count++;
            }
        }
        std::cout << count << '\n';  // goes to output.txt
    }
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    std::ifstream extentsFile("/mnt/c/Users/ritup/Downloads/bigtest/extents.txt");
    std::ifstream numbersFile("/mnt/c/Users/ritup/Downloads/bigtest/numbers.txt");

    if (!extentsFile.is_open() || !numbersFile.is_open()) {
        std::cerr << "Error: Could not open input files." << std::endl;
        return 1;
    }

    // Open output file (truncate to clear previous content)
    std::ofstream outFile("/mnt/c/Users/ritup/Downloads/bigtest/output.txt", std::ios::trunc);
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open output.txt" << std::endl;
        return 1;
    }

    // Redirect cout to file
    std::streambuf* coutBuffer = std::cout.rdbuf();   // save old buffer
    std::cout.rdbuf(outFile.rdbuf());                 // redirect to file

    // Read ranges
    std::vector<std::pair<int, int>> ranges;
    int x, y;
    while (extentsFile >> x >> y) {
        ranges.emplace_back(x, y);
    }

    // Read points
    std::vector<int> points;
    int p;
    while (numbersFile >> p) {
        points.push_back(p);
    }

    // Process (counts go to output.txt)
    processCountRangesAndPoints(points, ranges);

    // Restore cout back to console
    std::cout.rdbuf(coutBuffer);

    std::cout << "Results written to output.txt" << std::endl;

    // Compare output with expected
    std::string outputFile = "/mnt/c/Users/ritup/Downloads/bigtest/output.txt";
    std::string expectedFile = "/mnt/c/Users/ritup/Downloads/bigtest/expected.txt";

    bool identicalBinary = compareFilesBinary(outputFile, expectedFile);
    bool identicalContent = compareFilesContent(outputFile, expectedFile);

    if (identicalBinary) {
        std::cout << "Files are identical (binary check)." << std::endl;
    } else if (identicalContent) {
        std::cout << "Files differ in size/bytes but content is the same (line-by-line check)." << std::endl;
    } else {
        std::cout << "Files differ in content." << std::endl;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "Execution time: " << duration.count() << " seconds" << std::endl;
    return 0;
}