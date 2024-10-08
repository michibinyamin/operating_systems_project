#pragma once
#include "Graph.cpp"
#include "Functions.hpp"
//Interface (Strategy)- implements by Prim and Kruskal

class MSTAlgorithm {
public:
    virtual ~MSTAlgorithm() = default;

    //calculate MST
    virtual Graph calculateMST(const Graph& graph) = 0;
};