/**
 * @file 1.cpp
 * @author Sombaran Gupta
 * @brief : 2D Array
 * @version 0.1
 * @date 2025-11-29
 * @details : Techniques used
- Traversal patterns: row-wise, column-wise, diagonal, spiral.
- Transformations: transpose, rotate, reverse.
- Searching: binary search in sorted matrix, hash-based lookups.
- Dynamic Programming: submatrix sums, pathfinding.
- Graph Traversal: treating matrix as a grid graph (neighbors up/down/left/right).
 */

#include <iostream>
#include <functional>
#include <vector>

using namespace std;


/*

*
* *
* * *
* * * *
* * * * *

*/

std::function<void()> patternStar1 = [] {
    for (int i = 1; i <= 5; i++)
    {
        for (int j = 1; j <= i; j++)
        {
            cout << "* ";           
        }
        cout << endl;
    }
};

/**
1
2 2
3 3 3
4 4 4 4
5 5 5 5 5
 */
std::function<void()> patternNumber1 = [] {
    for (int i = 1; i <= 5; i++)
    {
        for (int j = 1; j <= i; j++)
        {
            cout << i << " ";
        }
        cout << endl;
    }
};

/*
1
1 2
1 2 3
1 2 3 4
1 2 3 4 5
*/

std::function<void()> patternNumber2 = [] {
    for (int i = 1; i <= 5; i++)
    {
        for (int j = 1; j <= i; j++)
        {
            cout << j << " ";
        }
        cout << endl;
    }
};


/*
A
B B
C C C
D D D D
E E E E E
*/

std::function<void()> patternAlphabet1 = [] {
    for (char i = 'A'; i <= 'E'; i++)
    {
        for (char j = 'A'; j <= i; j++)
        {
            cout << i << " ";
        }
        cout << endl;
    }
};

/* 
A
A B
A B C
A B C D
A B C D E
*/

std::function<void()> patternAlphabet2 = [] {
    for (char i = 'A'; i <= 'E'; i++)
    {
        for (char j = 'A'; j <= i; j++)
        {
            cout << j << " ";
        }
        cout << endl;
    }
};

/*
         * 
       * * 
     * * * 
   * * * * 
 * * * * * 

*/

std::function<void()>patternLeftHalfPattern = [] {
    for (int i = 0; i < 5; i++) {
        // Print leading spaces
        for (int j = 0; j < 5 - i; j++) {
            cout << "  ";  // Two spaces for alignment
        }
        // Print stars
        for (int k = 0; k <= i; k++) {
            cout << "* ";
        }

        cout << endl;
    }
};

/**
vector size 4
2
0 -> 1 2
4
1 -> 5 6 7 8
1
2 -> 9
3

*/

std::function<void()> pattern2DMatrix = [] {
    std::vector<std::vector<int>> vec = {{1, 2}, {5, 6, 7, 8}, {9},  {9, 8, 11}};

    // Display the jagged array
    std::cout << "vector size " << vec.size() << std::endl;
    for (int i = 0; i < vec.size(); i++)
    {
        std::cout << vec[i].size() << std::endl;
        std::cout << i << " -> ";
        for (int j = 0; j < vec[i].size(); j++)
        {
            std::cout << vec[i][j] << " ";
        }
        std::cout << std::endl;
    }
};

/***

1 2 0 0
5 6 7 8
9 0 0 0
9 8 11 0

*/

std::function<void()> pattern2DMatrix2 = [] {
    std::vector<std::vector<int>> vec = {{1, 2}, {5, 6, 7, 8}, {9},  {9, 8, 11}};
    for (int i=0; i<vec.size(); i++)
    {
        for (int j=0; j<vec.size(); j++)
        {
            std::cout << vec[i][j] << " ";
        }
        std::cout << std::endl;
    }  
};

/**
1 2 3 4 8 12 16 15 14 13 9 5 6 7 11 10
*/

std::function<std::vector<int>(std::vector<std::vector<int>>& )> spiralOrder 
    = [](std::vector<std::vector<int>>& matrixReceivedFromMain) {
    //print2DMatrix(matrixReceivedFromMain);
    std::cout << __func__ << std::endl;
    const int m = static_cast<int>(matrixReceivedFromMain.size());
    /*if (m == 0) {
        return;
    }*/
    const int n = static_cast<int>(matrixReceivedFromMain[0].size());
    std::vector<int> output {};
    output.reserve(static_cast<size_t>(m) * n);

    int top = 0, bottom = m-1;
    int left = 0, right = n-1;
    while (top <= bottom && left <= right) {
        // Top row: left -> right
        for (int i = left; i <= right; i++) {
            output.emplace_back(matrixReceivedFromMain[top][i]);
        }
        ++top;
        // Right column: top -> bottom
        for( int i = top; i <= bottom; i++) {
            output.emplace_back(matrixReceivedFromMain[i][right]);
        }
        --right;
        // Bottom row: right -> left (if still valid)
        if (top <= bottom) {
            for (int i = right; i >= left; i--) {
                output.emplace_back(matrixReceivedFromMain[bottom][i]);
            }
            --bottom;
        }
        // Left column: bottom -> top (if still valid)
        if (left <= right) {
            for (int i = bottom; i >= top; i--) {
                output.emplace_back(matrixReceivedFromMain[i][left]);
            }
            ++left;
        }
    }
    return output;
};

auto print2DMatrix = [] (const std::vector<std::vector<int>>& matrixReceivedFromMain) {
    std::cout << std::endl;
    std::cout << __func__ << std::endl;
    for(const std::vector<int>& row : matrixReceivedFromMain) {
        for(const int& val : row) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
};

/**
1 2 3 4 8 7 6 5 9 10 11 12 16 15 14 13
*/
std::function<void(std::vector<std::vector<int>>& )> zigZagOrder 
    = [] (std::vector<std::vector<int>>& matrixReceivedFromMain) {
        std::cout << __func__ << std::endl;
        const int row = static_cast<int>(matrixReceivedFromMain.size());
    if (row == 0) {
        return;
    }
    for (int i = 0; i < row; i++) {
        if (i % 2 == 0 ) {
            for (int j=0;j<matrixReceivedFromMain[i].size();j++) {
                std::cout << matrixReceivedFromMain[i][j] << " ";
            }
        } else {
            for (int j=matrixReceivedFromMain[i].size()-1;j>=0;j--) {
                std::cout << matrixReceivedFromMain[i][j] << " ";
            }
        }
    }
    std::cout << std::endl;
};

/**
1 5 9 13
2 6 10 14
3 7 11 15
4 8 12 16
 */

std::function<void(std::vector<std::vector<int>>& )> columnWiseTraversal 
    = [] (std::vector<std::vector<int>>& matrixReceivedFromMain) {
        std::cout << __func__ << std::endl;
        const int row = static_cast<int>(matrixReceivedFromMain.size());
    if (row == 0) {
        return;
    }
    const int col = static_cast<int>(matrixReceivedFromMain[0].size());
    /*for (int j = 0; j < col; j++) {
        for (int i=0;i<row;i++) {
            std::cout << matrixReceivedFromMain[i][j] << " ";
        }
        std::cout << std::endl;
    }*/
    for (int i=0;i<row;i++) {
        for (int j = 0; j < row; j++) {
            std::cout << matrixReceivedFromMain[j][i] << " ";
        }
        std::cout << std::endl;
    }
};


/**

 */

std::function<int(std::vector<std::vector<int>>& )> printDiagonalElements = [] (std::vector<std::vector<int>>& matrixReceivedFromMain) {
    std::cout << __func__ << std::endl;
    const int row = static_cast<int>(matrixReceivedFromMain.size());
    if (row == 0) {
        return 0;
    }
    int firstTraceSum = 0;
    for (int i=0;i<row;i++) {
        for (int j=0;j<row;j++) {
            if (i == j) {
                firstTraceSum += matrixReceivedFromMain[i][j];
                std::cout << matrixReceivedFromMain[i][j] << " ";
            }
        }
    }
    return firstTraceSum;
};

/**
4 7 10 13 Second Trace: 34
 */
std::function<int(std::vector<std::vector<int>>& )> calculateSecondTrace = [] (std::vector<std::vector<int>>& matrixReceivedFromMain) {
    std::cout << __func__ << std::endl;
    const int row = static_cast<int>(matrixReceivedFromMain.size());
    if (row == 0) {
        return 0;
    }
    int secondTraceSum = 0;
    for (int i = 0; i < matrixReceivedFromMain.size(); ++i) {
        // For the anti-diagonal, the sum of row and column indices is (n - 1)
        //secondTraceSum += matrix[i][matrix.size() - 1 - i];
        for (int j = 0; j < matrixReceivedFromMain.size(); ++j) {
            if (i + j == matrixReceivedFromMain.size() - 1) {
                std::cout << matrixReceivedFromMain[i][j] << " ";
                secondTraceSum += matrixReceivedFromMain[i][j];
            }
        }
    }
    return secondTraceSum;
};

int main(int argc, char** argv)
{
    /**
     * @brief: Right Half Pyramid Pattern
     */
    patternStar1();
    patternNumber1();
    patternNumber2();
    patternAlphabet1();
    patternAlphabet2();
    /**
     * @brief:Left Half Pyramid Pattern
     */
    patternLeftHalfPattern();

    /**
     * @brief:2D Matrix using vector
     */
    pattern2DMatrix();
    pattern2DMatrix2();    

    /**
     * @brief:Easy level matrix question
     */
    std::vector<std::vector<int>> matrix = {
        { 1, 2, 3, 4 },
        { 5, 6, 7, 8 },
        { 9, 10, 11, 12 },
        { 13, 14, 15, 16 }
    };

    std::vector<int> spiralOrderOutput = spiralOrder(matrix);
    //print2DMatrix(matrix);
    //turnImage90();
    for (const auto& i: spiralOrderOutput) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    zigZagOrder(matrix);
    /**
     * @brief: Column wise traversal
    1 5 9 13
    2 6 10 14
    3 7 11 15
    4 8 12 16
     */
    
    columnWiseTraversal(matrix);
    int firstTrace = printDiagonalElements(matrix);
    std::cout << "First Trace: " << firstTrace << std::endl;
    int secondTrace = calculateSecondTrace(matrix);
    std::cout << "Second Trace: " << secondTrace << std::endl;
    return (0);
}