/**
 * @brief: Adjacency List representation of directed/undirected non-weighted Graph
 */

#include <iostream>
#include <vector>
#include <functional>

using namespace std;

constexpr int N = 10;





int main(int argc, char** argv)
{
    std::vector<int> adj[N+1]; // Adjacency List representation
    auto add_edge = [&](int u, int v, bool directed = false) {
        adj[u].push_back(v);
        if (!directed) {
            adj[v].push_back(u);
        }
    };
    // Example edges
    add_edge(1, 2);
    add_edge(1, 3);
    add_edge(2, 4, true); // Directed edge from 2 to    4
    add_edge(3, 5);
    // Print adjacency list
    for (int i = 1; i <= N; ++i) {
        cout << "Node " << i << ": ";
        for (int neighbor : adj[i]) {
            cout << neighbor << " ";
        }
        cout << endl;
    }
    return (0);
}