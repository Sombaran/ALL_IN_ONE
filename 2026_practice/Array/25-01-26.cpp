
#include <bits/stdc++.h>


/**
 * @brief: Climbing Stairs Problem
 * You are climbing a staircase. It takes n steps to reach the top.
 * Each time you can either climb 1 or 2 steps. In how many distinct ways can you climb to the top?
 * @attention This is a classic Dynamic Programming problem that can be solved using bottom-up approach.
 * The idea is to use an array to store the number of ways to reach each step, and build up the solution from the base cases.
 * The number of ways to reach step i is the sum of the ways to reach step i-1 and step i-2.
 * Time Complexity: O(n)
 * Space Complexity: O(n)
 * @param n: number of steps
 * @return int: number of distinct ways to climb to the top
 */


int climbStarirs(int n) {
    if (n <= 1) return 1;

    std::vector<int> dp(n + 1, 0);
    dp[0] = 1;
    dp[1] = 1;

    for (int i = 2; i <= n; ++i) {
        dp[i] = dp[i - 1] + dp[i - 2];
    }
    return dp[n];
}

/**
 * @brief: Robber Problem
 * You are a professional robber planning to rob houses along a street. Each house has a certain amount of money stashed,
 * the only constraint stopping you from robbing each of them is that adjacent houses have security systems connected and it will automatically contact the police  if two adjacent houses were broken into on the same night.
 * Given an integer array nums representing the amount of money of each house, return the maximum amount of money you can rob tonight without alerting the police.
 * @attention Can be solved using DP or using previous two variables to store max loot till previous and loot till previous to previous house.
 * Time Complexity: O(n)
 * Space Complexity: O(n)
 * @param nums: vector of integers representing amount of money in each house
 * @return int: maximum amount of money that can be robbed without alerting the police
 */
int robHouse(std::vector<int>& nums) {
    if (nums.size() == 0) return 0;
    if (nums.size() == 1) return nums[0];

    std::vector<int> dp(nums.size(), 0);
    dp[0] = nums[0];
    dp[1] = std::max(nums[0], nums[1]);

    for (int i = 2; i < nums.size(); ++i) {
        dp[i] = std::max(dp[i - 1], dp[i - 2] + nums[i]);
    }
    return dp.back();
}

/**
 * 
 * @brief: Coin Change Problem
 * You are given coins of different denominations and a total amount of money amount. Write a function to compute the fewest number of coins that you need to make up that amount. If that amount of money cannot be made up by any combination of the coins, return -1.
 * @attention This is a classic Dynamic Programming problem that can be solved using bottom-up approach.
 * The idea is to use an array to store the minimum number of coins needed to make up each amount from 0 to amount.
 * Time Complexity: O(n * m) where n is the amount and m is the number of coins
 * Space Complexity: O(n)
 * @param coins: vector of integers representing the denominations of the coins
 * @param amount: total amount of money
 * @return int: fewest number of coins needed to make up that amount
 */
int coinChangeWays(std::vector<int>& coins, int amount) {
    std::vector<int> dp(amount + 1, 1e9);
    dp[0] = 0; // One way to make amount 0

    for (int i=1; i <= amount; ++i) {
        for (int j=0; j < coins.size(); ++j) {
            if (coins[j] <= i) {
                dp[i] = std::min(dp[i], dp[i - coins[j]] + 1);
            }
        }
    }
    return dp[amount];
}


int main(int argc, char** argv)
{
    int n = 5;
    std::cout << "Number of ways to climb " << n << " stairs is: " << climbStarirs(n) << std::endl;

    std::vector<int> nums {1, 2, 3, 1};

    std::cout << "Maximum amount that can be robbed is: " << robHouse(nums) << std::endl;

    std::vector<int> coins {1, 2, 5};
    int amount = 11;
    std::cout << "Minimum coins to make amount " << amount << " is: " << coinChangeWays(coins, amount) << std::endl;
    return (0);
}


/**
 * @brief: Additional DP problems can be added here
 * 2D Matrix Directional DP
 * Maximum Sum in Sliding Window
 * Counting Bits
 */