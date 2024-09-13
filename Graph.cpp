#include "Graph.hpp"

// Add a vertex to the graph
void Graph::addVertex(int vertex) {
    if (adjList.find(vertex) == adjList.end()) {
        adjList[vertex] = std::unordered_map<int, double>();
    }
}

// Add a directed edge from vertex u to vertex v with a given weight
void Graph::addEdge(int u, int v, double weight) {
    addVertex(u);
    addVertex(v);
    adjList[u][v] = weight;
}

// Get neighbors of a vertex
std::vector<int> Graph::getNeighbors(int vertex) const {
    std::vector<int> neighbors;
    auto it = adjList.find(vertex);
    if (it != adjList.end()) {
        for (const auto& pair : it->second) {
            neighbors.push_back(pair.first);
        }
    }
    return neighbors;
}

// Get the weight of an edge
double Graph::getEdgeWeight(int u, int v) const {
    auto it = adjList.find(u);
    if (it != adjList.end()) {
        auto edgeIt = it->second.find(v);
        if (edgeIt != it->second.end()) {
            return edgeIt->second;
        }
    }
    return std::numeric_limits<double>::infinity(); // Return infinity if edge does not exist
}