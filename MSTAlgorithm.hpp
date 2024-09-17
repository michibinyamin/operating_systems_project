#pragma once
#include "Graph.cpp"
#include "Tree.cpp"
//Interface (Strategy)- implements by Prim and Kruskal

class MSTAlgorithm {
public:
    virtual ~MSTAlgorithm() = default;

    //calculate MST
    virtual Tree calculateMST(const Graph& graph) = 0;
};