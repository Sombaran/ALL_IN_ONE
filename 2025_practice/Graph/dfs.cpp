/**
 * @brief: Depth First Search (DFS) implementation in C++
 * @date: 2025-10-10
 * @details: This code demonstrates the DFS algorithm on a graph represented using an adjacency list.
 *           It includes functions to add edges and perform DFS traversal starting from a given node.
 *          The graph can be either directed or undirected based on the edge addition.
 *         The DFS traversal order is printed to the console.
 * @note: This implementation uses C++17 standard features.
 */

#include<iostream>
#include<vector>

using namespace std;


void dfsTraversal(std::vector<std::vector<int>>& adj, int src, std::vector<bool>& visited, std::vector<int>& res) {
    visited[src] = true;
    res.emplace_back(src);
    for (const auto& x : adj[src]) {
        if (!visited[x]) {
            dfsTraversal(adj, x, visited, res);
        }
    }
}

std::vector<int>dfsConnected(std::vector<std::vector<int>>& adj) {
    int V = adj.size();
    std::vector<bool> visited(V, false);
    std::vector<int> res{};

    for (int i = 0; i < V; i++) {
        if (!visited[i]) {
            dfsTraversal(adj, i, visited, res);
        }
    }
    return res;
}

void addEdge(vector<vector<int>>& adj, int u, int v) {
    // Implementation for adding edges to the graph
    adj[u].emplace_back(v);
    adj[v].emplace_back(u); // For undirected graph
}


int main(int argc, char** argv)
{
    int V = 6;
    std::vector<std::vector<int>> adj(V);
    
    // creating adjacency list
    addEdge(adj, 1, 2);
    addEdge(adj, 0, 3);
    addEdge(adj, 2, 0);
    addEdge(adj, 5, 4);

    std::vector<int> res = dfsConnected(adj); 
    
    for (const auto& it : res) {
        std::cout << it << " ";
    }
    std::cout << std::endl;
    return (0);
}