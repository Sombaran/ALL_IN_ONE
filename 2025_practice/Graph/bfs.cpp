/**
 * @brief: Breadth First Search (BFS) implementation in C++
 * @date: 2025-10-10
 * @details: This code demonstrates the BFS algorithm on a graph represented using an adjacency list.
 *           It includes functions to add edges and perform BFS traversal starting from a given node.
 *          The graph can be either directed or undirected based on the edge addition.
 *         The BFS traversal order is printed to the console.
 * @note: This implementation uses C++14 standard features.
 */

#include<iostream>
#include<vector>
#include<queue>

using namespace std;

constexpr int N = 10;

std::vector<int> adj[N+1]; // Adjacency List representation
auto add_edge = [](int u, int v, bool directed = false) {
    adj[u].push_back({v});
    if (!directed) {
        adj[v].push_back({u});
    }
};

auto bfsTraversal = [](int startNode)->std::vector<int> {
    std::vector<bool> visited(N+1, false);
    std::queue<int> q{};
    std::vector<int> traversalOrder{};
    visited[startNode] = true;
    q.push(startNode);

    while (!q.empty()) {
        int currentNode = q.front();
        q.pop();
        cout << "Visited Node: " << currentNode << endl;
        traversalOrder.emplace_back(currentNode);
        for (const auto& neighbor : adj[currentNode]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
    return traversalOrder;
};


/**
 * @brief: In a disconnected graph, some vertices may not be reachable from a single source. 
 * To ensure all vertices are visited in BFS traversal, we iterate through each vertex, 
 * and if any vertex is unvisited, we perform a BFS starting from that vertex being the source. 
 * This way, BFS explores every connected component of the graph.
 */


// BFS for a single connected component
void bfsConnected(vector<vector<int>>& adj, int src, vector<bool>& visited, vector<int>& res) {
    queue<int> q;
    visited[src] = true;
    q.push(src);

    while (!q.empty()) {
        int curr = q.front();
        q.pop();
        res.push_back(curr);

        // visit all the unvisited
        // neighbours of current node
        for (int x : adj[curr]) {
            if (!visited[x]) {
                visited[x] = true;
                q.push(x);
            }
        }
    }
}


// BFS for all components (handles disconnected graphs)
vector<int> bfs(vector<vector<int>>& adj) {
    int V = adj.size();
    vector<bool> visited(V, false);
    vector<int> res;

    for (int i = 0; i < V; i++) {
        if (!visited[i])
            bfsConnected(adj, i, visited, res);
    }
    return res;
}
//Driver Code Starts
void addEdge(vector<vector<int>>& adj, int u, int v) {
    adj[u].push_back(v);
    adj[v].push_back(u);
}


int main(int argc, char** argv)
{
    // Example edges
    add_edge(1, 2);
    add_edge(0, 3);
    add_edge(2, 4); // Directed edge from 2 to    4
    add_edge(1, 3);
    add_edge(2, 5);
    add_edge(4, 6);
    add_edge(2, 0);
    add_edge(3, 5);
    add_edge(5, 4);
    #if 0
    // Print adjacency list
    for (int i = 1; i <= N; ++i) {
        cout << "Node " << i << ": ";
        for (int neighbor : adj[i]) {
            cout << neighbor << " ";
        }
        cout << endl;
    }
    #endif
    std::vector<int> bfsOrder = bfsTraversal(1);
    cout << "BFS Traversal Order: ";
    for (const auto& node : bfsOrder) {
        cout << node << " ";
    }
    cout << endl;

    // Example of BFS on disconnected graph
        int V = 6;
    std::vector<std::vector<int>> adj(V);
    
    // creating adjacency list
    addEdge(adj, 1, 2);
    addEdge(adj, 0, 3);
    addEdge(adj, 2, 0);
    addEdge(adj, 5, 4);
    
    vector<int> res = bfs(adj);

    for (const auto& i : res){
        std::cout << i << " ";
    }
    std::cout << std::endl;
    return (0);
}