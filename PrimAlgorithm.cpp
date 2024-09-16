#include <vector>
#include <limits>
#include <queue>
#include "MSTAlgorithm.hpp"

class PrimAlgorithm : public MSTAlgorithm {
public:
    PrimAlgorithm() {}

    Tree calculateMST(const Graph& graph) override{
        int n = graph.getAdjacencyMatrix().size();
        std::vector<int> key(n, INT_MAX);
        std::vector<bool> inMST(n, false);
        std::vector<int> parent(n, -1);

        key[0] = 0; // Start from node 0
        parent[0] = -1; // Root node has no parent

        for (int count = 0; count < n - 1; ++count) {
            int u = -1;
            for (int i = 0; i < n; ++i) {
                if (!inMST[i] && (u == -1 || key[i] < key[u])) {
                    u = i;
                }
            }

            inMST[u] = true;

            for (int v = 0; v < n; ++v) {
                if (graph.getAdjacencyMatrix()[u][v] && !inMST[v] && graph.getAdjacencyMatrix()[u][v] < key[v]) {
                    key[v] = graph.getAdjacencyMatrix()[u][v];
                    parent[v] = u;
                }
            }
        }

        Tree tree(n);
        for (int i = 1; i < n; ++i) {
            tree.addEdge(parent[i], i);
        }
        tree.computeDepths();
        return tree;
    }
};