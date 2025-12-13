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



int main(int argc, char** argv)
{
    // Example edges
    add_edge(1, 2);
    add_edge(0, 3);
    add_edge(2, 4, true); // Directed edge from 2 to    4
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
    return (0);
}