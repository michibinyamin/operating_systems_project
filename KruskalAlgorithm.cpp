#include <vector>
#include <algorithm>
#include <limits>
#include "MSTAlgorithm.hpp"

class KruskalAlgorithm : public MSTAlgorithm {
public:
    KruskalAlgorithm() {}

    Tree calculateMST(const Graph& graph) override {
        int n = graph.getAdjacencyMatrix().size();
        std::vector<Edge> edges;

        // Build the edge list
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                int weight = graph.getAdjacencyMatrix()[i][j];
                if (weight > 0) {
                    edges.push_back({i, j, weight});
                }
            }
        }

        // Sort edges by weight
        std::sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
            return a.weight < b.weight;
        });

        // Initialize Union-Find structure
        UnionFind uf(n);

        Tree tree(n);

        for (const auto& edge : edges) {
            int u = edge.u;
            int v = edge.v;
            if (uf.find(u) != uf.find(v)) {
                uf.unionSets(u, v);
                tree.addEdge(u, v);
            }
        }

        tree.computeDepths();
        return tree;
    }

private:
    struct Edge {
        int u, v, weight;
    };

    class UnionFind {
    public:
        UnionFind(int size) : parent(size), rank(size, 0) {
            for (int i = 0; i < size; ++i) {
                parent[i] = i;
            }
        }

        int find(int x) {
            if (parent[x] != x) {
                parent[x] = find(parent[x]); // Path compression
            }
            return parent[x];
        }

        void unionSets(int x, int y) {
            int rootX = find(x);
            int rootY = find(y);
            if (rootX != rootY) {
                // Union by rank
                if (rank[rootX] > rank[rootY]) {
                    parent[rootY] = rootX;
                } else if (rank[rootX] < rank[rootY]) {
                    parent[rootX] = rootY;
                } else {
                    parent[rootY] = rootX;
                    ++rank[rootX];
                }
            }
        }

    private:
        std::vector<int> parent;
        std::vector<int> rank;
    };
};
