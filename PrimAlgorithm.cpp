#include <iostream>
#include <vector>
#include <climits>

#include "MSTAlgorithm.hpp"

using namespace std;

class PrimAlgorithm : public MSTAlgorithm {
public:
    PrimAlgorithm() {}

    Graph calculateMST(const Graph &g) {
        vector<vector<int>> graph = g.getAdjacencyMatrix();
        int n = graph.size();

        // Initialize MST adjacency matrix with -1 (no edge)
        vector<vector<int>> mst(n, vector<int>(n, -1));

        // Arrays to store information for Prim's algorithm
        vector<int> key(n, INT_MAX);  // Stores the minimum weight edge for each vertex
        vector<int> parent(n, -1);    // Stores the parent of each vertex in the MST
        vector<bool> inMST(n, false); // True if the vertex is included in the MST

        // Start from the first vertex
        key[0] = 0;

        // Prim's algorithm
        for (int count = 0; count < n - 1; ++count) {
            // Find the vertex with the minimum key value not yet in the MST
            int minKey = INT_MAX, u = -1;
            for (int v = 0; v < n; ++v) {
                if (!inMST[v] && key[v] < minKey) {
                    minKey = key[v];
                    u = v;
                }
            }

            // Add the selected vertex to the MST
            inMST[u] = true;

            // Update key and parent arrays for the adjacent vertices
            for (int v = 0; v < n; ++v) {
                if (graph[u][v] != -1 && !inMST[v] && graph[u][v] < key[v]) {
                    key[v] = graph[u][v];
                    parent[v] = u;
                }
            }
        }

        // Construct the MST using the parent array
        for (int v = 1; v < n; ++v) {
            if (parent[v] != -1) {
                mst[v][parent[v]] = graph[v][parent[v]];
                mst[parent[v]][v] = graph[v][parent[v]];
            }
        }
        Graph Tree;
        Tree.New_graph(mst);
        return Tree;
    }
};