#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <unordered_map>
#include <limits>
#include <utility>

class Graph {
public:
    // Add a vertex to the graph
    void addVertex(int vertex);

    // Add a directed edge from vertex u to vertex v with a given weight
    void addEdge(int u, int v, double weight);

    // Get neighbors of a vertex
    std::vector<int> getNeighbors(int vertex) const;

    // Get the weight of an edge
    double getEdgeWeight(int u, int v) const;

private:
    // Adjacency list representation of the graph
    std::unordered_map<int, std::unordered_map<int, double>> adjList;
};
#endif // GRAPH_HPP
