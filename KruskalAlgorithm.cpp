#include <iostream>
#include <vector>
#include <algorithm>
#include "MSTAlgorithm.hpp"
using namespace std;



class KruskalAlgorithm : public MSTAlgorithm {
public:
    KruskalAlgorithm() {}
    //~MSTAlgorithm() {}

    Graph calculateMST(const Graph &g) {
        vector<vector<int>> graph = g.getAdjacencyMatrix();
        int n = graph.size();
        vector<Edge> edges;

        // Convert adjacency matrix to edge list
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                if (graph[i][j] != -1) {
                    edges.emplace_back(i, j, graph[i][j]);
                }
            }
        }

        // Sort edges by weight
        sort(edges.begin(), edges.end(), compareEdge);

        // Initialize Union-Find data structures
        vector<int> parent(n);
        vector<int> rank(n, 0);
        for (int i = 0; i < n; ++i) {
            parent[i] = i;
        }

        // MST result as adjacency matrix with -1 indicating no edge
        vector<vector<int>> mst(n, vector<int>(n, -1));

        // Kruskal's algorithm
        int edgeCount = 0;
        for (const auto &edge : edges) {
            if (find(edge.u, parent) != find(edge.v, parent)) {
                unionSets(edge.u, edge.v, parent, rank);
                mst[edge.u][edge.v] = edge.weight;
                mst[edge.v][edge.u] = edge.weight;
                edgeCount++;
                if (edgeCount == n - 1) {
                    break;
                }
            }
        }
        Graph Tree;
        Tree.New_graph(mst);
        return Tree;
    }

private:
    // Struct to represent an edge
    struct Edge {
        int u, v, weight;
        Edge(int u, int v, int weight) : u(u), v(v), weight(weight) {}
    };

    // Comparator to sort edges by weight
    static bool compareEdge(const Edge &a, const Edge &b) {
        return a.weight < b.weight;
    }

    // Find function for Union-Find (Disjoint Set)
    int find(int node, vector<int> &parent) {
        if (parent[node] != node) {
            parent[node] = find(parent[node], parent);  // Path compression
        }
        return parent[node];
    }

    // Union function for Union-Find (Disjoint Set)
    void unionSets(int u, int v, vector<int> &parent, vector<int> &rank) {
        int rootU = find(u, parent);
        int rootV = find(v, parent);
        if (rootU != rootV) {
            if (rank[rootU] > rank[rootV]) {
                parent[rootV] = rootU;
            } else if (rank[rootU] < rank[rootV]) {
                parent[rootU] = rootV;
            } else {
                parent[rootV] = rootU;
                rank[rootU]++;
            }
        }
    }
};
