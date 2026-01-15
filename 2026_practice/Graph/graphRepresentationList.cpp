


/**
@brief: Input given 
n and m which is node and edges

For eg

n=5 m=6

Nodes can be constant but edges can be anything



*/


#include <iostream>
#include <vector>

struct Edge {
    int to;
    int w;
};

constexpr int vertexNumber {6};

int main() {
    std::vector<Edge> adjacency[vertexNumber + 1];

    auto addEdge = [&] (int nodeVertex, int edge, int w, bool directed = false)  {
        adjacency[nodeVertex].push_back({edge, w});
        if (!directed) {
			adjacency[edge].push_back({nodeVertex, w});
		}
    };

    addEdge(1, 2, 10);
    addEdge(1, 3, 5);
    addEdge(1, 5, 2);
    addEdge(2, 4, 7, true);
    addEdge(3, 5, 3);

    for (int i = 1; i <= vertexNumber; ++i) {
        std::cout << "Vertex: " << i << " ";
        /*for (const auto& e : adjacency[i]) {
            std::cout << "Neighbour: " << e.to << " (w=" << e.w << ") ";
        }*/
		
		for (const auto& [nbr, w] : adjacency[i]) {
			std::cout << "Neighbour: " << nbr << " (w=" << w << ") ";
		}
        std::cout << std::endl;
    }
    return 0;
}
