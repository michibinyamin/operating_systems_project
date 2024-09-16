#pragma once
#include <iostream>
#include <vector>
#include <climits>
#include <queue>

class Tree {
public:
    Tree(int n) : adj(n), parent(n, -1), depth(n, 0) {}

    void addEdge(int parent, int child) {
        adj[parent].push_back(child);
        adj[child].push_back(parent); // Since it's an undirected tree
    }

    void computeDepths() {
        std::queue<int> q;
        q.push(0); // assuming node 0 is the root
        while (!q.empty()) {
            int node = q.front();
            q.pop();
            for (int child : adj[node]) {
                if (child != parent[node]) {
                    depth[child] = depth[node] + 1;
                    parent[child] = node;
                    q.push(child);
                }
            }
        }
    }

    void printTree() const {
        std::cout << "Tree Structure:" << std::endl;
        for (int i = 0; i < adj.size(); ++i) {
            std::cout << "Node " << i << " (Parent: " << parent[i] << "): ";
            for (int child : adj[i]) {
                if (child != parent[i]) { // Avoid printing parent node
                    std::cout << child << " ";
                }
            }
            std::cout << std::endl;
        }
    }

private:
    std::vector<std::vector<int>> adj;
    std::vector<int> parent;
    std::vector<int> depth;
};