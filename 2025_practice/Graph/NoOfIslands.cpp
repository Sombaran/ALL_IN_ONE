/**
 * @brief Given a 2D grid map of '1's (land) and '0's (water), count the number of islands.
 * An island is surrounded by water and is formed by connecting adjacent lands horizontally or vertically.
 * You may assume all four edges of the grid are all surrounded by water.
 * @example
 * Input:
 * 11110
 * 11010
 * 11000
 * 00000
 * Output: 1
 * Input:
 * 11000
 * 11000
 * 00100
 * 00011
 * Output: 3
 * @note This implementation uses Depth-First Search (DFS) to explore and mark visited land cells.
 */

#include <vector>
#include <iostream>

class Solution {
    void dfs(std::vector<std::vector<bool>>& visitedArray,
        std::vector<std::vector<char>>& grid, int i, int j) {
        // Boundary check
        if (i < 0 || j < 0 || i >= grid.size() || j >= grid[0].size() || grid[i][j] == '0' ||
    visitedArray[i][j]){
        return;
    }
        
        // Mark current cell as visited
        //grid[i][j] = '0';
        visitedArray[i][j] = true;
        // Explore neighbors (up, down, left, right)
        dfs(visitedArray, grid, i + 1, j);
        dfs(visitedArray, grid, i - 1, j);
        dfs(visitedArray, grid, i, j + 1);
        dfs(visitedArray, grid, i, j - 1);
    }

public:
    int numIslands(std::vector<std::vector<char>>& grid) {
        if (grid.empty()) {
            return 0;
        }
        int count = 0;
        std::vector<std::vector<bool>> visitedArray(
            grid.size(), std::vector<bool>(grid[0].size(), false));
        for (int i = 0; i < grid.size(); i++) {
            for (int j = 0; j < grid[0].size(); j++) {
                if ((grid[i][j] == '1')and (!visitedArray[i][j])) {
                    dfs(visitedArray, grid, i, j);  // sink this island
                    count++;
                }
            }
        }
        return count;
    }
};

int main() {
    std::vector<std::vector<char>> grid = {
        {'1','1','0','0','0'},
        {'1','1','0','0','0'},
        {'0','0','1','0','0'},
        {'0','0','0','1','1'}
    };

    Solution sol;
    std::cout << "Number of islands: " << sol.numIslands(grid) << std::endl;
    return 0;
}